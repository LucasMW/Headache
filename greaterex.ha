
void printAnswer(byte a,byte b){
	@a; @" is greater than "; @b; @"!\n";
}

void main() {
	byte n1,n2;
	@"Which is greater?\n";
	@"Enter the first number\n";
	#n1;
	@"Enter the next number\n";
	#n2;
	if(n1>n2){
		printAnswer(n1,n2);
	} else if(n1 == n2) {
		@"They are equal!\n";
	} else {
		printAnswer(n2,n1);
	}

}