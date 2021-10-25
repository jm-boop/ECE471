#include <stdio.h>

int main(int argc, char **argv) {
	
	int i;
	
	// Iteratively print 12 lines to the terminal
	for (i = 0; i < 12; i++) {
		printf("#%d: ECE471 This is line numero %d!\n", i + 1, i + 1);
	}

	return 0;
}
