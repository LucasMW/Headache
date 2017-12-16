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
	byte a,b;
	a=2;
	b=9;
	@"pow(";@a;@",";@b;@") = ";
	@pow(a,b);@"\n";
}