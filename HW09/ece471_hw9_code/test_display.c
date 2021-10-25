#include <stdio.h>
#include <stdlib.h>	/* atof() */

#include "write_display.h"

int main(int argc, char **argv) {

	double temperature=42.0;
	int fd;

	if (argc>1) {
		temperature=atof(argv[1]);
	}

	/* Initialize display */
	fd=init_display();

	/* Write out testing value */
	printf("Testing temperature with %.4lf\n",temperature);
	write_display(fd,temperature);

	/* Shutdown display when done */
	shutdown_display(fd);

	return 0;
}

