#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int main(){
	char file_path[1000];
	FILE *input_file;
	char data[10000];
	
	
	printf("Provide the file path and name.\n");
	gets(file_path);

	input_file = fopen(file_path,"r");
	
	if(!input_file){
		printf("File does not exist in the directory");
		return 0;
	}
	 
	int rows = 0;
	int cols = 0;

	while(fgets(data,10000,input_file)!=NULL){
		char *token = strtok(data," \t");
		if(cols<=0){
			while(token){	
				cols = cols + 1;
				token = strtok(NULL," \t");
			}
		}
		token = NULL;
		rows = rows + 1;
	}
	
	double arr[rows][cols];
	for(int row=0;row<rows;row++){
		for(int col=0;col<cols;col++){
			arr[row][col] = 0.0f;
		}
	}
	
	input_file = fopen(file_path,"r");
	rows = 0;
	cols = 0;
	
	while(fgets(data,10000,input_file)!=NULL){
		cols = 0;
		char *token = strtok(data," \t");
		while(token){	
			double temp = (double) strtof(token,NULL);
			arr[rows][cols] = temp;
			token = strtok(NULL," \t");
			cols = cols + 1;
		}
		token = NULL;
		rows = rows + 1;
	}

	float class[10000];
	
	for(int u=0;u<10000;u++){
		class[u]=-9999.0f;
	}
	
	int no_cls = 0;
	int no_inp_col = cols-1;
	
	for(int i=0;i<rows;i++){
		int a = (int) arr[i][no_inp_col];
		if(class[a]==-9999.0f){
			class[a]=arr[i][no_inp_col];	
		}
	} 

	int max_class_index=0;

	for(int i=0;i<10000;i++){
		if(class[i]!=-9999.0f){
			no_cls = no_cls + 1;
			max_class_index = i;
		}
	}
	
	
	//Changing the number of class index becasue if  in a continuous class declaration if anythhing is missing
	//then it can handle the index
	//0 1 2 3 4 5 7 8 9 : 6 is missing so program should ignore the 6th index everywhere;
	no_cls = max_class_index+1;
	
	double var_mat[no_cls][no_inp_col];
	double std_mat[no_cls][no_inp_col];
	double conf_mat[no_cls][no_inp_col];
	int class_row_count[no_cls];
	
	for(int b=0;b<no_cls;b++){
		for(int c=0;c<no_inp_col;c++){
			var_mat[b][c] = 0.0f;
			std_mat[b][c] = 0.0f;
			conf_mat[b][c] = 0.0f;
		}
	}
	
	for(int b=0;b<no_cls;b++){
		class_row_count[b] = 0;
	}
	
	for(int row=0;row<rows;row++){
		int clas = (int) arr[row][no_inp_col];
		for(int col=0;col<no_inp_col;col++){
			conf_mat[clas][col] = conf_mat[clas][col] + arr[row][col];
		}
		class_row_count[clas] = class_row_count[clas] + 1;
	}

	//calculate Mean
	for(int m=0;m<no_cls;m++){
		for(int n=0;n<no_inp_col;n++){
			int row_count = class_row_count[m];
	
			if(row_count!=0 && conf_mat[m][n]!=0.0f){
				conf_mat[m][n] = conf_mat[m][n] / (float) row_count;
			}
		}
	}
	
	//Calculate Standard Deviation
	for(int u=0;u<rows;u++){
		for(int v=0;v<no_inp_col;v++){
			int clas1 = (int) arr[u][no_inp_col];
			var_mat[clas1][v] = var_mat[clas1][v] + pow(arr[u][v] - conf_mat[clas1][v],2);
		}
	}
	
	//Calculating the variance of the data given 
	for(int u=0;u<no_cls;u++){
		for(int v=0;v<no_inp_col;v++){
			int no_of_var_row = class_row_count[u]-1;
			//This to check whether the number of row available is 1
			//if 1 then 1/N-1 will throw error becasue divide by 0.
			//in that case the variance will be 0
			
			if(no_of_var_row!=0){
				var_mat[u][v] = var_mat[u][v]/(float) (no_of_var_row);
			}
			else{
				var_mat[u][v] = var_mat[u][v];
			}
		}
	}
	
	//Displaying the Mean and variance of the data being provided
	for(int u=0;u<no_cls;u++){
		for(int v=0;v<no_inp_col;v++){
			if(class[u]!=-9999.0f){
				printf("Class %d, dimension %d, mean = %.2f, variance = %.2f \n",u,v+1,conf_mat[u][v],var_mat[u][v]);
			}
		}
	}
	
	fclose(input_file);

	return 0;
}

