byte add(byte a, byte b){
	return (a+b) as byte;
}
byte sub(byte a, byte b){
	return (a-b) as byte;
}
byte mul(byte a, byte b){
	return (a*b) as byte;
}
byte div(byte a, byte b){
	return (a/b) as byte;
}
void println(byte a){
	@a;@"\n";
}
void main(){
	println(add(10 as byte,2 as byte));
	println(sub(10 as byte,2 as byte));
	println(mul(10 as byte,2 as byte));
	println(div(10 as byte,2 as byte));
	
}