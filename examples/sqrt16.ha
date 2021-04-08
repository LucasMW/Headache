short sqrt(short x){
	short y,r;
	y = 0;
	r = 0;
	while(x > r){
		y++;
		r = y*y;
	}
	if(x == r) {
		return y;
	} else {
		y--;
		return y; 
	}
}
void main(){
    short x;
    @"Enter a number: ";
    read x;
    @"sqrt("; @x; @") = "; @sqrt(x); @"\n";
}