   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** |
   | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   | 0 |
   | Up     | 0.495&nbsp;V | 101 | 101 |
   | Down   | 1.203&nbsp;V | 246 | 245 |
   | Left   | 1.970&nbsp;V | 403 | 402 |
   | Select | 3.182&nbsp;V | 651 | 650 |
   | none   | 5.000&nbsp;V | 1023 | 1022 |

   | **Operation** | **Register(s)** | **Bit(s)** | **Description** |
   | :-: | :-- | :-- | :-- |
   | Voltage reference | ADMUX | REFS1:0 | 01: AVcc voltage reference, 5V |
   | Input channel | ADMUX | MUX3:0 | 0000: ADC0, 0001: ADC1, ... |
   | ADC enable | ADCSRA | ADEN | 0:ADC off 1: ADC on |
   | Start conversion | ADCSRA | ADSC  | 1: start each(single)/first(free running) conversion 0:conversion in progress|
   | ADC interrupt enable | ADCSRA | ADIE | Bit set after ADC conversion and updated Data Registers |
   | ADC clock prescaler | ADCSRA | ADPS2:0 | 000: Division factor 2, 001: 2, 010: 4, ...|
   | ADC result | ADCL and ADCH | ADC9:0 | Represents result of conversion |



   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `uart_init` | `UART_BAUD_SELECT(9600, F_CPU)` | Initialize UART to 8N1 and set baudrate to 9600&nbsp;Bd | `uart_init(UART_BAUD_SELECT(9600, F_CPU));` |
   | `uart_getc` | `none(void)`  |  Get received byte from ringbuffer &nbsp;Bd |`uart_getc()`| 
   | `uart_putc` | `unsigned char data`  | Put byte to ringbuffer for transmitting via UART. |`uart_putc(data)`|
   | `uart_puts` | `const char* s` | Put string to ringbuffer for transmitting via UART. |`uart_puts(s)`|
