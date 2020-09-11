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
byte root8(byte x, byte grade){
    byte y,r,z;
	y = 0;
    z = 0;
	while(x > z){
        z = pow8(y,grade);
        @z;@"\n";
		y++;
	}
	return y;
}
void main(){
    byte x;
    @"Enter a number: ";
    read x;
    @"cubic root ("; @x; @") = "; @root8(x,2); @"\n";
}