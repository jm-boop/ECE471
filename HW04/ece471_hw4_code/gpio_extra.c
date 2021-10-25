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
	int fd, inrv, outrv;
	/* Initialize two handles for GPIO17 input and GPIO18 output */
	struct gpiohandle_request inreq;
	struct gpiohandle_request outreq;
	struct gpiohandle_data indata, outdata; 

	/* Open the gpio device */
        fd = open("/dev/gpiochip0", O_RDWR);
        /* Print error and exit if function unsuccessfull */
        check_error(fd);
	
	/* Clear memory in structs just in case  */
	memset(&inreq, 0, sizeof(struct gpiohandle_request));
	memset(&outreq, 0, sizeof(struct gpiohandle_request));

	/* Set GPIO17 input and GPIO18 output */
	inreq.flags = GPIOHANDLE_REQUEST_INPUT;
	outreq.flags = GPIOHANDLE_REQUEST_OUTPUT;
	inreq.lines = 1;
	outreq.lines = 1;
	inreq.lineoffsets[0] = 17;
	outreq.lineoffsets[0] = 18;
	inreq.default_values[0] = 1;
	outreq.default_values[0] = 0;

	/* Store requested configs in a corresponding handles */
	inrv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &inreq);
	outrv = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &outreq);
	check_error(inrv);
	check_error(outrv);

        memset(&indata, 0, sizeof(indata));
	memset(&outdata, 0, sizeof(outdata));

	
	while (1) {
		/* Read from GPIO17 if switch pressed (wire connected), if so turn LED on from GPIO18, else turn LED off */
		inrv = ioctl(inreq.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &indata);
		
		if (indata.values[0]) {
			outdata.values[0] = 1;
			outrv = ioctl(outreq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &outdata);
			check_error(outrv);
		} else {
			outdata.values[0] = 0;
			outrv = ioctl(outreq.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &outdata);
			check_error(outrv);
		}
	}

	return 0;
}

void check_error(int handle) {
	if (handle < 0) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}
