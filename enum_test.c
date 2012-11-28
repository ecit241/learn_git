#include <stdio.h>

enum Money {PENNY=1, NICKEL=5};

void
main()
{
	enum Money penny, nickel;
	penny = PENNY;
	nickel = NICKEL;
	printf("penny is %d\n", penny);
	printf("nickel is %d\n", nickel);
}
