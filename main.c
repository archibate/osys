#define INIT(x) extern void init_##x(void); init_##x();
int main
(void)
{
	INIT(idt);
	INIT(idtfill);
	INIT(pic);
	INIT(kbc);
	INIT(game);
}



struct VBUF_CTL
{
	unsigned char *buf;
	unsigned short xsiz;
	unsigned short ysiz;
} __attribute__((packed));


void init_game
(void)
{
	struct VBUF_CTL *vbuf = (struct VBUF_CTL *) 0x7c00;

	for (int i = 0; i < vbuf->xsiz * vbuf->ysiz; i++)
		vbuf->buf[i] = (i % vbuf->xsiz) % 0x10;
}


