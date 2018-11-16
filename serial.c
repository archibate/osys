#include <serial.h>
#include <ioport.h>
#include <irq.h>


static
void delay
(void)
{
	io_inb(0x84);
	io_inb(0x84);
	io_inb(0x84);
	io_inb(0x84);
}


#define COM      0x3f8
#define COM_RX   (COM+0)
#define COM_TX   (COM+0)
#define COM_DLL  (COM+0)
#define COM_DLM  (COM+1)
#define COM_IER  (COM+1)
#define COM_IIR  (COM+2)
#define COM_FCR  (COM+2)
#define COM_LCR  (COM+3)
#define COM_MCR  (COM+4)
#define COM_LSR  (COM+5)

#define COM_IER_RDI     0x01
#define COM_LCR_DLAB    0x80
#define COM_LCR_WLEN8   0x03
#define COM_MCR_RTS     0x02
#define COM_MCR_DTR     0x01
#define COM_MCR_OUT2    0x08
#define COM_LSR_DATA    0x01
#define COM_LSR_TXRDY   0x20
#define COM_LSR_TSRE    0x40


/*static
int serial_proc_data
(void)
{
	if (!(io_inb(COM_LSR) & COM_LSR_DATA))
		return -1;
	return io_inb(COM_RX);
}*/



#define FAST_PUTC 1
void serial_putc
(char c)
{
#ifndef FAST_PUTC
	for (int i = 0;
			!(io_inb(COM_LSR) & COM_LSR_TXRDY) &&
			i < 12800;
			i++)
		delay();
#endif

	io_outb(COM_TX, c);
#ifndef FAST_PUTC
	io_outb(COM_FCR, 0x0f);
	io_outb(COM_FCR, 0x08);
#endif
}


void init_serial
(void)
{
	io_outb(COM_FCR, 0);
	io_outb(COM_LCR, COM_LCR_DLAB);

	unsigned short dlab = 115200 / 9600;
	io_outb(COM_DLL, dlab & 0xff);
	io_outb(COM_DLM, (dlab >> 8) & 0xff);

	io_outb(COM_LCR, COM_LCR_WLEN8 & ~COM_LCR_DLAB);
	io_outb(COM_MCR, 0);
	io_outb(COM_IER, COM_IER_RDI);

	int exists = (io_inb(COM_LSR) != 0xff);
	io_inb(COM_IIR);
	io_inb(COM_RX);

	if (exists)
		set_irq_enable(IRQ_SERIAL, 1);
}
