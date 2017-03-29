#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(){
	int count_a=0;	
	char snow_in_jan_data[3100];

	for(int counter=0;counter<3100;counter++){
		srand48(counter);
		float ran = drand48();
		
		if(ran<=0.1){
			count_a = count_a + 1;
			snow_in_jan_data[counter]='a';
		}else{
			snow_in_jan_data[counter]='b';
		}
	}

	printf("p(c = 'a') = %.4f",((float)count_a)/3100.0f);

	return 0;
}
