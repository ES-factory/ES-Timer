#define WINDOWS               0x01
#define LINUX                 0x02
#define MACOS                 0x03

#define STATIC                0x04
#define DYNAMIC               0x05

#define SYSTEM_OS             WINDOWS                  // To go to the Lock screen, you must specify your operating system type (WINDOWS or LINUX or MACOS).

#define INCLUDE_OLED          false                    // Change it to true if you assemble monitor version.
#define ADDRESS_OLED          0x3C                     // Usually the ssd1306 address is this value, but if your ssd1306 value was different you can change it (use Digispark_Examples/i2cScanner to get it).
#define BRIGHTNESS_OLED       0.5                      // In the range 0 to 1, where 1 is the maximum OLED brightness and 0 means it is completely off.
#define FLIP_CONTENT_OLED     false                    // If you have connect your ES Timer to right side of computer should change it to true.

#define WORK_DURATION         minutes(25) + seconds(0) // The duration of each work can be a maximum of 99 minutes, which you can use up to a second accuracy.
#define SHORT_BREAK_DURATION  minutes(5) + seconds(0)  // The duration of each short break can be a maximum of one-fifth work time, which you can use up to a second accuracy.
#define LONG_BREAK_RATE       4.0                      // There should be take a long break after all four pomodoros, which is four times the short break by default.
#define SHORT_BREAK_COUNTER   STATIC                   // The method of counting short break, STATIC according by done pomodoros, DYNAMIC after plugging timet to system.
#define PAUSABLE_WORK_PHASE   false                    // If you want the work phase counter to pause when the timer is unplugged and resume it when again plugged, change it to true.

#define _WORK_DURATION min(WORK_DURATION, minutes(99))
#define BREAK_DURATION min((min(SHORT_BREAK_DURATION, WORK_DURATION / 5) * (currentTimerPhase == SHORT_BREAK ? 1 : max(LONG_BREAK_RATE, 1))), _WORK_DURATION)

#define seconds(n) (n)
#define minutes(n) (n * seconds(60))

#include "Images.h"
#include "Fonts.h"

#include <ESTimer.h>
#include <EEPROM.h>

#define REMAINING_CD_ADDRESS  0
#define COUNT_POMODO_ADDRESS  OFFSET_RCD_ADDRESS - sizeof(countPomodoros)
#define OFFSET_RCD_ADDRESS    WORK_TIME_ADDRESS - sizeof(offsetRCD)
#define WORK_TIME_ADDRESS     MAX_ADDRESS - sizeof(workTime)
#define MAX_ADDRESS           512

enum TimerPhase { WORK, SHORT_BREAK, LONG_BREAK };

TimerPhase currentTimerPhase = WORK;
uint16_t workTime = _WORK_DURATION;
uint16_t offsetRCD = 0;
uint8_t countPomodoros = 0;
uint8_t dynamicCountPomodoros = 0;
uint16_t remainingCountdownTime = 0;

void setup() {
  setPinsMode();
  initTimer();
#if INCLUDE_OLED
  initOLED();
#endif
}

void loop() {
#if INCLUDE_OLED
  showCurrentTimerPomodoro();
#endif

  startTimerWorkPhase();
  goToSystemLockScreen();
  startTimerBreakPhase();
  
  increaseCountPomodoros();
  awaitToNextPomodoroRequest();

#if INCLUDE_OLED
  updateCompletedTimerPomodoros();
#endif
}

void setPinsMode() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void initTimer() {
  changeImmediatelyWorkTimeIfWasChanged();
  fetchValuesFromEEPROM();
}

void changeImmediatelyWorkTimeIfWasChanged() {
  uint16_t temp;
  EEPROM.get(WORK_TIME_ADDRESS, temp);
  if (temp != workTime) {
#if PAUSABLE_WORK_PHASE
    safeStoreRemainingTime(0);
#endif
    EEPROM.put(WORK_TIME_ADDRESS, workTime);
  }
}

void fetchValuesFromEEPROM() {
#if PAUSABLE_WORK_PHASE
  EEPROM.get(OFFSET_RCD_ADDRESS, offsetRCD);
  EEPROM.get(REMAINING_CD_ADDRESS + offsetRCD, remainingCountdownTime);
#endif
  EEPROM.get(COUNT_POMODO_ADDRESS, countPomodoros);
}

void startTimerWorkPhase() {
  if (timerPhaseIsWork()) startCountdownTimer();
}

void goToSystemLockScreen() {
  ESTimer.goToSystemLockScreen();
  ESTimer.delay(500);
}

void startTimerBreakPhase() {
  if (!timerPhaseIsWork()) startCountdownTimer();
}

void increaseCountPomodoros() {
  EEPROM.put(COUNT_POMODO_ADDRESS, ++countPomodoros);
  dynamicCountPomodoros++;
}

void awaitToNextPomodoroRequest() {
  while (!ESTimer.receivedRequestFromUser()) blinkLED();
}

void blinkLED() {
  turnOnLED();
  ESTimer.delay(500);
  turnOffLED();
  ESTimer.delay(500);
}

void turnOnLED() {
  digitalWrite(LED_BUILTIN, HIGH);
}

void turnOffLED() {
  digitalWrite(LED_BUILTIN, LOW);
}

void startCountdownTimer() {
  uint16_t remainingTime = timerPhaseIsWork() ? (remainingCountdownTime > 0 ? remainingCountdownTime : _WORK_DURATION) : BREAK_DURATION;

  do {
    uint16_t start = (uint16_t) millis();

#if INCLUDE_OLED
    updateTimerCounter(remainingTime);
#endif

    if (timerPhaseIsWork()) {
#if PAUSABLE_WORK_PHASE
      safeStoreRemainingTime(remainingTime);
#endif
      if (remainingTime <= seconds(15)) turnOnLED();      
    } else {
      ESTimer.goToSystemLockScreen();
      turnOffLED();
    }
    
    uint16_t takenTime = (uint16_t) millis() - start;
    ESTimer.delay(1000 - takenTime);
  } while (remainingTime-- > 0);

  changeTimerPhaseToNext();
}

void safeStoreRemainingTime(uint16_t remainingTime) {
  uint16_t temp;
  do {
    EEPROM.put(REMAINING_CD_ADDRESS + offsetRCD, remainingTime);
    EEPROM.get(REMAINING_CD_ADDRESS + offsetRCD, temp);
  } while (temp != remainingTime && increaseOffsetRCD());
  EEPROM.get(REMAINING_CD_ADDRESS + offsetRCD, remainingCountdownTime);
}

bool increaseOffsetRCD() {
  offsetRCD += sizeof(remainingCountdownTime);
  EEPROM.put(OFFSET_RCD_ADDRESS, offsetRCD);
  return true;
}

bool timerPhaseIsWork() {
  return currentTimerPhase == WORK;
}

void changeTimerPhaseToNext() {
  currentTimerPhase = getNextTimerPhase();
}

TimerPhase getNextTimerPhase() {
  switch (currentTimerPhase) {
    case SHORT_BREAK:
    case LONG_BREAK:
      return WORK;
    default:
      uint8_t nextPomodoro = currentCountPomodoros() + 1;
      return nextPomodoro % 4 ? SHORT_BREAK : LONG_BREAK;
  }
}

uint8_t currentCountPomodoros() {
  return SHORT_BREAK_COUNTER == DYNAMIC ? dynamicCountPomodoros : countPomodoros;
}

void initOLED() {
  ESTimer.initSSD1306();
  
  showSplashScreen();
  showTimerCounter();
  showCompletedTimerPomodoros();
}

void showSplashScreen() {
  ESTimer.drawBitmap(46, 0, 82, 8, es_logo);
  ESTimer.delay(3000);
  ESTimer.clearOLED();
}

void showTimerCounter() {
  uint8_t zero = 0;
  drawNumbers(zero, zero, zero, zero);
}

void showCompletedTimerPomodoros() {
  for (uint8_t index = 0; index < 8; index++) {
    drawPomodoro(index, dots[(index < currentCountPomodoros() % 8) ^ ((currentCountPomodoros() / 8) % 2)]);
  }
}

void updateTimerCounter(uint16_t totalTime) {
  uint8_t leftDigitOfMinute = (totalTime / 60) / 10;
  uint8_t rightDigitOfMinute = (totalTime / 60) % 10;
  uint8_t leftDigitOfSecound = (totalTime % 60) / 10;
  uint8_t rightDigitOfSecound = (totalTime % 60) % 10;

  drawNumbers(leftDigitOfMinute, rightDigitOfMinute, leftDigitOfSecound, rightDigitOfSecound);
}

void updateCompletedTimerPomodoros() {
  uint8_t countCompletedPomodoros = currentCountPomodoros() - 1;
  uint8_t index = countCompletedPomodoros % 8;
  drawPomodoro(index, dots[((countCompletedPomodoros / 8) + 1) % 2]);
}

void showCurrentTimerPomodoro() {
  uint8_t index = currentCountPomodoros() % 8;
  drawPomodoro(index, dots[2 + (currentCountPomodoros() / 8) % 2]);
}

void drawNumbers(uint8_t topLeft, uint8_t topRight, uint8_t bottomLeft, uint8_t bottomRight) {
  ESTimer.drawBitmap(65, 1, 100, 4, numbers[topLeft]);
  ESTimer.drawBitmap(65, 4, 100, 7, numbers[topRight]);
  ESTimer.drawBitmap(28, 1, 63,  4, numbers[bottomLeft]);
  ESTimer.drawBitmap(28, 4, 63,  7, numbers[bottomRight]);
}

void drawPomodoro(uint8_t index, const byte *bitmap) {
  uint8_t rowOffset = index < 4 ? 104 : 0;
  ESTimer.drawBitmap(
    4 + rowOffset,
    2 * (index % 4),
    20 + rowOffset,
    (2 * (index % 4)) + 2,
    bitmap
  );
}
