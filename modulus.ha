
byte modulus(byte a, byte b){
	byte r,q;
	q = a/b;
	r = a - b*q;
	return r;
}

void main(){
	byte a;
	byte b;
	byte c;
	byte d;
	a = modulus(2,5);
	@"a: "; @a; @"\n";
	b = modulus(11,2);
	c = modulus(5,3);
	d = modulus(63,8);
	//d = modulus(224,15);
	@"b: "; @b; @"\n";
	@"c: "; @c; @"\n";
	@"d: "; @d; @"\n";
}