open O, '>vkeys.h';

print O <<EOF
#pragma once

#define _VKEY_OF(i) (-i)
EOF
;

$i = 0;
for $v (qw(F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12 CAPS SHIFT RSHIFT CTRL WIN ALT RALT MENU RCTRL INS HOME PGUP DEL END PGDN UP LEFT DOWN RIGHT)) {
	print O "#define VK_$v _VKEY_OF($i)\n";
	$i++;
}
