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

        int spi_fd, mode, result, val, i;
	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x01, 0x00, 0x00};
	unsigned char data_in[LENGTH];
	double max_temp, min_temp, deg_c, deg_f;
 
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
 
	/* Single-ended input mode, CH2 */
	data_out[1] = 0xA0;

	/* Monitor 10 seconds of temp data, calculate and print
           the max. and min. of the data                       */
	while(1) {
		/* Reference temp to compare subsequent 10 temps */
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		check_error(result);
		/* Grab least most significant 10 bits of data */
		val = (data_in[1] <<  8) | data_in[2];
		/* Calculate degrees C and F using voltage from CH2 */
		deg_c = 100*val*3.3/1024 - 50;
		deg_f = deg_c*9/5 + 32;
		printf("Reference temp:       ");
		printf("%f F\n", deg_f);
		max_temp = deg_f;
		min_temp = deg_f;	

		i = 0;
		
		/* Monitor and display min. and max. temp. (F) of last 10 seconds */
		do {
			result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
			check_error(result);
			val = (data_in[1] <<  8) | data_in[2];
			deg_c = 100*val*3.3/1024 - 50;
			deg_f = deg_c*9/5 + 32;
			printf("Temp @ %d seconds:     ", i + 1);
			printf("%f F\n", deg_f);			
			/* Calculate min and max temp */
			if (deg_f > max_temp) max_temp = deg_f;
			if (deg_f < min_temp) min_temp = deg_f;

			i++;
				
		usleep(1000000);
		} while (i < 10);
		printf("\nMAX: %f                MIN: %f\n\n\n", max_temp, min_temp);
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
