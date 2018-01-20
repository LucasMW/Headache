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
void main(){
	short a,b;
	a=2;
	b=10;
	@"pow(";@a;@",";@b;@") = ";
	@pow(a,b);@"\n";
}