 short factorial(short x){
 	short y,z;
 	y = x;
 	z = 1;
 	while(y) {
 		z = y * z;
 		y--;
 	}
 	return z;
 }

void main(){
	short x;
	@"Calculate x!\n";
	@"Enter x:\n";
	read x;	
	@"x! is "; @factorial(x); @"\n";
}