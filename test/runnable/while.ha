void main(){
	byte a;
	a = 0;
	while(a){	
		@"Do not execute\n";
	}
	a = 1;
	while(a){
		a = a-1;
		@"execute one time\n";
	}
	a = 2;
	while(a){
		a = a-1;
		@"execute twice\n";
	}
	a = 3;
	while(a){
		a = a-1;
		@"execute three times\n";
	}
}