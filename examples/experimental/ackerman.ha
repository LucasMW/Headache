//Ackermann Function in Monga
//Copyright 21/12/16 
//Modified  21/12/16
//Lucas Menezes



void displayAck(int m,int n) 
{
    @"A(";@m;@",";@n;@")"; 
}

int ackermann(int m, int n)
{   int r;
		
        if (!m) { 
        	displayAck(m,n);@" = ";@n+1; @"\n";
        	return n + 1;
        }
        if (!n) { 
        	displayAck(m,n); @" = "; displayAck(m-1,1 as int); @"\n";
         return ackermann(m - 1, 1 as int);
        }
        displayAck(m,n);@" = ";@"A(";@m;@",A(";@m;@",";@n-1;@"))\n"; 
        r = ackermann(m - 1, ackermann(m, n - 1));
        return r;
}
 
void main()
{
	int m, n;
	int r;
	
	m = 1; n = 1;

    r = ackermann(m, n);
    @"Threfore:\n";
    displayAck(m,n);@" = ";@r;@"\n"; 
 
}