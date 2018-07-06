/*
 * Public Domain
 */
#include <x86_64/cpu_management.h>
#include <asm/serial.h>
/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(uint16_t com, uint16_t divisor)
{
	outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
	outb(SERIAL_DATA_PORT(com), (divisor >> 8) & 0x00FF);	/* HIGH */
	outb(SERIAL_DATA_PORT(com), divisor & 0x00FF);	/* LOW */
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(uint16_t com)
{
	/* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
	 * Content: | d | b | prty  | s | dl  |
	 * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
	 */
	outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/** serial_configure_fifo:
 * enable, clear, 14 bytes treshold
 * @param com The serial port to configure 
 */
void serial_configure_fifo(uint16_t com)
{
	outb(SERIAL_FIFO_COMMAND_PORT(com), 0b11000111);
}

/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(uint16_t com)
{
	/* 0x20 = 0010 0000 */
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/**serial_write_asyn:
 * @param com com to write
 * @param ch char we write
 */
void serial_write_asyn(uint16_t com, char ch)
{
	while (serial_is_transmit_fifo_empty(com) == 0) ;
	outb(com, ch);
}

/**serial_received:
 * @param com com to receive from 
 * @return 0 if no char received
 *         1 if char received
 */
int serial_received(uint16_t com)
{
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 1;
}

/**serial_read_asyn:
 * wait for characters to receive
 * @param com com to receive from 
 * @return char
 */
char serial_read_asyn(uint16_t com)
{
	while (serial_received(com) == 0) ;
	return inb(com);
}

void serial_disable_ints(uint16_t com)
{
	outb(com + 1, 0x00);
}
