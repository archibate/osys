for x in *.c *.h; do
	echo processing $x...
	sed -i 's/#include "\(\w*\).h"/#include <\1.h>/' $x
done
