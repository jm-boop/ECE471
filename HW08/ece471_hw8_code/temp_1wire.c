#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

#include "sensor_name.h"

// Function to check return values for errors
// NOTE: 'fopen()' returns NULL upon failure 
void check_error(FILE *handle) {
	if (!handle) {
		printf("%s\n", strerror(errno));
		exit(1);
	}
}

static double read_temp(char *filename) {

	double result=0.0;
	
	char string1[256], string2[256];
	FILE *fff;

	// Open w1 file, check for errors
	fff = fopen(SENSOR_NAME, "r");
	check_error(fff);

	// Read file 
	fscanf(fff, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", string1, string2);
	
	// Return error if value is "NO", otherwise return temperature
	if (!strcmp(string1, "YES")) {
	// Convert miliCelsius to Celsius
		result = atoi(&string2[2])*1E-3;	
	} else if (!strcmp(string1, "NO")) {
		printf("Error: invalid temperature value\n");
	}	
	
	// Close file pointer
	fclose(fff);

	return result;
}

int main(int argc, char **argv) {

	double temp1;

	while(1) {
		// Infinitely print temperature in Celsius
		temp1=read_temp(SENSOR_NAME);
		printf("%.2lfC\n",temp1);
		sleep(1);
	}

	return 0;
}


