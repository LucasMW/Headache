byte bitsInByte(){
	return 8 as byte;
}
byte bitsInWinXP(){
	byte a;
	a = 2*2*2*2*2;
	return a;
}
void main(){
	byte a;
	byte b;
	a = bitsInByte() + bitsInByte();
	//%;
	b = bitsInWinXP() + bitsInWinXP();
	//%;
	@a;@"\n";
	@b;@"\n";
}