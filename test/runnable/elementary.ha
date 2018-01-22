short add(short a, short b){
	return (a+b) as short;
}
short sub(short a, short b){
	return (a-b) as short;
}
short mul(short a, short b){
	return (a*b) as short;
}
short div(short a, short b){
	return (a/b) as short;
}
void println(short a){
	@a;@"\n";
}
void main(){
	short a,b;
	short temp;
	a = 1000;
	b = 20;
	temp = add(a,b);
	println(temp);
	temp = sub(a,b);
	println(temp);
	temp = mul(a,b);
	println(temp);
	temp = div(a,b);
	println(temp);
	
}