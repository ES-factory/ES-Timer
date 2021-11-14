#define INCLUDE_OLED          false    // Change it to true if you assemble monitor version
#define BRIGHTNESS_OLED       0.5      // In Range of 0 to 1
#define FLIP_CONTENT          false    // If you have connect your ES Timer to right side of computer should change it to true 

#define BE_SAVE_TIMER_STATUS  true     // If you don't want to save status of the timer (awake and sleep), change it to false

#include <ESTimer.h>
#if INCLUDE_OLED
#include "Fonts.h"
#include "Images.h"
#endif

#define seconds(n) (n)
#define minutes(n) (n * seconds(60))

#define AWAKE_TIME min(minutes(25), minutes(99))
#define SLEEP_TIME min(minutes(5), AWAKE_TIME / 4)

#define LED_PIN 1
#define TIMER_STATUS_ADDRESS 0
#define COUNT_DP_ADDRESS     TIMER_STATUS_ADDRESS + sizeof(timerStatus)

bool timerStatus = true;
uint8_t countDonePomodoros = 0;

bool isCompletedRest = false;

void setup() {
  initTimer();
  startTimer();
}

void loop() {
  while (ESTimer.onSleepMode()) {
    // This part of the code runs only when Windows is on sleep mode
    if (!isCompletedRest) {
#if INCLUDE_OLED
      updateStatusPomodoros();
#endif
      startCountdownTimer();

      EEPROM.put(COUNT_DP_ADDRESS, ++countDonePomodoros);
      isCompletedRest = true;
    }
    flashLight();
  }

  // This part of the code runs only when Windows is ready
  if (isCompletedRest) {
    startTimer();
  }
}

void initTimer() {
  DDRB |= 0b10; // pinMode(LED_PIN, OUTPUT);

  EEPROM.get(COUNT_DP_ADDRESS, countDonePomodoros);

#if INCLUDE_OLED
#if FLIP_CONTENT
  ESTimer.flipContent();
#endif
  splash();

  initNumbers();
  initStatusPomodoros();
#endif
}

void startTimer() {
  PORTB &= 0b01; // digitalWrite(LED_PIN, LOW);

#if BE_SAVE_TIMER_STATUS
  EEPROM.get(TIMER_STATUS_ADDRESS, timerStatus);
#endif

  if (timerStatus) {
#if INCLUDE_OLED
    updateStatusPomodoros();
#endif

    if (!ESTimer.onSleepMode()) {
      startCountdownTimer();
    } else {
      while (ESTimer.onSleepMode()) flashLight();
      startTimer();
    }
  }

  while (!ESTimer.onSleepMode()) {
    ESTimer.goToSleepMode();
    ESTimer.delay(100);
  }
  isCompletedRest = false;
}

void flashLight() {
  PORTB |= 0b10; // digitalWrite(LED_PIN, HIGH);
  delay(500);
  PORTB &= 0b01; // digitalWrite(LED_PIN, LOW);
}

void startCountdownTimer() {
  uint16_t totalTime = timerStatus ? AWAKE_TIME : SLEEP_TIME * ((countDonePomodoros + 1) % 4 == 0 ? 4 : 1);

  do {
    uint16_t previousTime = (uint16_t) millis();

#if INCLUDE_OLED
    uint8_t m0 = (totalTime / 60) / 10;
    uint8_t m1 = (totalTime / 60) % 10;
    uint8_t s0 = (totalTime % 60) / 10;
    uint8_t s1 = (totalTime % 60) % 10;

    drawNumbers(m0, m1, s0, s1);
#endif

    if (!timerStatus) ESTimer.goToSleepMode();

    ESTimer.delay(1000 - ((uint16_t) millis() - previousTime));
  } while (totalTime-- > 0);

  timerStatus = !timerStatus;
#if BE_SAVE_TIMER_STATUS
  EEPROM.put(TIMER_STATUS_ADDRESS, timerStatus);
#endif
}

#if INCLUDE_OLED
void splash() {
  ESTimer.drawBitmap(46, 0, 82, 8, es_logo);
  ESTimer.delay(3000);
  ESTimer.clear();
}

void drawNumbers(uint8_t m0, uint8_t m1, uint8_t s0, uint8_t s1) {
  ESTimer.drawBitmap(65, 1, 100, 4, numbers[m0]);
  ESTimer.drawBitmap(65, 4, 100, 7, numbers[m1]);
  ESTimer.drawBitmap(28, 1, 63,  4, numbers[s0]);
  ESTimer.drawBitmap(28, 4, 63,  7, numbers[s1]);
}

void initNumbers() {
  uint8_t t = 0;
  drawNumbers(t, t, t, t);
}

void initStatusPomodoros() {
  for (uint8_t i = 0; i < 8; i++) {
    drawPomodoro(i, dots[(i <= countDonePomodoros % 8) ^ ((countDonePomodoros / 8) % 2)]);
  }
}

void updateStatusPomodoros() {
  uint8_t i = countDonePomodoros % 8;
  drawPomodoro(i, dots[((countDonePomodoros / 8) + 1) % 2]);
}

void drawPomodoro(uint8_t index, const byte *s) {
  uint8_t rowOffset = index < 4 ? 104 : 0;
  ESTimer.drawBitmap(
    4 + rowOffset,
    2 * (index % 4),
    20 + rowOffset,
    (2 * (index % 4)) + 2,
    s
  );
}
#endif
