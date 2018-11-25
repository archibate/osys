#include <stdio.h>

int main()
{
	int    i,j,k;   
	unsigned    char    incode[3]="中";
	unsigned    char    qh,wh;   
	unsigned    long    offset;     
	FILE    * HZK;   
	char    mat[16][2]; 
	char mat32[16];
	char mat64[16];
	qh    =    incode[0]    -    0xa0;
	wh    =    incode[1]    -    0xa0;   
	//qh=18;
	//wh=51;
	printf("%d %d\n",qh,wh);
	offset    =    (94*(qh-1)+(wh-1))*32;  

	if((HZK=fopen("HZK16","rb"))    ==    NULL)   
	{   
		printf("Can't    Open    hzk16\n");   
		return 0;   
	}   
	fseek(HZK,    offset,    SEEK_SET);   
	fread(mat,    32,    1,    HZK);
	fseek(HZK,    offset,    SEEK_SET);   
	fread(mat32,    16,    1,    HZK);   
	fread(mat64,    16,    1,    HZK);
	for(j=0;j<16;j++)
	{
		for(i=0;i<2;i++)
		{
			for(k=0;k<8;k++)
			{
				if(mat[j][i]&(0x80>>k))
				{
					printf("%c",'#');
				}else{  
					printf("%c",'-');
				}
			}
		}
		printf("\n");
	}
	printf("\n");
	for(i=0;i<16;i++)
	{
		for(k=0;k<8;k++)
		{
			if(mat32[i]&(0x80>>k))
			{
				printf("%c",'#');
			}else{  
				printf("%c",'-');
			}
		}
		if(i%2)
			printf("\n");
	}
	for(i=0;i<16;i++)
	{
		for(k=0;k<8;k++)
		{
			if(mat64[i]&(0x80>>k))
			{
				printf("%c",'#');
			}else{  
				printf("%c",'-');
			}
		}
		if(i%2)
			printf("\n");
	}

	fclose(HZK);

	return 0;
}
