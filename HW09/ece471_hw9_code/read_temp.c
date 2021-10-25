#include <stdio.h>
#include <stdlib.h> /* exit(1), atoi() */
#include <string.h>
#include <sys/time.h>
#include <errno.h> /* strerror */

#include "read_temp.h"

double read_temp() {

	double result=0.0;
	
	char string1[256], string2[256];
	FILE *fff;

	// Open w1 file, check for errors
	fff = fopen(SENSOR_NAME, "r");
	if (fff == NULL) {
		printf("%s\n", strerror(errno));
		exit(1);
	}

	// Read file 
	fscanf(fff, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", string1, string2);
	
	// Return -2000 degrees  if value is "NO", otherwise return actual temperature
	if (!strcmp(string1, "YES")) {
		// Convert miliCelsius to Celsius
		result = atoi(&string2[2])*1E-3;		

	} else if (!strcmp(string1, "NO")) {
		fclose(fff);
		return -1010.0;
	}	
	
	// Close file
	fclose(fff);

	return result;
}
