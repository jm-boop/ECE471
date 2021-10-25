#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
 
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3 // transmit and receive 3 bytes of data

void check_error(int);

int main(int argc, char **argv) {

        int spi_fd, mode, result, val;
	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x01, 0x00, 0x00};
	unsigned char data_in[LENGTH];
 
	/* Open SPI bus 0 device 0 */
	spi_fd = open("/dev/spidev0.0", O_RDWR);
 	check_error(spi_fd);

	/* Set SPI Mode_0 */
	mode = SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
	check_error(result);

	/* Safely fill struct and data_in memory with constant 0 */
	memset(&spi, 0, sizeof(struct spi_ioc_transfer));
	memset(&data_in, 0, sizeof(unsigned char)*LENGTH);

	/* Initialize full-duplex transfer of 3 bytes */
	spi.tx_buf = (unsigned long)&data_out;
	spi.rx_buf = (unsigned long)&data_in;
	spi.len = LENGTH;
	spi.delay_usecs = 0;
	spi.speed_hz = 100000;
	spi.bits_per_word = 8;
	spi.cs_change = 0;
 
	/* Loop forever printing the CH0 and CH1 Voltages,                      
	   once per second,using the SPI_IOC_MESSAGE(1) 
           ioctl() as described in the class notes        */
	while(1) {
		/* Single-ended input mode, CH0 */
		data_out[1] = 0x80;
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		check_error(result);
		/* Grab least most significant 10 bits of data */
		val = (data_in[1] <<  8) | data_in[2];
		printf("CH0: %fV\n", val * 3.3/1024);

		/* Single-ended input mode, CH1 */
		data_out[1] = 0x90;
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		check_error(result);
		/* Grab least most significant 10 bits of data */
		val = (data_in[1] <<  8) | data_in[2];
		printf("CH1: %fV\n\n", val * 3.3/1024);

		usleep(1000000);
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
