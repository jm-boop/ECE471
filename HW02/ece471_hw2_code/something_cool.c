#include <stdio.h>

int main(int argc, char **argv) {
	
	int i;
	int j = 1;
	
	// Print each line a different color using ANSI escape codes and an iterator
	for (i = 0; i < 12; i++) {
		if (j == 8) {
			j = 1;
		}
		
		printf("\x1b[3%dm#%d: ECE471 This is line numero %d!\n", j, i + 1, i + 1);

		j++;
	}

	return 0;
}
