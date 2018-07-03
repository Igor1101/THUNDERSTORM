/* 
 * Public Domain 
 * https://littleosbook.github.io/#the-serial-ports
 * https://en.wikibooks.org/wiki/Serial_Programming/8250_UART_Programming
 * */
#include <stdint.h>
/**
 * All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */
#define SERIAL_COM2_BASE                0x2F8      /* COM1 base port */
#define SERIAL_COM3_BASE                0x3E8      /* COM1 base port */
#define SERIAL_COM4_BASE                0x2E8      /* COM1 base port */
#define SERIAL_MAIN SERIAL_COM1_BASE                
#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */
/**
 * SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

void serial_disable_ints(uint16_t com);   
/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(uint16_t com, uint16_t divisor);
/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */                                                                            
void serial_configure_line(uint16_t com);
/** serial_configure_fifo:
 * enable, clear, 14 bytes treshold
 * @param com The serial port to configure 
 */
void serial_configure_fifo(uint16_t com);
/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(uint16_t com);
/**serial_write_asyn:
 * @param com com to write
 * @param ch char we write
 */
void serial_write_asyn(uint16_t com, char ch) ;

/**serial_received:
 * @param com com to receive from 
 * @return 0 if no char received
 *         1 if char received
 */
int serial_received(uint16_t com) ;

/**serial_read_asyn:
 * wait for characters to receive
 * @param com com to receive from 
 * @return char
 */
char serial_read_asyn(uint16_t com);
