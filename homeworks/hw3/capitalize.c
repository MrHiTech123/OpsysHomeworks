

#define quote "
#define stringify(thing) #thing

#define plusFive(x) ({return x + 5;})

int main(int argc, char const *argv[])
{
	printf("%s\n", stringify(thing) );
	printf("%d\n", plusFive(5));
	return 0;
}
