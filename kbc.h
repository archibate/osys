#pragma once


#define KB_PORT_DATA  0x60
#define KB_PORT_STATE 0x64
#define KB_PORT_CMD   0x64

#define KB_STATE_NOT_READY  0x02
#define KB_CMD_SET_MODE     0x60
#define KB_MODE_CONTROLLER  0x47
#define KB_CMD_TO_MOUSE     0xd4
#define KB_MOUSE_CMD_ENABLE 0xf4


extern
void kbc_cmd
	( unsigned char cmd
	, unsigned char data
	);
