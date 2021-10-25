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
	int temp = 1;

	/* Open the gpio device */
        fd = open("/dev/gpiochip0", O_RDWR);
        /* Print error and exit if function unsuccessfull */
        check_error(fd);
	
	/* Clear memory in struct just in case  */
	memset(&req, 0, sizeof(struct gpiohandle_request));

	/* Want GPIO17 to be output */
	req.flags = GPIOHANDLE_REQUEST_INPUT;
	/* Only want GPIO17 set */
	req.lines = 1;
	req.lineoffsets[0] = 17;
	req.default_values[0] = 0;
	/* Set an awesome label */
	strcpy(req.consumer_label, "ECE471 Flippin' Switches!!!");

	/* Store requested config. in a handle */
	rv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
	check_error(rv);
	
	memset(&data, 0, sizeof(data));	 
	
	while (1) {
		/* Send request to read input value from GPIO17 */
		rv = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
		/* De-bounce by waiting 50msec once a change is detected to test again */
		if (temp != data.values[0]) {
			usleep(50000);
			rv = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
			check_error(rv);
			if (temp != data.values[0]) {
				if (temp) {
					printf("Switch OPEN (%d --> %d)\n", temp, data.values[0]);
				} else { 
					printf("Switch CLOSED (%d --> %d)\n", temp, data.values[0]);
				}
			}
		}	
		/* Initialize next input value for testing */
		temp = data.values[0];
	}

	return 0;
}

void check_error(int handle) {
	if (handle < 0) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}


