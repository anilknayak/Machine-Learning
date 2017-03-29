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
		//printf("1");
		cols = 0;
		char *token = strtok(data," \t");
		while(token){	
			double temp = (double) strtof(token,NULL);
			//printf("%f",temp);
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
	
	//Summing up the data for each dimensions 
	for(int row=0;row<rows;row++){
		int clas = (int) arr[row][no_inp_col];
		//Only 2 dimension being used for the 2d Gaussian 
		for(int col=0;col<2;col++){
			conf_mat[clas][col] = conf_mat[clas][col] + arr[row][col];
		}
		class_row_count[clas] = class_row_count[clas] + 1;
	}

	//calculate Mean
	for(int m=0;m<no_cls;m++){
		for(int col=0;col<2;col++){
			conf_mat[m][col] = conf_mat[m][col] / (float) class_row_count[m];
		}
	}

	double sigma[2][2];
	
	for(int r=0;r<2;r++){
			for(int c=0;c<2;c++){
				sigma[r][c] = 0.0f;
			}
	}
	
	for(int clas=0;clas<no_cls;clas++){
		
		if(class[clas]!=-9999.0f){
			
			for(int r=0;r<2;r++){
				for(int c=0;c<2;c++){
					double sigma_cell = 0.0f;
					for(int d=0;d<rows;d++){
						int cl = (int) arr[d][no_inp_col];
						if(cl == clas)
							sigma_cell = sigma_cell + ((arr[d][r]-conf_mat[clas][r])*(arr[d][c]-conf_mat[clas][c]));
					}
								
					sigma[r][c] = sigma_cell/(float)(class_row_count[clas]-1);
				}
			}
			printf("Class %d, mean = [%.2f, %.2f], sigma = [%.2f, %.2f, %.2f, %.2f]\n", 
					clas,conf_mat[clas][0],conf_mat[clas][1],sigma[0][0],sigma[0][1],sigma[1][0],sigma[1][1]);
					
		}
	}
		

	fclose(input_file);

	return 0;
}

