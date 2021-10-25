#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define LENGTH 3

int main(int argc, char **argv) {
	int spi_fd = 0;
	int mode = SPI_MODE_0;
	int result;
	int value;
	struct spi_ioc_transfer spi;
	unsigned char data_out[LENGTH] = {0x1,0x90,0x0};
	unsigned char data_in[LENGTH];
	double Vin;
 	
	ioctl(spi_fd,SPI_IOC_WR_MODE,&mode);
	memset(&spi,0,sizeof(struct spi_ioc_transfer));
	
	spi . tx_buf = (unsigned long)&data_out;
	spi . rx_buf = (unsigned long)&data_in;
	spi . len = LENGTH;
	spi . delay_usecs = 0;
	spi . speed_hz = 100000;
	spi . bits_per_word = 8;
	spi . cs_change = 0;

	/* Open SPI device */
	spi_fd = open("/dev/spidev0.0",O_RDWR);

	/* Set SPI Mode_0 */
	result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
	if(result < 0) printf("Error\n");
	/* Loop forever printing the CH0 and CH1 Voltages 	*/
	/* Once per second.					*/

	while(1){
	data_out[1] = 0x10; //setting what channel i want to read
	ioctl(spi_fd,SPI_IOC_MESSAGE(1),&spi);// storing the data into data_in
	value = (data_in[1] << 8) | data_in[2];
	printf("First byte: %d\n", data_in[1]);
	printf("Second byte: %d\n", data_in[2]);
	Vin = (value * 3.3) / 1024;
	printf("Vin: %f\n",Vin);


	data_out[1] = 0x00;
	ioctl(spi_fd,SPI_IOC_MESSAGE(1),&spi); //reads the pins and stores into data_in
	value = (data_in[1] << 8) | data_in[2]; // shifting the values i want to read into value 
	printf("First byte: %d\n", data_in[1]); //checking what my data reads	
	printf("Second byte: %d\n", data_in[2]);
	Vin = (value * 3.3) / 1024; // calculating voltage
	printf("Vin: %f\n",Vin);
	usleep(1000000);
	

	}

	/* Use the SPI_IOC_MESSAGE(1) ioctl() as described in the class notes */

	return 0;
}
