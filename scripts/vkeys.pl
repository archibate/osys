open O, '>vkeys.h';

print O <<EOF
#pragma once

#define IS_VKEY(i) ((i)<0)
#define _VKEY_OF(i) (-i)
EOF
;

$i = 0;
for $v (split /\s/, `cat vkeys.txt`) {
	print O "#define VK_$v _VKEY_OF($i)\n";
	$i++;
}
