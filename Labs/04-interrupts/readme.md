<h1> Lab 4 </h1>
<h2> Preparation tasks </h2>

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16us | 128us | -- |1.024ms | -- | 4.096ms | 16.384ms |
| Timer/Counter1 | 16 | 4.096ms | 32.768ms | -- | 262.144ms | -- | 1.048576s | 4.194304s |
| Timer/Counter2 | 8  | 16us | 128us | 512us | 1.024ms | 2.048ms | 4.096ms | 16.384ms |


| **Program address** | **Source** | **Vector name** | **Description** |
| :-: | :-- | :-- | :-- |
| 0x0000 | RESET | -- | Reset of the system |
| 0x0002 | INT0  | External Interrupt 0 |
| 0x0004 | INT1 | INT1_vect | External Interrupt Request 1 |
| 0x0006 | PCINT0 | PCINT0_vect | Pin Change Interrupt Request 0 |
| 0x0008 | PCINT1 | PCINT1_vect | Pin Change Interrupt Request 1 |
| 0x000A | PCINT2 | PCINT2_vect | Pin Change Interrupt Request 2 |
| 0x0012 | TIMER2_OVF | TIMER2_OVF_vect | Timer/Counter2 Overflow |
| 0x001A | TIMER1_OVF | TIMER1_OVF_vect | Timer/Counter1 Overflow |
| 0x0020 | TIMER0_OVF | TIMER0_OVF_vect | Timer/Counter0 Overflow |
| 0x002A | ADC | ADC_vect | ADC Conversion Complete |
| 0x0030 | TWI | TWI_vect | 2-wire Serial Interface |
