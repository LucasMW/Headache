int pow32(int x, int y){
	int r,z;
	r=1;
	z = y;
	while(z){
		r = r*x;
		z--;
	}
	return r;
}
short pow(short x, short y){
	short r,z;
	r=1;
	z = y;
	while(z){
		r = r*x;
		z--;
	}
	return r;
}
byte pow8(byte x, byte y){
	byte r,z;
	r=1;
	z = y;
	while(z){
		r = r*x;
		z--;
	}
	return r;
}
short modulus(short a, short b){
	short r,q;
	q = a/b;
	r = a - b*q;
	return r;
}
byte modulus8(byte a, byte b){
	byte r,q;
	q = a/b;
	r = a - b*q;
	return r;
}
short sqrt16(short x){
	short y,r;
	y = 0;
	r = 0;
	while(x > r){
		y++;
		r = y*y;
	}
	if(x == r) {
		return y;
	} else {
		y--;
		return y; 
	}
}
byte sqrt(byte x){
	byte y,r;
	y = 0;
	r = 0;
	while(x > r){
		y++;
		r = y*y;
	}
	if(x == r) {
		return y;
	} else {
		y--;
		return y; 
	}
}
byte root8(byte x, byte grade){
    byte y,r;
	y = 0;
	while(x > pow8(y,grade)){
		y++;
	}
	return y;
}
short root(short x, short grade){
    short y,r;
	y = 0;
	while(x > pow(y,grade)){
		y++;
	}
	return y;
}
int less(int x, int y){
	return !(x > y || x == y);
}
int greaterEqual(int x, int y){
	return x > y || x == y;
}

void main() {
	byte a,b;
	print "Enter a and b\n";
	read a;
	read b;
	if(greaterEqual(a as int,b as int)){
		@"a >= b\n";
	} else {
		@"a < b\n";
	}
	%;
	if(less(a as int,b as int)){
		@"less\n";
	} else {
		@"Not less\n";
	}
	@sqrt(a); @"\n";
	@sqrt(b); @"\n";
}