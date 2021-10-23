# ⏲ ES Timer 
<img src="https://github.com/ErfanSn/ES-Timer/blob/master/media/usb_pomodoro_timer.png" align="right" 
     alt="ES Timer" width="175" height="180">

Powered⚡ by `Digispark ATtiny85` and it works based on 🍅 pomodoro time management technique a special timer for all those that work a lot with computer and have difficulty managing time. Features of ES Timer in compare to existing softwares:
- forcing to follow the simple law of this technique
- it is simple and cheap to make less than 10$
- it can store in memory awake and sleep time and count of pomodoros and show them

## Contents
1. [How it works](#how-it-works)
2. [Requirements](#requirements) 
3. [Assemble monitor version](#assemble-monitor-version) (Optional)
4. [Initialization arduino ide for Digispark ATtiny85](#initialization-arduino-ide-for-digispark-attiny85)
5. [Upload code](#upload-code)
6. [Configurations](#configurations)
7. [Demos](#demos)
8. [Donations](#donations)
10. [License](#license)

## How it works
As mentioned, this timer works according to the `Pomodoro technique`, which you can refer to [this link](https://everhour.com/blog/pomodoro-technique/) for more information about the technique. it works in such a way that when the user connects it to the port of her/his computer, it starts countdown according to the time specified by the user, and the user should to do his task during that time. when counter reach to zero, it using the [HID](https://en.wikipedia.org/wiki/Human_interface_device) protocol goes the computer to sleep mode and it starts counting again for user rest. From this moment, it is not possible to use the computer by user and it cannot wake up the computer until the counting is over.

## Requirements
- [Arduino IDE](https://www.arduino.cc/en/software)
- Digispark ATtiny85
- For monitor version (Optional)
  - Female and male pin header standard
  - LED
  - PCB 10x10 pin
  - Soldering iron and wire
  - SSD1306 I2C OLED
  - Wire

## Assemble monitor version

| ![Step 1](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_1.jpg) | ![Step 2](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_2.jpg) | ![Step 3](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_3.jpg) |
| :---: | :---: | :---: |
| **Step 1** | **Step 2** | **Step 3** |

| ![Step 4](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_4.jpg) | ![Step 5](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_5.jpg) | ![Step 6](https://github.com/ErfanSn/ES-Timer/blob/master/media/step_6.jpg) |
| :---: | :---: | :---: |
| **Step 4** | **Step 5** | **Step 6** |

1. Cut your PCB into square shape (10x10 holes) with cutter.
2. Break female pin header into two pins 2 and 4 and put them on pcb like top photo then soldering them legs.
3. The same work do on male pin header with the difference that, you have to break it into 3 and 6 pins, look to related picture.
4. In this step putting digispark attiny85 on male pin headers and soldering their top legs related to 5V, GND, P0, P1, P2.
5. According to the picture of the fifth step, do wiring and solder  on the back of pcb.
6. In the end, put OLED and LED on pin headers, in this moment your ES Timer is ready for next step.

## Initialization arduino ide for Digispark ATtiny85
If this is the first time you have used `Digispark ATtiny85` in `Arduino IDE`, you need to follow these steps
[to see click here](http://digistump.com/wiki/digispark/tutorials/connecting) please.

## Upload code
After opening the file  `EsTimer.ino` in `Arduino IDE` In order to be able to upload the code on the device, you must add the zip files in the `lib` folder to IDE To do this, follow the path:

`Arduino IDE Toolbar -> Sketch -> Include Library -> Add .ZIP Library...`

If you have assembled the monitor version, you need to change the value of `INCLUDE_OLED` to `1`. Now to upload the code in `ES Timer` click on the arrow button or use shortcut `Ctrl + U`. Wait for the code to be compiled. After you see the `Please plug in the device ...` message. Connect the device to the computer. After a maximum of 5 seconds, the code will be uploaded. Congratulations 🎉, your ES Timer is ready to use.

## Configurations
You can customize the awake and sleep system. All you have to do is change the definition values of `AWAKE_TIME` and `SLEEP_TIME`.

**Note 📝**: According to the code, the maximum definition time for `AWAKE_TIME` is 99 minutes and for `SLEEP_TIME` is a quarter `AWAKE_TIME`.

## Demos
| **Monitor version** | **Only  digispark** |
| :---: | :---: |
| ![Monitor version](https://github.com/ErfanSn/ES-Timer/blob/master/media/monitor_version.gif) | ![Only digispark](https://github.com/ErfanSn/ES-Timer/blob/master/media/only_digispark.gif) |

## Donations
This is free, open-source software. If you'd like to support the development of future projects, or say thanks for this one, you can donate:

RIAL at `6037701527276452`

BTC at `bc1q0l8rn8y5rlka278ypq6u4dvapyue7d64uvhgct`

![BTC qrcode](https://github.com/ErfanSn/ES-Timer/blob/master/media/btc_qrcode.png)

## License
```
Copyright (C) 2021  Erfan Sn

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see https://www.gnu.org/licenses/.
```
