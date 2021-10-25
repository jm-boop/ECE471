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
	
	while (1) {
		/* Write 'ECE' to first 3 display columns */
		buffer[0] = 0x00;
		buffer[1] = 0x79;
		buffer[2] = 0x00;
		buffer[3] = 0x39;
		buffer[4] = 0x00;
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0x79;
		for (i = 0; i < 8; i++) buffer[i + 8] = 0x00;
		write(fd, buffer, 17);
		
		/* Pause for 1 sec */
		usleep(1000000);

		/* Write '471' to first 3 display columns */
		buffer[0] = 0x00;
		buffer[1] = 0x66;
		buffer[2] = 0x00;
		buffer[3] = 0x07;
		buffer[4] = 0x00;
		buffer[5] = 0x00;
		buffer[6] = 0x00;
		buffer[7] = 0x06;
		for (i = 0; i < 8; i++) buffer[i + 8] = 0x00;
		write(fd, buffer, 17);

		/* Pause for 1 sec */
		usleep(1000000);
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
