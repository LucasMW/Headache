short sqrt(short x){
	short y,r;
	y = 0;
	while(x > y*y){
		y++;
	}
	return y;
}
void main(){
    short x;
    @"Enter a number: ";
    read x;
    @"sqrt("; @x; @") = "; @sqrt(x); @"\n";
}