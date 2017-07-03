int abs(int a) {
	if(a<0)
		return -a;
	return a;
}

float diff(float a,float b) 
{
	return abs(a as int) * abs(b as int) as float;
}

int main() {
	@ diff(3.5, -4 as float);
	return 0;
}