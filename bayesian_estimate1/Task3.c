#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double calculate_combination(int _count_a,int _count_b);
long int calculate_factorial(int n);
double calculate_likelihood(int _count_a,int _count_b,double _prob);

double calculate_combination(int _count_a,int _count_b){
	long int n_f = calculate_factorial(_count_a+_count_b);
	long int n_a = calculate_factorial(_count_a);
	long int n_b = calculate_factorial(_count_b);
	
	return ((double)n_f/((double)n_a*(double)n_b));
}

long int calculate_factorial(int n){
    if (n >= 1)
        return n*calculate_factorial(n-1);
    else
        return 1;
}

double calculate_likelihood(int _count_a,int _count_b,double _prob){
	double cnr =  calculate_combination(_count_a,_count_b);
	
	return cnr*pow(_prob,_count_a)*pow(1.0f-_prob,_count_b);
}



int main(int arg_c,char *input_cmd[]){
	//prior of the baysian probability 
	double _prob_getting_a[5] = {0.1,0.3,0.5,0.7,0.9};
	double _prob_prior_a[5] = {0.9,0.04,0.03,0.02,0.01};
	double _prob_posterior_a[5] = {0.0,0.0,0.0,0.0,0.0};
	double _prob_initial_prior_a[5] = {0.9,0.04,0.03,0.02,0.01};

	char *input = NULL;
	int _input_len = 0;
	int _count_a = 0;
	int _count_b = 0;
		
	if(arg_c==2){
		//In case of value entered
		input = input_cmd[1];
		_input_len = strlen(input);
		for(int i=0;i<_input_len;i++){
			if(input[i]=='a'){
				_count_a = _count_a + 1;
			}else if(input[i]=='b'){
				_count_b = _count_b + 1;
			}
		}
	}
			
	for(int i=0;i<5;i++){
		//likelihood will change for each occurrence of a and b
		double likelihood = calculate_likelihood(_count_a,_count_b,_prob_getting_a[i]);
		double prior  = _prob_prior_a[i]; 
		
		double sum = 0.0f;
		for(int j=0;j<5;j++){
			sum = sum + (calculate_likelihood(_count_a,_count_b,_prob_getting_a[j])  * _prob_prior_a[j]);	
		}

		double normalized_posterior = (likelihood * prior)/sum;

		_prob_posterior_a[i] = normalized_posterior;

		printf("p(m = %0.1f | data) = %.4f\n",_prob_getting_a[i],normalized_posterior);
	}
	
	double p_a = 0.0f;
	for(int i=0;i<5;i++){
		p_a = p_a + _prob_getting_a[i] * _prob_posterior_a[i];	
	}
	
	printf("p(c = 'a' | data) = %.4f",p_a);
	
	
	
	
	return 0;

}
