int main()  {
	int a,b,c;
	int i;
	a=3;
	b=2;
	c=1;
	i=0;
	while(i<a) {
		b = b +c;
		i= i +1;
	}
	if(b>a) {
		c=10;
		b=c/a;
	}
	if(i<c || i<0 || i < b && i>c || b && b || c ) {
		printf("asd");
		a = a + i*2;
	} else {
		printf("else");
		a = (a+i) *2;
	}
	return a;
}