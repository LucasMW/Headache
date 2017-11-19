void main(){
	byte i;
	byte j;
	byte x;
	byte y;
	byte r;
	i = 2;
	r = 0;
	while(i){
		j=i;
		while(j){
			//@"y ";
			j=j-1;
			r = r + 1;
		}
		i = i-1;
		//@"x\n";
	}
	@"Result is "; @r; @"\n";
}