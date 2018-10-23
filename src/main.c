#include "dwt.h"

#include <stdio.h>

#define WIDTH 512

void read_stream(const char* file, int* buffer, int n) {
	FILE *fp = fopen(file, "rb");
	fread(buffer, sizeof(int), n, fp);
	fclose(fp);
}

void write_stream(const char* file, int* buffer, int n) {
	FILE *fp = fopen(file, "wb");
	fwrite(buffer, sizeof(int), n, fp);
	fclose(fp);
}

int main(int argc, char **argv) {
	if (argc < 4) {
		printf("Usage: dwt <n_resolutions> <input-file>.bin <output-file>.bin\n");
		return 1;
	}
	int n = WIDTH * WIDTH;
	int* codestream = malloc(n*sizeof(int));
	read_stream(argv[2], codestream, n);
	dwt_2d(codestream, atoi(argv[1]));
	write_stream(argv[3], codestream, n);
	printf("%s has been transformed and stored to %s\n", argv[1], argv[2]);
	return 0;
}
