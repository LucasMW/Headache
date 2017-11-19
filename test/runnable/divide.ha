void main(){
	byte a;
	byte b;
	byte c;
	byte d;
	a = (10*6+3)/7; /* 63/7 = 9 */
	b = (10*23-5*23)/23; /* 5*23/23 = 5 */
	c = 0/0+10/10; /* 0/0 converts to zero in headache */
	d = a/a + b/b + c/c;
	@"a: "; @a; @"\n";
	@"b: "; @b; @"\n";
	@"c: "; @c; @"\n";
	@"d: "; @d; @"\n";
}