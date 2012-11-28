#include <stdio.h>

void
print_size(const char *type, long unsigned size)
{
	char *bytes = "Bytes";
	if (size <= 1) {
		bytes = "Byte";
	}
	printf("Size of %s: %lu %s\n", type, size, bytes);
}

void main()
{
	print_size("int", sizeof(int));
	print_size("char", sizeof(char));
	print_size("short int", sizeof(short));
	print_size("long int", sizeof(long));
	print_size("float", sizeof(float));
	print_size("double", sizeof(double));
}
