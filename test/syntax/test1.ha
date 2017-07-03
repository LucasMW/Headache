float ppt (float ar) 
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
	float result, x;
	int y;
	int [] vector;
	x = 5.0;
	y = 3;
	vector = new int[fat(10)];
	result = ppt(x);
	if (result > x) {
		y = 0;
	} else {
		y = 1;
	}
	string = new byte[10];
	return 0;
	
	
}
