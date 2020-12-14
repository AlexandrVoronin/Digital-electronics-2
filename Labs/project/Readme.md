<h1> Project DE-2 </h1>
Authors: Alexandr Voronin, Richard Šebo
<h2> Project objectives </h2>
<p>Application of parking assistant using two HC-SR04 sensors. Output interface:LCD, frequency audio, led stripe</p>
<h2> Hardware description </h3>
<h3> HC-SR04 </h3>
<p>HC-SR04 is a distance measuring sensor. It has 4 pins:Vcc,Gnd, trigger, echo. To start its function it must first recieve pulse of at least 10us to the trigger pin. Sensor then transmits a sonic burst of eight pulses at 40 KHz. This pattern makes the signal unique, which allows it to differentiate between emited signal and ambient ultrasonic noise. The echo pin is set high and if the pulses are not reflected back it times out after 38ms and is set to low (no obstacle in range). If the pulses are reflected, echo is set to low when the signal is recieved producing pulse with width between 150 µS to 25 mS based on how long it took the signal to come back. Based on that and knowing the speed of light we can calculate the distance between the sensor and object that reflected the original pulses. In our program we let the MCU itterate as fast as it can as long as echo is HIGH, and then convert to distance by multiplying precalculated constant</p>
<h3> Atmega328p </h3>
<p>Application was brought to life using Atmega328p, 8-bit microchip controller. It has 21 GPIO pins accessed through registers PORTB,PORTC,PORTD. We are using external interrupt pins (PD3,PD2) for echo signals and I/O pins (PB3,PB2) for sending trigger signal to the sensor. I/0 ports PC1-5 are used for the led stripe output. I/0 pin PB4 is used for output to led replacing audio signaling by blinking in different frequencies. For LCD pins PD4-7 are used for data ports, pin PB0 asi RS pin and port PB1 as E pin. </p>




<h2> Code description </h2>
<h3> User Interface </h3>


| **function** | **input** | **output** |**Description**|
| :-: | :-: | :-: | :-- | 
| `  | | |   |
|   |  | ||
| |   |  ||


