#define prepend(no, x) p ## x

int main(int argc, char const *argv[])
{
	int ppxn = 50;
	printf("%s\n", prepend(0, prepend(xn)));
	return 0;
}
