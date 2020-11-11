   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** |
   | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0   |  |
   | Up     | 0.495&nbsp;V | 101 |  |
   | Down   | 1.203&nbsp;V | 246 |  |
   | Left   | 1.970&nbsp;V | 403 |  |
   | Select | 3.182&nbsp;V | 651 |  |
   | none   | 5.000&nbsp;V | 1023 |  |





   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `uart_init` | `UART_BAUD_SELECT(9600, F_CPU)` | Initialize UART to 8N1 and set baudrate to 9600&nbsp;Bd | `uart_init(UART_BAUD_SELECT(9600, F_CPU));` |
   | `uart_getc` | `none(void)`  |  Get received byte from ringbuffer &nbsp;Bd |`uart_getc()`| 
   | `uart_putc` | `unsigned char data`  | Put byte to ringbuffer for transmitting via UART. |`uart_putc(data)`|
   | `uart_puts` | `const char* s` | Put string to ringbuffer for transmitting via UART. |`uart_puts(s)`|
