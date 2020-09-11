
void main() {
	char c; byte b;
	@"Enter a character:\n";
	read c;
	@"Your character is "; @c; @"\n";
	b = c as byte;
	@"Here is your character in bytes:"; @b; @"\n";

}