# ⏲ ES Timer 
<img src="/media/usb_pomodoro_timer.png" align="right" 
     alt="ES Timer" width="175" height="180">

A product from [ES Factory](https://www.instagram.com/es_factory_official/), powered⚡ by Digispark ATtiny85 and it works based on 🍅 pomodoro time management technique a timer especially for those who deal with computers a lot, such as freelancers and have difficulty managing time. 

Features of ES Timer in compare to existing softwares:
- forcing to follow the breaks between tasks
- it is simple and cheap to make around 10$
- It is not dependent on the special OS and can works in the windows, linux and mac os without install anything

## Contents
1. [How it works](#how-it-works)
2. [Requirements](#requirements) 
3. [Assemble monitor version](#assemble-monitor-version) (Optional)
4. [Update from v1.x.x to v2.x.x](#update-from-v1xx-to-v2xx)
5. [Initialization Arduino IDE and Digispark ATtiny85](#initialization-arduino-ide-and-digispark-attiny85)
6. [Upload code](#upload-code)
7. [Configurations](#configurations)
8. [Demos](#demos)
9. [Donations](#donations)
10. [License](#license)
11. [Acknowledgments](#acknowledgments)

## How it works
As mentioned, this timer works according to the `Pomodoro technique`, which you can refer to [this link](https://everhour.com/blog/pomodoro-technique/) for more information about the technique. it works in such a way that when the user connects it to the port of a computer, it starts countdown according to the time specified by the user, and the user should to do his task during that time. when counter reach to zero, it using the [HID](https://en.wikipedia.org/wiki/Human_interface_device) protocol goes the computer to lock screen and it starts counting again for user rest. From this moment, it is not possible to use the computer by user and it cannot unlock the computer until the counting is over.

> **Note 📝**: To let the timer know that you want to start the next Pomadoro, you have to keystroke the `CapsLock` key twice with a one second delay.

## Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)
- Digispark ATtiny85
- For monitor version (Optional)
  - Female and male pin header standard
  - LED
  - PCB prototype board
  - Soldering iron and wire
  - SSD1306 I2C OLED
  - Wire

## Assemble monitor version

| ![Step 1](/media/step_1.jpg) | ![Step 2](/media/step_2.jpg) | ![Step 3](/media/step_3.jpg) |
| :---: | :---: | :---: |
| **Step 1** | **Step 2** | **Step 3** |

| ![Step 4](/media/step_4.jpg) | ![Step 5](/media/step_5.jpg) | ![Step 6](/media/step_6.jpg) |
| :---: | :---: | :---: |
| **Step 4** | **Step 5** | **Step 6** |

1. Cut your PCB into square shape (10x10 holes) with cutter.
2. Break female pin header into two pins 2 and 4 and put them on pcb like top photo then soldering them legs.
3. The same work do on male pin header with the difference that, you have to break it into 3 and 6 pins, look to related picture.
4. In this step putting digispark attiny85 on male pin headers and soldering their top legs related to 5V, GND, P0, P1, P2.
5. According to the picture of the fifth step, do wiring and solder  on the back of pcb.
6. In the end, put OLED and LED on pin headers, in this moment your ES Timer is ready for next step.

## Update from v1.x.x to v2.x.x
If you have used version 1.x.x ES Timer before, in order to update its, you must first delete the ES timer and EEPROM folders in the library folder of Arduino IDE which you can usually be found in the `C:\Users\USER\Documents\Arduino\libraries` path and update the bootloader of digispark attiny85 for this purpose, follow next step.

## Initialization Arduino IDE and Digispark ATtiny85

In order to be able to upload codes into `Digispark Attiny85`, you first need to add it to ide. To do this, follow these steps:

![Step 1](/media/initialization_steps.png)

1. In the Arduino IDE go to the **"File"** menu and select **"Preferences"**
2. In the box labeled **"Additional Boards Manager URLs"** enter and click OK
   ```
   https://raw.githubusercontent.com/ArminJo/DigistumpArduino/master/package_digistump_index.json
   ```
3. Go to the **"Tools"** menu and then the **"Board"** submenu - select **"Boards Manager"** and then type **"Digistump AVR Boards"** in filter box and click the **"Install"** button.
4. You'll see the download progress on the bottom bar of the **"Boards Manager"** window, when complete it will show **"Installed"** next to that item on the list
5. For Windows you must install the Digispark driver before you can program the board, download it [here](https://github.com/digistump/DigistumpArduino/releases/download/1.6.7/Digistump.Drivers.zip), open it and run InstallDrivers.exe (If you already have them installed, this installer will update them and install any that are missing)
6. With the install complete, close the **"Boards Manager"** window and select the **"Digispark"** from the `Tools → Board → Digistump AVR Boards` menu
7. Then select **"16.5 MHz - For V-USB"** from the `Tools → Clock` menu
8. After going through these steps, it is time to update the bootloader digispark attiny85, for which you only need to first click on the **"Born Bootloader"** option. After the message appears, connect the it to the usb port to start the update process.

## Upload code
After opening the file  **"ESTimer.ino"** in Arduino IDE In order to be able to upload the code on the device, you must add the zip file in the **"lib"** folder to IDE To do this, follow the path:

`Arduino IDE Toolbar → Sketch → Include Library → Add .ZIP Library...`

Now to upload the code, click on the arrow button or use shortcut `Ctrl + U`. Wait for the code to be compiled. After you see the `> Please plug in the device (will time out in 60 seconds) ... ` message. Connect the device to the computer. After a few seconds, the code will be uploaded. 

Congratulations 🎉, your ES Timer is ready to use.

> **Note 📝**: After clicking the upload option, you will see a few warning related to core code like the one below. Do not worry about this issue:
> ```
> C:\Users\USER\Documents\Arduino\libraries\ESTimer\ESTimerC.c: In function 'usbFunctionSetup':
> ....
> ```

## Configurations

| Definition | description |
| :--- | :--- |
| [`SYSTEM_OS`](/ESTimer.ino#L8) | To go to the Lock screen, you must specify your operating system type (WINDOWS or LINUX or MACOS). |
| [`INCLUDE_OLED`](/ESTimer.ino#L10) | Change it to true if you assemble monitor version. |
| [`ADDRESS_OLED`](/ESTimer.ino#L11) | Usually the ssd1306 address is this value, but if your ssd1306 value was different you can change it (use Digispark_Examples/i2cScanner to get it). |
| [`BRIGHTNESS_OLED`](/ESTimer.ino#L12) | In the range 0 to 1, where 1 is the maximum OLED brightness and 0 means it is completely off. |
| [`FLIP_CONTENT_OLED`](/ESTimer.ino#L13) | If you have connect your ES Timer to usb port of right side of computer should change it to true. |
| [`WORK_DURATION`](/ESTimer.ino#L15) | The duration of each work can be a maximum of 99 minutes, which you can use up to a second accuracy. |
| [`SHORT_BREAK_DURATION`](/ESTimer.ino#L16) | The duration of each short break can be a maximum of one-fifth work time, which you can use up to a second accuracy. |
| [`LONG_BREAK_RATE`](/ESTimer.ino#L17) | There should be take a long break after all four pomodoros, which is four times the short break by default. |
| [`SHORT_BREAK_COUNTER`](/ESTimer.ino#L18]) | The method of counting short break for long break, STATIC according by done pomodoros, DYNAMIC after plugging timet to system. |
| [`PAUSABLE_WORK_PHASE`](/ESTimer.ino#L19) | If you want the work phase counter to pause when the timer is unplugged and resume it when again plugged, change it to true. |

## Demos

| **Video** |
| :---: |
| [<img src="/media/youtube.png" width="560px">](https://www.youtube.com/watch?v=Ut2UdHNUHx0) |

| **Monitor version** | **Only  digispark** |
| :---: | :---: |
| <img src="/media/monitor_version.gif" width="200px"> | <img src="/media/only_digispark.gif" width="200px"> | 

## Donations
This is free, open-source software and instrument. If you'd like to support the development of future projects, or say thanks for this one, you can donate:

| **BTC** | **ETH** |
| :---: | :---: |
| ![BTC qrcode](/media/btc_qrcode.png) | ![ETH qrcode](/media/eth_qrcode.png) |
| bc1q0l8rn8y5rlka278ypq6u4dvapyue7d64uvhgct | C3e0fF9534F54e91439AD877149903E7bFa50920 |


## License
```
Copyright (C) 2021  Erfan Sn

ES Timer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ES Timer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.
```

## Acknowledgments

* [DigistumpArduino](https://github.com/ArminJo/DigistumpArduino)
* [V-USB](https://github.com/obdev/v-usb)
