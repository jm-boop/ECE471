#include <stdio.h>

int print_all_chars_in_string(char *array) {

	char ch;
	int i=0;

	while(1) {
		ch=array[i];
		if (ch==0) break;
		printf("%c",ch);
		i++;
	}
	return i;
}


int main(int argc, char **argv) {


	print_all_chars_in_string("Hello!\n");

	print_all_chars_in_string(NULL);

	return 0;
}
