int abs(int a) {
	if(a<0)
		return -a;
	return a;
}

short diff(short a,short b) 
{
	return abs(a as int) * abs(b as int) as short;
}

int main() {
	@ diff(35 as short, -4 as short);
	return 0 as int;
}