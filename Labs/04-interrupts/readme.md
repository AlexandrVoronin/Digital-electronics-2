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

| **Module** | **Operation** | **I/O register(s)** | **Bit(s)** |
| :-: | :-- | :-: | :-- |
| Timer/Counter0 | Prescaler<br><br>8-bit data value<br>Overflow interrupt enable |  TCCR0B<br>TCNT0<br>TIMSK0<br> | CS02, CS01, CS00<br>(000: stopped, 001: 1, 010: 8, 011: 64, 100: 256, 101: 1024)<br>TCNT0[7:0]<br>TOIE0 (1: enable, 0: disable) |
| Timer/Counter1 | Prescaler<br><br>16-bit data value<br>Overflow interrupt enable | TCCR1B<br><br>TCNT1H, TCNT1L<br>TIMSK1 | CS12, CS11, CS10<br>(000: stopped, 001: 1, 010: 8, 011: 64, 100: 256, 101: 1024)<br>TCNT1[15:0]<br>TOIE1 (1: enable, 0: disable) |
| Timer/Counter2 | Prescaler<br><br>8-bit data value<br>Overflow interrupt enable |  TCCR2B<br>TCNT2<br>TIMSK2<br><br> | CS22, CS21, CS20<br>(000: stopped, 001: 1, 010: 8, 011: 32, 100: 64, 101: 128,110: 256,111:1024)<br>TCNT2[7:0]<br> TOIE2 (1: enable, 0: disable)|
