byte pow(byte x, byte y){
	byte r,z;
	r=1;
	z = y;
	while(z){
		r = r*x;
		z--;
	}
	return r;
}
void main(){
	@pow(2 as byte,9 as byte);@"\n";
}