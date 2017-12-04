void main(){
	byte i,j;
	byte x,y,r;
	i = 2;
	r = 0;
	while(i){
		j=2;
		while(j){
			//@"y ";
			j--;
			r++;
		}
		i--;
		//@"x\n";
	}
	@"Result is "; @r; @"\n";
}