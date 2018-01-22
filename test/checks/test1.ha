short ppt (short ar) 
{
	int a;
	int result;
	result = a*a*a/a+a;
	return result/result+result*result/result as short;
}


int fat(int n) 
{
	if (n == 0)
		return 1 as int;
	else
		return n * fat(n-1);
}
		

int main() {
	short result, x;
	int y;
	x = 500;
	y = 3;
	if ((result as int) > (x as int)) {
		y = 0;
	} else {
		y = 1;
	}
	@fat(5 as int);
	return 0 as int;
	
	
}
