#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void save(const char * filename, const char * mode, BYTE * data, int size)
{
	FILE* fp = fopen(filename, mode);

	if (!fp) return;

	fwrite(data, sizeof(BYTE), size, fp);

	fclose(fp);
}

void printProgress(double value)
{
	int size = 32;
	char bar[size + 1];
	bar[size] = '\0';

	for (int i = 0; i < size; i++)
		bar[i] = ((i / (double) size) < value) ? '#' : ' '; 

	printf("\r [%s] %d%%", bar, (int) (100 * value));
	fflush(stdout);
}
