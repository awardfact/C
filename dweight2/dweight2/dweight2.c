#include <stdio.h>

int main(void) {

	int height, lenfth, width, volume, weight;

	printf("input height\n");
	scanf_s("%d", &height);

	printf("input lenfth\n");
	scanf_s("%d", &lenfth);

	printf("input width\n");
	scanf_s("%d", &width);

	volume = height * width * lenfth;
	weight = (volume + 165) / 166;

	printf("volume (cubic inches) : %d\n ", volume);
	printf("dimensional weight (pounds) : %d\n", weight);

	return 0;

}