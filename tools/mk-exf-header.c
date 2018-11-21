#include <stdio.h>
#include <stdlib.h>
#include "../kuinc/exf.h"

int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "usage: %s <output> <stack_pages> <code_size>\n", argv[0]);
		return 1;
	}

	EXF_HEADER exfhdr;
	exfhdr.x_magic	= EXF_MAGIC;
	exfhdr.x_stkpgs	= atoi(argv[2]);
	exfhdr.x_size	= atoi(argv[3]);
	exfhdr.x_off	= sizeof(exfhdr);

	FILE *fout = fopen(argv[1], "wb");
	fwrite(&exfhdr, sizeof(exfhdr), 1, fout);
	fclose(fout);

	return 0;
}
