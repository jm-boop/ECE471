#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <string.h>

#include "read_temp.h"

// Function that accepts integers 0-9 to return the appropiate decimal value for writing to LED display
int led_byte(int num) {
	int bt;

	switch(num) {
		case 0:	
			bt = 63;
			break;
		case 1:
			bt = 6;
			break;
		case 2:
			bt = 91;
			break;
		case 3:
			bt = 79;
			break;
		case 4:
			bt = 102;
			break;
		case 5:
			bt = 109;
			break;
		case 6:
			bt = 124;
			break;
		case 7:
			bt = 7;
			break;
		case 8:
			bt = 127;
			break;
		case 9:
			bt = 103;
			break;
		default:
			bt = 0;
	}

	return bt;
}

// Function to check return values for errors
void check_error(int handle) {
	if ((handle < 0)) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}

int init_display(void) {
	int fd, result;
	char i2c_device[] = "/dev/i2c-1";
	unsigned char buffer[17];

	/* Open i2c device and check for errors opening file.
	   Store to an integer file descriptor. */
	fd = open(i2c_device, O_RDWR);

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
	
	return fd;
}

int write_display(int fd, double value) {
	double deg_f;
	unsigned char buffer[17];
	int tens, ones, tenths, i;

	deg_f = value;
	// Extract tens place from temperature
	tens = abs(deg_f/10);
	// Extract ones place from temperature
	ones = abs((int)(deg_f)%10);
	// Extranct tenths place from temperature
	tenths = abs((int)(deg_f*10.0)%10);

	// Clear all bytes that don't display to LED display
	buffer[0] = 0x00;
	buffer[2] = 0x00;
	buffer[4] = 0x00;
	buffer[5] = 0x00;
	buffer[6] = 0x00;
	buffer[8] = 0x00;
	for (i = 0; i < 8; i++) buffer[i + 9] = 0x00;

	// 0.0 <= temp <= 99.9
	if ((value <= 99.9) && (value >= 0.0)) {  
		buffer[1] = (tens == 0) ? 0x00 : led_byte(tens); // Suppress leading zeros
		buffer[3] = led_byte(ones) | 0x80;
		buffer[7] = led_byte(tenths);
		buffer[9] = 0x63;
		write(fd, buffer, 17);
	// -99.9 <= temp < 0.0
	} else if ((value < 0.0) && (value >= -99.9)) {
		buffer[1] = 0x40;
		buffer[3] = (tens == 0) ? 0x00 : led_byte(tens); // Suppress leading zeros
		buffer[7] = led_byte(ones);
		buffer[9] = 0x63;
		write(fd, buffer, 17);	
	// 100 <= temp <= 999
	} else if ((value <= 999) && (value >= 100)) {
		buffer[1] = led_byte(tens/10);
		buffer[3] = led_byte(tens%10);
		buffer[7] = led_byte(ones);
		buffer[9] = 0x63;
		write(fd, buffer, 17);
	// Display "Err" if temp is invalid or won't fit on display
	} else {
		buffer[1] = 0x79;
		buffer[3] = 0x50;
		buffer[7] = 0x50;
		buffer[9] = 0x00;
		write(fd, buffer, 17);
	}	

	return 0;
}



int shutdown_display(int fd) {
	/* Close display's file descriptor */
	close(fd);

	return 0;
}
