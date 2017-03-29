#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(){
	int count_a=0;	
	float simulation_info[10000];
	int simulation_result[5];
	
	for(int s=0; s < 10000 ; s++){
		count_a = 0;

		for(int counter=103100;counter<106200;counter++){
			float ran = ((double)rand() * (1.0 - 0.0)) / (double)RAND_MAX + 0.0;
			
			if(ran<=0.1){
				count_a = count_a + 1;
			}
		}
		
		simulation_info[s]= (((float)count_a)/3100.0f);
	}

	for(int i=0; i < 10000 ; i++){
		float sim_data = simulation_info[i];
		if(sim_data<0.08){
			simulation_result[0]=simulation_result[0]+1;
		}
		if(sim_data<0.09){
			simulation_result[1]=simulation_result[1]+1;
		}
		if(sim_data>=0.09 && sim_data<=0.11){
			simulation_result[2]=simulation_result[2]+1;
		}
		if(sim_data>0.11){
			simulation_result[3]=simulation_result[3]+1;
		}
		if(sim_data>0.12){
			simulation_result[4]=simulation_result[4]+1;
		}

	}

	printf("In %d of the simulations p(c = 'a') < 0.08.\n",simulation_result[0]);
	printf("In %d of the simulations p(c = 'a') < 0.09.\n",simulation_result[1]);
	printf("In %d of the simulations p(c = 'a') is in the interval [0.09, 0.11].\n",simulation_result[2]);
	printf("In %d of the simulations p(c = 'a') > 0.11.\n",simulation_result[3]);
	printf("In %d of the simulations p(c = 'a') > 0.12.\n",simulation_result[4]);

	return 0;
}
