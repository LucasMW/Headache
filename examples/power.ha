/*
23/01/2018
Author: Lucas Menezes
Interactive 16 bit power program
*/
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
	byte a,b;
	@"Calculate pow(a,b)\n";
	@"Enter a:\n";
	#a;
	@"Enter b:\n";
	#b;
	@"pow(";@a;@",";@b;@") = ";
	@pow(a as short,b as short);@"\n";
}