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

	/* Write 1s to all Display Columns to light entire display */
	buffer[0] = 0x0;
	for (i = 0; i < 16; i++) buffer[1 + i] = 0xff;
	write(fd, buffer, 17);

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
