void main() {
	byte current;
	current = 99;
	while(current){
		@current;@" bottles of beer on the wall, "; @current; @" bottles of beer\n";
		current--;
		@"Take one down, pass it around, ";@current; @" bottles of beer on the wall...\n";
	}
	@"No more bottles of beer on the wall, no more bottles of beer.\n";
	@"Go to the store and buy some more, 99 bottles of beer on the wall.\n";
}