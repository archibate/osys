#include <div.h>


div_t div(unsigned int a, unsigned int b)
{
	div_t d = { a / b, a % b };
	return d;
}


ldiv_t ldiv(unsigned long a, unsigned long b)
{
	ldiv_t d = { a / b, a % b };
	return d;
}
