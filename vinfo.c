#include <txtdev.h>
#include <string.h>
#include <vinfo.h>

void init_vinfo(void)
{
	const VIDEO_INFO *vinfo = (const VIDEO_INFO *) 0x7b00;
	make_txtinfo_dev("vinfo", vinfo, sizeof(VIDEO_INFO));
}
