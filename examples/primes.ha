byte modulus(byte a, byte b){
	byte r,q;
	q = a/b;
	r = a - b*q;
	return r;
}
void main() {
    byte x,y;
    bit answer;
    read x;
    y = 2;
    answer = true;
    while(x > y){
       if(modulus(x,y)) {
       } else{  
           answer = false;
           y = x + 1;
        }
       y++;
    }
    @"Is "; @x; @" prime?\n";
    @"Answer: "; @answer; @"\n";
}