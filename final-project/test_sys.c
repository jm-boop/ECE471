#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
 
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "linux/gpio.h"

#define LENGTH 3 // transmit and receive 3 bytes of data

void check_error(int);

int main(int argc, char **argv) {
	// GPIO init
	int gpio_fd, i;
	struct gpiohandle_request gpio_req;
	struct gpiohandle_data gpio_data;

	// SPI init
        int spi_fd, mode, result;
	float val, last;
	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x01, 0x00, 0x00};
	unsigned char data_in[LENGTH];

	// Open the gpio device
	gpio_fd = open("/dev/gpiochip0", O_RDWR);
	check_error(gpio_fd);

	// Open SPI bus 0 device 0
	spi_fd = open("/dev/spidev0.0", O_RDWR);
 	check_error(spi_fd);

	// Set SPI Mode_0 
	mode = SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	check_error(result);

	// Safely fill struct and data_in memory with constant 0
	memset(&spi, 0, sizeof(struct spi_ioc_transfer));
	memset(&data_in, 0, sizeof(unsigned char)*LENGTH);

	memset(&gpio_req, 0, sizeof(struct gpiohandle_request));

	// Want GPIO18 to be output
	gpio_req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	// Only want GPIO18 set
	gpio_req.lines = 1;
	gpio_req.lineoffsets[0] = 18;
	gpio_req.default_values[0] = 0;

	// Initialize full-duplex transfer of 3 bytes
	spi.tx_buf = (unsigned long)&data_out;
	spi.rx_buf = (unsigned long)&data_in;
	spi.len = LENGTH;
	spi.delay_usecs = 0;
	spi.speed_hz = 100000;
	spi.bits_per_word = 8;
	spi.cs_change = 0;

	// Store requested config. in a handle 
	result = ioctl(gpio_fd, GPIO_GET_LINEHANDLE_IOCTL, &gpio_req);
	check_error(result);
 
	/* Loop forever printing the CH0 and CH1 Voltages,                      
	   once per second,using the SPI_IOC_MESSAGE(1) 
           ioctl() as described in the class notes        */
	last = 2.0;
	while(1) {
		/*// Single-ended input mode, CH0
		data_out[1] = 0x80;
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		check_error(result);
		// Grab least most significant 10 bits of data 
		val = ((data_in[1] <<  8) | data_in[2])*5.0/1024;
		printf("CH0: %fV\n", val);*/

		// Single-ended input mode, CH1 
		data_out[1] = 0x90;
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		check_error(result);
		// Grab least most significant 10 bits of data 
		val = ((data_in[1] <<  8) | data_in[2])*5.0/1024;

		printf("CH1: %fV", val);
		if (val <= 1.7) {
			
			printf("\nIntruder Alert!\n");
			if (last >= 1.7) {
				printf("Email Home Owner\n\n");
				system("./email_alert.py");
			}			
			for (i = 0; i < 500; i++) {
				gpio_data.values[0] = 1;
				result = ioctl(gpio_req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &gpio_data);
				check_error(result);
				usleep(500);
				gpio_data.values[0] = 0;
				result = ioctl(gpio_req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &gpio_data);
				check_error(result);
				usleep(500);
			}
		
		} else {
			if (last <= 1.7) {
				printf("\nDisarmed\n\n");
				system("./email_disarm.py");
			}
			else printf("\n\n");
		}
		usleep(50000);	
		last = val;
	}
	return 0;
}

/* Function to check return values for errors */
void check_error(int handle) {
	if (handle < 0) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}
