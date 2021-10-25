#include <stdio.h>

#include <fcntl.h>	/* open() */
#include <unistd.h>	/* close() */
#include <stdlib.h>     /* exit() */
#include <string.h>
#include <errno.h>

#include "linux/gpio.h"
#include "sys/ioctl.h"

void check_error(int);

int main(int argc, char **argv) {
	/* 'File Descriptor' and 'Return Value' */
	int fd, rv;
	struct gpiohandle_request req;
	struct gpiohandle_data data; 

	/* Open the gpio device */
        fd = open("/dev/gpiochip0", O_RDWR);
        /* Print error and exit if function unsuccessfull */
        check_error(fd);
	
	/* Clear memory in struct just in case  */
	memset(&req, 0, sizeof(struct gpiohandle_request));

	/* Want GPIO18 to be output */
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	/* Only want GPIO18 set */
	req.lines = 1;
	req.lineoffsets[0] = 18;
	req.default_values[0] = 0;
	/* Set an awesome label */
	strcpy(req.consumer_label, "ECE471 Blinkin' LEDs!!!");

	/* Store requested config. in a handle */
	rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	check_error(rv);
	
	printf("%s\n", req.consumer_label);
	
	while (1) {
		/* Turn LED on and wait 0.5 seconds */
		data.values[0] = 1;
		rv = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		check_error(rv);
		printf("%d\n", data.values[0]);
		usleep(500000);

		/* Turn LED off and wait 0.5 seconds */
		data.values[0] = 0;
		rv = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		check_error(rv);
		printf("%d\n", data.values[0]);
		usleep(500000);
	}

	return 0;
}

void check_error(int handle) {
	if (handle < 0) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}
