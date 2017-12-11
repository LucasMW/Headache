
byte c,d,u;
void main(){
	@"Enter exactly 3 digits of a byte:\n";
	c-=48;
	d-=48;
	u-=48;
	@"Received:"; @(c*100+d*10+u) as byte; @"\n";
}