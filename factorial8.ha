 byte factorial(byte x){
 	byte y,z;
 	y = x;
 	z = 1;
 	while(y) {
 		z = y * z;
 		y--;
 	}
 	return z;
 }
 
void main(){
	byte x;
	@"Calculate x!\n";
	@"Enter x:\n";
	read x;	
	@x; @"! is "; @factorial(x); @"\n";
}