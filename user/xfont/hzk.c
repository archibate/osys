#include <stdio.h>

int main(void)
{
	FILE *HZK = fopen("hzk16.fnt", "rb");
	unsigned char incode[3] = "我";
	unsigned char qh = incode[0] - 0xa0;
	unsigned char wh = incode[1] - 0xa0;
	unsigned int offset = (94 * (qh - 1) + (wh - 1)) * 32;
	unsigned char mat[16][2];
	printf("%s=[%02X:%02X]=(%d,%d)\n", incode, incode[0], incode[1], qh, wh);
	fseek(HZK, offset, SEEK_SET);
	fread(mat, sizeof(mat), 1, HZK);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 2; j++) {
			unsigned char w = mat[i][j];
			for (int k = 0; k < 8; k++) {
				if (w & 0x80)
					printf("#");
				else
					printf("-");
				w <<= 1;
			}
		}
		printf("\n");
	}
	printf("\n");
	return 0;
}
