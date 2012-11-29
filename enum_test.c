#include <stdio.h>

enum Money {PENNY=1, NICKEL=5};

int
func1()
{
	static int counter = 1;
	return ++counter;
}

static int y = 1;

void
main(int argc, char *argvs[])
{
	enum Money penny, nickel;
	penny = PENNY;
	nickel = NICKEL;
	printf("penny is %d\n", penny);
	penny = penny + PENNY;
	printf("penny+PENNY is %d\n", penny);
	printf("nickel is %d\n", nickel);
	nickel += PENNY;
	printf("nickel+PENNY is %d\n", nickel);

	long a = 0xffffffff;
	printf("a=%ld\n", a);
	unsigned short b = 1;
	b = a;
	printf("b=%u\n", b);

	double c = 0xffffffffff;
	printf("c=%f\n", c);
	float d = (float)(25/10);
	printf("d=%f\n", d);

	int answer = func1() - func1() * func1();
	printf("answer=%d\n", answer);

	if (argc == 2) {
		char *arg1 = argvs[1];
		printf("arg1=%s\n", arg1);
		int year = atoi(arg1);
		int leap_year = (year%4 == 0 && year%100 != 0 || year%400 == 0) ? 1 : 0;
		if (leap_year != 0)
			printf("%d is a leap year\n", year);
		else
			printf("%d is not a leap year\n", year);
	}

	int e = 20;
	if (1 <= e <= 10)
		printf("%d is in range.\n", e);
	else
		printf("%d is out of range.\n", e);
}
