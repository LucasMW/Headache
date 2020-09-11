byte routine(byte num, byte div){
	byte b,a;
	a = num;
	b = div;
	while(b){
		a--;
		b--;
	}
	return a;
}
byte modulus2(byte a, byte b){
	byte r,q;
	q = a/b;
	r = a - b*q;
	return r;
}
byte modulus(byte a, byte b){
	byte result,div,num;
	div = b;
	num = a;
	if(a<b) {
		return a;
	}
	while(num){
		while(div){
			num--;
			div--;
			@num; @" "; @div; @"\n";
		}
		if(num<b){
			return num;
		}
		div = b;
	}
	return result;
}
void main(){
	byte a;
	byte b;
	byte c;
	byte d;
	a = modulus2(2,5);
	@"a: "; @a; @"\n";
	b = modulus2(11,2);
	c = modulus2(5,3);
	d = modulus2(63,8);
	//d = modulus2(224,15);
	@"b: "; @b; @"\n";
	@"c: "; @c; @"\n";
	@"d: "; @d; @"\n";
}