float ppt (float ar) 
{
	int a;
	int result;
	result = a*a*a/a+a;
	return result/result+result*result/result as float;
}


int fat(int n) 
{
	if (n == 0)
		return 1;
	else
		return n * fat(n-1);
}
		

int main() {
	byte[] string;
	float result, x;
	int y;
	int [] vector;
	x = 5.0;
	y = 3;
	vector = new int[fat(10)];
	result = ppt(x);
	if ((result as int) > (x as int)) {
		y = 0;
	} else {
		y = 1;
	}
	string = new byte[10];
	@fat(5);
	return 0;
	
	
}
