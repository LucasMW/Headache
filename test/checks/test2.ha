

int main (float ae){
	
	int var1, var2, var3,var4 ;
	
	var1 = 1;
	var2 = -3;
	var3 = var1 - var2 ; 

	/*float var4 = var1 * var3; monga does not have this*/
	var4 = -1;
	if(var3>0){

	   if(var2<var3){
	   var2 = var3;
	   }
	   else{
	   	var4 = 0;
	   	while(var4 < (2* var1) ){
	   		var4 = var4 + var1;
	   	} 
	   }

	   @ var4;
	}
	return 0;
}
