#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>

#include <linux/i2c-dev.h> // I2C_SLAVE

#include <errno.h>
#include <string.h>

void check_error(int);

int main(int argc, char **argv) {

	int fd;

	char i2c_device[]="/dev/i2c-1";

	unsigned char buffer[17];
	unsigned char display[] = {0x79, 0x39, 0x79, 0x00, 0x66, 0x07, 0x06, 0x00}; // Array to hold byte values of 'ECE 471 '
	int i;
	int result;

	/* Open i2c device and check for errors opening file.
	   Store to an integer file descriptor. */
	fd = open(i2c_device, O_RDWR);
	check_error(fd);

	/* Set slave address */
	result = ioctl(fd, I2C_SLAVE, 0x70);
	check_error(result);

	/* Turn on System oscillator (normal operation mode) */
	buffer[0] = (0x2<<4) | (0x1);
	result = write(fd, buffer, 1);
	check_error(result);

	/* Turn on LED Display and set to no blinking */
	buffer[0] = (0x8<<4) | (0x1);
	result = write(fd, buffer, 1);
	check_error(result);

	/* Set Brightness to full by using a 16/16 duty cycle */
	buffer[0] = (0xE<<4) | (0xF);
	result = write(fd, buffer, 1);
	check_error(result);
	
	/* Display 'ECE 471 ' on LED display, scrolling from right to left  */
	while (1) {	
		/* Iterate through each byte, write to display, then shift each
		   by one column  */		
		for (i = 0; i < 8; i++) {
			buffer[0] = 0x00;
			buffer[2] = 0x00;
			buffer[4] = 0x00;
			buffer[5] = 0x00;
			buffer[6] = 0x00;
			buffer[1] = display[i];
			if (i >= 5) {
				buffer[9] = display[i - 5];
			} else {
				buffer[9] = display[i + 3];
			}
			if (i >= 6) {
				buffer[7] = display[i - 6];
			} else{
				buffer[7] = display[i + 2];
			}
			if (i >= 7) {
				buffer[3] = display[i - 7];
			} else {
				buffer[3] = display[i + 1];
			}
			write(fd, buffer, 17);
			usleep(250000);
		}
	}

	/* Close device */
	close(fd);

	return 0;
}

/* Function to check return values for errors */
void check_error(int handle) {
	if (handle < 0) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}
