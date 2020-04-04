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
	if(less(a as int,b as int)){
		@"less\n";
	} else {
		@"Not less\n";
	}
}