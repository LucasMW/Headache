byte sqrt(byte x){
	byte y,r;
	y = 0;
	while(x > y*y){
		y++;
	}
	return y;
}
void main(){
    byte x;
    @"Enter a number: ";
    read x;
    @"sqrt("; @x; @") = "; @sqrt(x); @"\n";
}