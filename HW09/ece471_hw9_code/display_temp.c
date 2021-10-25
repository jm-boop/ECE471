#include <stdio.h>
#include <unistd.h>

#include "read_temp.h"
#include "write_display.h"

int main(int argc, char **argv) {

	double temp1;
	int fd;

	while(1) {
		// Infinitely print temperature in Celsius
		temp1 = read_temp();
		printf("%.2lfC\n",temp1);

		/* Initialize display */
		fd=init_display();

		/* Write out testing value */
		write_display(fd,temp1);

		/* Shutdown display when done */
		shutdown_display(fd);
		sleep(1);
	}
	return 0;
}

