<h1> Project DE-2 </h1>
Authors: Filip Paul, Kristýna Pijáčková

<h2> Project objectives </h2>
<p>Application of analog signal generator using 8-bit R-2R DAC with several preset signal types. User interface: 4x3 keypad control, 2x16 LCD, UART.</p>
<h2> Hardware description </h3>
<p>The brain of the generator is the 8-bit microcontroller Atmega238p. This MCU is soldered with few more components (voltage regulators, 16MHz oscillator, USB interface etc.) on the premade board called Arduino Uno. ATMEGA 328p has 21 GPIO pins. These pins can be accessed by registers PORTB, PORTC and PORTD. In our case we will use PORTD pins to control LCD with Hitachi HD44780 driver, PORTB as input to 8-bit R-2R DAC and PORTC to scan 4x3 keypad matrix </p>
<h3>R-2R 8-bit DAC</h3>
nějaký fešný obrázek zde...
<p>R-2R is basically a voltage divider created from resistors of 2 values. The output of this DAC is given by the following equation</p>
nějaká pěkná rovnice zde

<h3> 4x3 keypad matrix </h3>
<p>Key pad matrix is an array of buttons, where each button is connected to the 2 GPIOs, as can be seen at fig. below. Connecting buttons in this pattern reduces the number of used GPIOs. With this method, we can use only 7 (3 rows x 4 columns) pins to control 12 buttons. If we want to get the position of the pressed button, we need to set all columns as inputs with the internal pullup resistors and rows as output with HIGH logic level. The next step is to loop through all rows in the following pattern: ROW(i) = LOW, check all columns, if the column is LOW, then the button has position row(i) x column(LOW). If none of the columns is LOW set ROW(i) back to HIGH and go to the next row.</p>
schéma zapojení

<h2> Code description </h2>
<h3> User Interface </h3>
For purpose of controlling the 4x3 matrix, we have written a library called "myMatrix". This library contain these functions: 

| **function** | **input** | **output** |**Description**|
| :-: | :-: | :-: | :-- | 
| `initMatrix()`  | none | none | set columns as inputs with pullup resistors, and rows as output HIGH  |
| `scanMatrix()`   | none | uint8_t pos |returns position in form of integer of pushed button EX: 23 -> row 2 x column 3; returns 0 if nothing was pressed|
| `posToConstChar(uint8_t pos, bool shift)` |  `uint8_t pos` `bool shift` | `const char* button_name` |returns button name according to the position and also takes into account if shift button was pressed |

To scan matrix. function scanMatrix() is called in regular interval given by Timer1 overflow each 33ms. The display, UART and parameters like frequency, output status and waveform are then updated according to the pushed button.
