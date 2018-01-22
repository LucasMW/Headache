short ppt (short ar) 
{
	int result;
	result = a*a*a/a+a;
	return result/result+result*result/result;
}

int fat(int n) 
{
	if (n == 0)
		return 1;
	else
		return n * fat(n-1);
}
		

int main() {
	short result, x;
	int y;
	x = 550;
	y = 3;
	result = ppt(x);
	if (result > x) {
		y = 0;
	} else {
		y = 1;
	}
	return 0;
	
	
}
