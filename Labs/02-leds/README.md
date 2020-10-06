# Lab 2: Control of GPIO, LED, push button

### Learning objectives

The purpose of this laboratory exercise is to learn how to use basic input/output devices such as LEDs (Light Emitting Diodes) and push buttons, and how to control GPIO (General Purpose Input Output) pins with help of control registers.

## Preparation tasks (done before the lab at home)

Draw two basic ways to connect a LED to the output pin of the microcontroller: LED active-low, LED active-high. What is the name of the LED pin that is connected to the microcontroller in each case?

Diode active high:

![logic](../../Images/DiodeActiveHigh.PNG)

Diode active low:

![logic](../../Images/DiodeActiveLow.PNG)



[Calculate LED resistor value](https://electronicsclub.info/leds.htm) for typical red and blue LEDs.

![Clock period](../../Images/ohms_law.PNG)

| **LED color** | **Supply voltage** | **LED current** | **LED voltage** | **Resistor value** |
| :-: | :-: | :-: | :-: | :-: |
| red | 5&nbsp;V | 20&nbsp;mA | 2 | 150 Ω |
| blue | 5&nbsp;V | 20&nbsp;mA | 4 | 50 Ω|



Draw the basic ways to connect a push button to the microcontroller input pin: button active-low, button active-high.
&nbsp;

Button active high:

![logic](../../Images/PushActiveHigh.PNG)

Button active low:

![logic](../../Images/PushActiveLow.PNG)




A detailed description of working with input/output ports can be found in [ATmega328P datasheet](https://www.microchip.com/wwwproducts/en/ATmega328p) in section I/O-Ports.

Use the datasheet to find out the meaning of the DDRB and PORTB control register values and their combinations.

| **DDRB** | **Description** |
| :-: | :-- |
| 0 | Input pin |
| 1 | Output pin|

| **PORTB** | **Description** |
| :-: | :-- |
| 0 | Output low value |
| 1 | Output high value |

| **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
| :-: | :-: | :-: | :-: | :-- |
| 0 | 0 | input | no | Tri-state, high-impedance |
| 0 | 1 | input | yes | Activate pull-up resistor |
| 1 | 0 | output | no | Output low (sink) |
| 1 | 1 | output | no | Output high (source) |

See [schematic of Arduino Uno board](../../Docs/arduino_shield.pdf) in docs folder of Digital-electronics-2 repository and find out which pins of ATmega328P can be used as input/output pins. To which pin is the LED L connected? (PB5) Is it connected as active-low or active-high?(Active high)

| **Port** | **Pin** | **Input/output usage?** |
| :-: | :-: | :-- |
| A | x | Microcontroller ATmega328P does not contain port A |
| B | 0 | Yes (Arduino pin 8) |
|   | 1 | Yes (Arduino pin -9) |
|   | 2 | Yes (Arduino pin -10)|
|   | 3 | Yes (Arduino pin -11)|
|   | 4 | Yes (Arduino pin 12) |
|   | 5 | Yes (Arduino pin 13) LED L|
|   | 6 | not available as I/O pin |
|   | 7 | not available as I/O pin |
| C | 0 | Yes (Arduino pin A0) |
|   | 1 | Yes (Arduino pin A1)  |
|   | 2 | Yes (Arduino pin A2)  |
|   | 3 | Yes (Arduino pin A3)  |
|   | 4 | No (Arduino pin A4)  |
|   | 5 | Yes (Arduino pin A5)  |
|   | 6 | not available as I/O pin|
|   | 7 | not available as I/O pin|
| D | 0 | Yes (Arduino pin RX<-0) |
|   | 1 | Yes (Arduino pin TX<-0) |
|   | 2 | Yes (Arduino pin 2)  |
|   | 3 | Yes (Arduino pin -3)|
|   | 4 | Yes (Arduino pin 4) |
|   | 5 | Yes (Arduino pin -5)|
|   | 6 | Yes (Arduino pin -6)|
|   | 7 | Yes (Arduino pin 7) |
