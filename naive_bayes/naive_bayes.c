#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double calculate_normal_distribution(double x,double mean,double std);

double calculate_normal_distribution(double x,double mean,double std){
	double pi = 3.1415926535897932384626;
	
	double part1 = (1/(std*sqrt(2*pi)));
		
	double part2 = 	exp(-((pow((x-mean),2))/(2*(pow(std,2)))));
	
	return part1*part2;
}

int main(int args,char *arg[]){
	FILE *training_file;
	FILE *training_file1;
	FILE *testing_file;
	FILE *testing_file1;
	
	int number_of_segments = 0;
		
	char *training_file_name = arg[1];
	char *testing_file_name = arg[2];
	char *classification_type = arg[3];
	
	if(args==5){
		number_of_segments = atoi(arg[4]);
	}
		
	int rows = 0;
	int cols = 0;
	int col;
	int attribute;
	int bin_c;
	int gaussian;
	int k;
	int y;
	int class_row;
	int stop;
	int cls;
	int i,j;
	double class[100];
	char data[10000];
	double default_variance = 0.0001f;
	double default_g = 0.0001f;
	double default_std = 0.01f;
	
	training_file = fopen(training_file_name,"r");
	while(fgets(data,10000,training_file)!=NULL){
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
	fclose(training_file);
	//free(training_file);

	int tr_rows = rows;
	int tr_cols = cols;
		
	double training_data_arr[tr_rows][tr_cols]; 
	int u;
	int v;
	for(u=0;u<tr_rows;u++){
		for(v=0;v<tr_cols;v++){
			training_data_arr[u][v]=0.0f;
		}
	}
	rows = 0;
	cols = 0;
	
	training_file1 = fopen(training_file_name,"r");
	while(fgets(data,10000,training_file1)!=NULL){
		cols = 0;
		char *token = strtok(data," \t");
		while(token){	
			double temp = atof(token);
			training_data_arr[rows][cols] = temp;
			
			token = strtok(NULL," \t");
			cols = cols + 1;
		}
		token = NULL;
		rows = rows + 1;
	}
	fclose(training_file1);
	//free(training_file1);
	
	double training_classes[100];
	int tr_no_inp_col = tr_cols-1;
	
	for(u=0;u<100;u++){
		training_classes[u]=-9999.0f;
	}
	
	for(i=0;i<tr_rows;i++){
		int a = (int) training_data_arr[i][tr_no_inp_col];
		if(training_classes[a]==-9999.0f){
			training_classes[a]=training_data_arr[i][tr_no_inp_col];	
		}
	}
	
	int a = 0;
	int max_class_index = 0;
	for(i=0;i<100;i++){
		if(training_classes[i]!=-9999.0f){
			a = a + 1;
			max_class_index = i;
		}
	}
	
	int training_no_of_cls = max_class_index+1;
	
	
	int class_row_count[training_no_of_cls];
	int b;
	for(b=0;b<training_no_of_cls;b++){
		class_row_count[b] = 0;
	}
	int row;
	for(row=0;row<tr_rows;row++){
		int cls = (int) training_data_arr[row][tr_no_inp_col];
		class_row_count[cls] = class_row_count[cls] + 1;
	}
	
	rows = 0;
	cols = 0;
	testing_file = fopen(testing_file_name,"r");
	while(fgets(data,10000,testing_file)!=NULL){
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
	
	int ts_rows = rows;
	int ts_cols = cols;
	fclose(testing_file);
	//free(testing_file);
	
	double testing_data_arr[ts_rows][ts_cols]; 
	for(u=0;u<ts_rows;u++){
		for(v=0;v<ts_cols;v++){
			testing_data_arr[u][v]=0.0f;
		}
	}
	
	//RUN
	testing_file1 = fopen(testing_file_name,"r");
	
	rows = 0;
	cols = 0;
	
	while(fgets(data,10000,testing_file1)!=NULL){
		cols = 0;
		char *token = strtok(data," \t");
		while(token){	
			double temp = atof(token);
			testing_data_arr[rows][cols] = temp;
			token = strtok(NULL," \t");
			cols = cols + 1;
		}
		token = NULL;
		rows = rows + 1;
	}
	
	fclose(testing_file1);
	//free(testing_file1);
		
	
	if(strcmp(classification_type,"gaussians")==0){
		//Start : P(Ck)
		double prior_prob_class[training_no_of_cls];
		for(i=0;i<training_no_of_cls;i++){
			prior_prob_class[i] = 0.0f;
		}
		for(i=0;i<training_no_of_cls;i++){
			if(training_classes[i]!=-9999.0f){
				prior_prob_class[i] = (float)class_row_count[i]/(float)tr_rows;
			}
			//printf("P(C%d) = %f\n",i,prior_prob_class[i] );
		}
		//End : P(Ck)
		
		//Calculating Mean / Std for training data set
		double sum[training_no_of_cls][tr_cols-1];
		double mean[training_no_of_cls][tr_cols-1];
		double std[training_no_of_cls][tr_cols-1];
		double sum_var[training_no_of_cls][tr_cols-1];
		
		for(i=0;i<training_no_of_cls;i++){
			for(j=0;j<tr_cols-1;j++){
				sum[i][j] = 0.0f;
				mean[i][j] = 0.0f;
				std[i][j] = 0.0f;
				sum_var[i][j] = 0.0f;
			}
		}
		
		
		//Starting Gaussian Probability calculation
		//training_no_of_cls
		for(cls = 0;cls < training_no_of_cls;cls++){
			int row_clas_count = class_row_count[cls];
			if(row_clas_count>0){
				double clas_data[row_clas_count][tr_cols];
				for(i=0;i<row_clas_count;i++){
					for(j=0;j<tr_cols-1;j++){
						clas_data[i][j] = 0.0f;
					}						
				}
				
				//Start : if the class if present
				if(training_classes[cls]!=-9999.0f){
					int clas_row_c = 0;
					int clas_col_c = 0;
					//Start : Taking out the class specific rows
					for(i=0;i<tr_rows;i++){
						double clas = training_data_arr[i][tr_no_inp_col];
						
						if(clas==training_classes[cls]){
							clas_col_c = 0;
							
							for(j=0;j<tr_cols-1;j++){
								clas_data[clas_row_c][clas_col_c] = training_data_arr[i][j];
								sum[cls][j] = sum[cls][j] + training_data_arr[i][j];
								clas_col_c = clas_col_c + 1;
							}
							clas_row_c = clas_row_c + 1;
						}
					}
					//End : Taking out the class specific rows	

					//Calculate Mean for each class
					for(j=0;j<tr_cols;j++){
						mean[cls][j] = sum[cls][j]/(float)row_clas_count;
					}	

					//Calculate Standard Deviation
					for(row=0;row<clas_row_c;row++){
						for( col=0;col<tr_cols-1;col++){//
							//printf("%f \t %f \t %f \n" , clas_data[row][col],mean[cls][col],pow((clas_data[row][col] - mean[cls][col]),2));
							sum_var[cls][col] = sum_var[cls][col] + pow((clas_data[row][col] - mean[cls][col]),2);
						}
						//printf("\n");
					}
					
					for(j=0;j<tr_cols-1;j++){
						
						double variance = sum_var[cls][j]/(double)(row_clas_count-1);
						
						if(variance<=default_variance){
							variance = default_variance;
						}
						
						std[cls][j] = sqrt(variance);
						
					}
					
					for(j=0;j<tr_cols-1;j++){
						printf("Class %d, attribute %d, mean = %.2f, std = %.2f\n",cls,j,mean[cls][j],std[cls][j]);
					}
									
				}
				//End : if the class if present
				
			}
		}
		//Till now data is trained
		//All the class mean and std we have right now
		//Ending Gaussian Probability calculation
		
		//p(x|Ck) : we have to calculate [prob_liklihood]
		//p(Ck) : already have it  [prior_prob_class]
		//p(x) :  we have to calculate [sum_product]
		
		//probability likelihood
		double prob_liklihood[ts_rows][training_no_of_cls];
		double sum_product[ts_rows];
		for(row=0;row<ts_rows;row++){
			for( col=0;col<training_no_of_cls;col++){
				prob_liklihood[row][col] = 0.0f;
			}
			sum_product[row] = 0.0f;
		}
		
		for(row=0;row<ts_rows;row++){
			double sum_product_prior = 0.0f;
			
			for(cls=0;cls<training_no_of_cls;cls++){
				if(training_classes[cls]!=-9999.0f){
					double product = 1.0f;
					for(col=0;col<ts_cols-1;col++){
						double x = testing_data_arr[row][col];
						double m = mean[cls][col];
						double s = std[cls][col];
						
						if(s<default_std){
							s = default_std;
							std[cls][col] = default_std;
						}
											
						double normail_dist = calculate_normal_distribution(x,m,s);
						
						product = product * normail_dist;
					}
					//Calculate p(x|Ck)
					prob_liklihood[row][cls] = product;
					sum_product_prior = sum_product_prior + (product*prior_prob_class[cls]);
				
				}
			}
			//Calculate p(x)
			sum_product[row] = sum_product_prior;
		}
		
		double max_liklihood[ts_rows];
		int predicted_class[ts_rows];
		//Start Calculate the maximum arg
		for(row=0;row<ts_rows;row++){
			max_liklihood[row] = 0.0f;
			predicted_class[row] = 99999;
		}
		
		int predicted_class_label_count[ts_rows];
		
		for(row=0;row<ts_rows;row++){
			predicted_class_label_count[row] = 1;
			
		}
		
		for(row=0;row<ts_rows;row++){
			double maximum_liklihood = 0.0f;
			int predicted_class_label = 99999;
			
			
			for(cls=0;cls<training_no_of_cls;cls++){
				if(training_classes[cls]!=-9999.0f){
					double cls_naive_bayes_prob = (prob_liklihood[row][cls]  * prior_prob_class[cls] )/ sum_product[row];
					//printf("prob_liklihood %f\n",prob_liklihood[row][cls]);
					//printf("prior_prob_class %f\n",prior_prob_class[cls]);
					//printf("sum_product %f\n",sum_product[row]);
					//printf("Maxi %f : cal %f \n",maximum_liklihood,cls_naive_bayes_prob);
					//printf("=======================\n");
					
					if(maximum_liklihood == cls_naive_bayes_prob){
						predicted_class_label_count[row] = predicted_class_label_count[row] + 1;
					}
					
					if(maximum_liklihood < cls_naive_bayes_prob){
						maximum_liklihood = cls_naive_bayes_prob;
						predicted_class_label = cls;
						
						//printf(" row %d \t  maximum_liklihood %f \n",row,maximum_liklihood);
					}
					
					
				}
			}
			
			//printf("=======================\n");
			
			max_liklihood[row] = maximum_liklihood;
			predicted_class[row] = predicted_class_label;
			//printf("Maxi %f : cal %d \n",maximum_liklihood,predicted_class_label);		
		}
		//End Calculate the maximum arg
		
		
		int confusion_matrix[training_no_of_cls][training_no_of_cls];
		for(i=0;i<training_no_of_cls;i++){
			for(j=0;j<training_no_of_cls;j++){
				confusion_matrix[i][j] = 0;
			}
		}
		
		for(row=0;row<ts_rows;row++){
			int pred_class_label = predicted_class[row];
			int actual_class_label = (int) testing_data_arr[row][ts_cols-1];
	
			confusion_matrix[actual_class_label][pred_class_label] = confusion_matrix[actual_class_label][pred_class_label] + 1;
		}
		
		double accuracy = 0.0f;
		int correct_classification = 0;
		for(i=0;i<training_no_of_cls;i++){
			for(j=0;j<training_no_of_cls;j++){
				if(i==j)
					correct_classification = correct_classification + confusion_matrix[i][j];
				//printf("%d \t",confusion_matrix[i][j]);
			}
			//printf("\n");
		}
		correct_classification = 0;
		for(row=0;row<ts_rows;row++){
			int pred_class_label = predicted_class[row];
			int actual_class_label = (int) testing_data_arr[row][ts_cols-1];
			
			double accuracy = 0.0f;
			if(pred_class_label==actual_class_label && predicted_class_label_count[row] == 1){
				correct_classification = correct_classification + 1;
				accuracy = 1.0f;
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] == 1)
				accuracy = 0.0f;
			else if(pred_class_label==actual_class_label && predicted_class_label_count[row] > 1){
				correct_classification = correct_classification +1;
				accuracy = 1.0f/(double)predicted_class_label_count[row];
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] > 1)
				accuracy = 0.0f;
			
			//printf(" no of couunt %d \n",predicted_class_label_count[row]);
			
			printf("ID=%5d, predicted=%3d, probability = %.4lf, true=%3d, accuracy=%4.2lf\n",row, predicted_class[row],max_liklihood[row], (int)testing_data_arr[row][ts_cols-1], accuracy);
		}
		printf("classification accuracy=%6.4lf\n",(double)correct_classification/(double)ts_rows);
	//end of gaussian
	}
	
	else if(strcmp(classification_type,"histograms")==0){
		double prior_prob_class[training_no_of_cls];
		for(i=0;i<training_no_of_cls;i++){
			prior_prob_class[i] = 0.0f;
		}
		for(i=0;i<training_no_of_cls;i++){
			if(training_classes[i]!=-9999.0f){
				prior_prob_class[i] = (float)class_row_count[i]/(float)tr_rows;
			}
			//printf("P(C%d) = %f\n",i,prior_prob_class[i] );
		}
		double prob_bin[training_no_of_cls][tr_cols][number_of_segments];
		//Binning
		int bin[training_no_of_cls][tr_cols-1][number_of_segments];
		double bin_min_max[training_no_of_cls][tr_cols-1][number_of_segments][2];
		double bin_range[training_no_of_cls][tr_cols-1];
		for(i=0;i<training_no_of_cls;i++){
			for( attribute=0;attribute<tr_cols-1;attribute++){
				for(bin_c=0;bin_c<number_of_segments;bin_c++){
					
					
					bin_min_max[i][attribute][bin_c][0] = 99999.0f;
					bin_min_max[i][attribute][bin_c][1] = 0.0f;
			
					bin[i][attribute][bin_c] = 0;
					prob_bin[i][attribute][bin_c] = 0.0f;
				}
				bin_range[i][attribute] = 0.0f;
			}
		}
		
		//training_no_of_cls
		for(cls = 0;cls < training_no_of_cls;cls++){
			int row_clas_count = class_row_count[cls];
			if(row_clas_count>0){
				double clas_data[row_clas_count][tr_cols];
				for(i=0;i<row_clas_count;i++){
					for(j=0;j<tr_cols;j++){
						clas_data[i][j] = 0.0f;
					}						
				}
				
				//Start : Taking out the class specific rows
				if(training_classes[cls]!=-9999.0f){
					int clas_row_c = 0;
					//int clas_col_c = 0;
					
					for(i=0;i<tr_rows;i++){
						double clas = training_data_arr[i][tr_no_inp_col];
						if(clas==training_classes[cls]){
							//clas_col_c = 0;
							
							for( j=0;j<tr_cols-1;j++){
								clas_data[clas_row_c][j] = training_data_arr[i][j];
								//clas_col_c = clas_col_c + 1;
							}
							clas_row_c = clas_row_c + 1;
						}
					}		
					//Start : Taking out the class specific rows	
					
					//Here I have Each class Matrix
					//find min and max for the each attribute and each class
					double min[tr_cols-1];
					double max[tr_cols-1];
					//double G[tr_cols-1];
				
					for(i=0;i<tr_cols-1;i++){
						min[i] = 0.0f;
						max[i] = 0.0f;
						//G[i] = 0.0f;
					}
				
				
					for(i=0;i<row_clas_count;i++){
						for(j=0;j<tr_cols-1;j++){
							if(i==0){
								min[j] = clas_data[i][j];
								max[j] = clas_data[i][j];
							}else{
								if(min[j]>clas_data[i][j])
									min[j] = clas_data[i][j];
								if(max[j]<clas_data[i][j])
									max[j] = clas_data[i][j];
							}
						}
					}
				
					for(attribute=0;attribute<tr_cols-1;attribute++){
						double range_of_class_attr = max[attribute]-min[attribute];
						
						double range  = (range_of_class_attr)/(double)(number_of_segments-3);
						
						if(range<default_g){
							bin_range[cls][attribute] = default_g;
						}else{
							bin_range[cls][attribute] = range;
						}
						
						//if(attribute==1)
						//printf("class %d \t Attribute %d, \t Min: %f, \t Max: %f, \t Bin range: %f \n",cls,attribute,min[attribute],max[attribute],bin_range[cls][attribute]);
					}
					
				
					for(attribute=0;attribute<tr_cols-1;attribute++){
						double min_attr = min[attribute];
						double g_2 = bin_range[cls][attribute]/2.0f;
						double g = bin_range[cls][attribute];
						for(bin_c=0;bin_c<number_of_segments;bin_c++){
							
							double min = 0.0f;
							double max = 0.0f;
							
							if(bin_c==0){
								min = -999999.0f;
								max = min_attr-g_2;
							}else if(bin_c==1){
								min = min_attr-g_2;
								max = min_attr+g_2;
							}else if(bin_c==number_of_segments-1){
								min = min_attr+(bin_c-2)*g+g_2;
								max = 999999.0f;
							}else{
								min = min_attr+(bin_c-2)*g+g_2;
								max = min_attr+(bin_c-1)*g+g_2;
							}
							
							//Min
							bin_min_max[cls][attribute][bin_c][0] = min;
							//Max
							bin_min_max[cls][attribute][bin_c][1] = max;
						}
					}
					
					
				
					//printf("as class %d count %d \n",cls,row_clas_count);
					for(row=0;row<row_clas_count;row++){
						for(attribute=0;attribute<tr_cols-1;attribute++){
							double min_attr = min[attribute];
							double x = clas_data[row][attribute];
							double g_2 = bin_range[cls][attribute]/2.0f;
							double g = bin_range[cls][attribute];
							
							for(bin_c=0;bin_c<number_of_segments;bin_c++){
								//Bin 0
								if(x<(min_attr-g_2)){
										bin[cls][attribute][0] = bin[cls][attribute][0] + 1;
										/* if(x<bin_min_max[cls][attribute][0][0]){
											bin_min_max[cls][attribute][0][0] = x;
										}
										if(x>bin_min_max[cls][attribute][0][1]){
											bin_min_max[cls][attribute][0][1] = x;
										}  */
										break;
								}

								//Bin 1
								if(x>=(min_attr-g_2) && x<(min_attr+g_2)){
									
									bin[cls][attribute][1] = bin[cls][attribute][1] + 1;
									/* if(x<bin_min_max[cls][attribute][1][0]){
										bin_min_max[cls][attribute][1][0] = x;
									}
									if(x>bin_min_max[cls][attribute][1][1]){
										bin_min_max[cls][attribute][1][1] = x;
									} 	 */									
									break;
								}
								
								
								//Bin between 2->N-1
								if(x>=(min_attr+(bin_c-2)*g+g_2) &&  x<(min_attr+(bin_c-1)*g+g_2)){
									bin[cls][attribute][bin_c] = bin[cls][attribute][bin_c] + 1;
									
									/* if(x<bin_min_max[cls][attribute][bin_c][0]){
										bin_min_max[cls][attribute][bin_c][0] = x;
									}
									
									if(x>bin_min_max[cls][attribute][bin_c][1]){
										bin_min_max[cls][attribute][bin_c][1] = x;
									} 	 */									
									
									break;
								}
							}
						}
					}
					
					/* for(int attribute=0;attribute<tr_cols-1;attribute++){
						for(int bin_c=0;bin_c<number_of_segments;bin_c++){
							if(bin_min_max[cls][attribute][bin_c][0]==99999.0f)
								bin_min_max[cls][attribute][bin_c][0] = -0.0f;
						}
					} */
					 
				    /* if(cls==1)
						for(attribute=0;attribute<tr_cols-1;attribute++){
							for(int bin_c=0;bin_c<number_of_segments;bin_c++){
								printf("Class %d \t attribute %d \t  Bin %d \t \t min %f  \t max %f \t no_of_element %d\n",cls,attribute,bin_c,bin_min_max[cls][attribute][bin_c][0],bin_min_max[cls][attribute][bin_c][1],bin[cls][attribute][bin_c]);
							}
						}   */ 
					
				}
			}
		}//Cls Loop Complete
		
		
		//Training complete
		for( cls = 0;cls < training_no_of_cls;cls++){
			int row_clas_count = class_row_count[cls];
			if(row_clas_count>0){
				for( attribute=0;attribute<tr_cols-1;attribute++){
					for( bin_c=0;bin_c<number_of_segments;bin_c++){
						///if(attribute==0){
							
							//double bin_range = bin_min_max[cls][bin_c][attribute][1]-bin_min_max[cls][bin_c][attribute][0];
							double bin_range_intv = bin_range[cls][attribute];
						    if(bin_range_intv<default_g)
								bin_range_intv = default_g;  
							
							//TODO if bin range is zero
							
							//printf("bin_count %d \t bin range %f \t row_count %d \n",
							//		bin[cls][bin_c][attribute],bin_range_intv,row_clas_count);
							
							//if(attribute==6)
							//printf("class %d \t Bin Count %d \t row_clas_count %d \t bin_range_intv %f\n",cls,bin[cls][attribute][bin_c],row_clas_count,bin_range_intv);
							double prob = ((double)bin[cls][attribute][bin_c]/((double)row_clas_count*(bin_range_intv)));
							
							/* if(prob==0.0f)
								printf("Class %d \t Attribute %d \t Bin %d \t Prob %f \n",cls,attribute,bin_c,prob);
							 */
							prob_bin[cls][attribute][bin_c] = prob;
							printf("Class %d, attribute %d, bin %d, P(bin | class) = %.2f\n",cls,attribute,bin_c,prob);
						//}
					}
				}
			}
		}
		
		
		//Classification with the histogram starts
		
		//Naive Based Histogram Probability
		
		double max_liklihood[ts_rows];
		int predicted_class[ts_rows];
		double sum_product[ts_rows];
		int predicted_class_label_count[ts_rows];
		double prob_liklihood[ts_rows][training_no_of_cls];
		double prob_row[ts_rows];
		for(row=0;row<ts_rows;row++){
			max_liklihood[row] = 0.0f;
			predicted_class[row] = 99999;
			predicted_class_label_count[row] = 1; 
			sum_product[row] = 0.0f;
			prob_row[row] = 0.0f;
			
			for( cls = 0;cls < training_no_of_cls;cls++){
				prob_liklihood[row][cls] = 0.0f;
			}
		}
		
		//TODO
		for(row=0;row<ts_rows;row++){
			double prob_max_product = 0.0f;
			int pred_class = 99999;
			
			double all_class_prod_sum = 0.0f;
			for( cls = 0;cls < training_no_of_cls;cls++){
				if(training_classes[cls]!=-9999.0f){
					double prob_class_wise_product = 1.0f;
					
					for(attribute=0;attribute<ts_cols-1;attribute++){
						double x = testing_data_arr[row][attribute];
						double prob = 0.0f;
						
						//printf("class %d \t Attribute %d \t Value %f \n",cls,attribute,x);
						
						
						for( bin_c=0;bin_c<number_of_segments;bin_c++){
							//printf("Bin Choosing %d \t :  Min %f \t Max %f \n",bin_c,bin_min_max[cls][attribute][bin_c][0],bin_min_max[cls][attribute][bin_c][1]);
							if(x >= bin_min_max[cls][attribute][bin_c][0] && x < bin_min_max[cls][attribute][bin_c][1]){
								prob = prob_bin[cls][attribute][bin_c];
								//printf("Rows %d \t Class %d \t Attribute %d \t Bin %d \t Data %f \t Min %f \t Max %f \t Prob %f \n",row,cls,attribute,bin_c,x,bin_min_max[cls][attribute][bin_c][0],bin_min_max[cls][attribute][bin_c][1],prob_bin[cls][attribute][bin_c]);
								break;
							}
						}
											
						//TODO Prior Of Class
						
						prob_class_wise_product = prob_class_wise_product * prob;
						
					} 
					//printf("Class %d \t prob_class_wise_product %f \n",cls,prob_class_wise_product);
					//printf("==========================\n");
					//P(x1,x2,x3|Ck)==num/total class
					prob_liklihood[row][cls] = prob_class_wise_product*prior_prob_class[cls];
					
					all_class_prod_sum = all_class_prod_sum + (prob_class_wise_product*prior_prob_class[cls]);
					
					
					
					
					/* if(prob_max_product == prob_class_wise_product){
							predicted_class_label_count[row] = predicted_class_label_count[row] + 1;
					}
					
					if(prob_max_product<prob_class_wise_product){
							prob_max_product = prob_class_wise_product;
							pred_class = cls;
					} */
				}					
			}
			
			sum_product[row] = all_class_prod_sum;
			max_liklihood[row] = prob_max_product/all_class_prod_sum;
			predicted_class[row] = pred_class;
			
			
		}
		
		//TODO  - Calculate Probability
		for(row=0;row<ts_rows;row++){
			int pred_class = 99999;
			double max_prob = 0.0f;
			//training_no_of_cls
			for( cls = 0;cls < training_no_of_cls;cls++){
				if(training_classes[cls]!=-9999.0f){
					double cls_naive_bayes_prob = 0.0f;
					double sum_prob = sum_product[row];
					if(sum_prob==0.0f){
						cls_naive_bayes_prob = 1.0f/(double)training_no_of_cls;
					}else{
						cls_naive_bayes_prob = ( prob_liklihood[row][cls])/ sum_product[row];
					}
					
					//if(row==0)
					//printf("Class %d \t Class Prob %f \t Sum of Class %f Division %F\n",cls,prob_liklihood[row][cls],sum_product[row],cls_naive_bayes_prob);
					/* if(max_prob<cls_naive_bayes_prob){
						max_prob=cls_naive_bayes_prob;
					} */
					
					if(max_prob==cls_naive_bayes_prob){
							predicted_class_label_count[row] = predicted_class_label_count[row] + 1;
					}
					
					if(max_prob<cls_naive_bayes_prob){
							max_prob=cls_naive_bayes_prob;
							pred_class = cls;
					}
				}
			}
			
			prob_row[row] = max_prob;
			predicted_class[row] = pred_class;
			
			
		}
		
		
		int correct_classification = 0;
		for(row=0;row<ts_rows;row++){
			int pred_class_label = predicted_class[row];
			int actual_class_label = (int) testing_data_arr[row][ts_cols-1];
			
			double accuracy = 0.0f;
			if(pred_class_label==actual_class_label && predicted_class_label_count[row] == 1){
				accuracy = 1.0f;
				correct_classification = correct_classification + 1;
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] == 1)
				accuracy = 0.0f;
			else if(pred_class_label==actual_class_label && predicted_class_label_count[row] > 1){
				accuracy = 1.0f/(double)predicted_class_label_count[row];
				correct_classification = correct_classification + 1;
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] > 1)
				accuracy = 0.0f;
			
			printf("ID=%5d, predicted=%3d, probability = %.4lf, true=%3d, accuracy=%4.2lf\n",row, predicted_class[row],prob_row[row], (int)testing_data_arr[row][ts_cols-1], accuracy);
		}
		printf("classification accuracy=%6.4lf\n",(double)correct_classification/(double)ts_rows);
		
	}//end of Histograms
	else if(strcmp(classification_type,"mixtures")==0){
			
		double mixs_gaussian_data_sum[training_no_of_cls][tr_cols][number_of_segments];
		double weigth[training_no_of_cls][tr_cols][number_of_segments];
		double std[training_no_of_cls][tr_cols][number_of_segments];
		double mean[training_no_of_cls][tr_cols][number_of_segments];
		
		
		//double n_gaussian_data[training_no_of_cls][tr_cols][number_of_segments]
		
		for( cls = 0;cls < training_no_of_cls;cls++){
			for( attribute=0;attribute<tr_cols-1;attribute++){
				for( gaussian = 0;gaussian < number_of_segments;gaussian++){
					weigth[cls][attribute][gaussian] = (1.0f/(double)number_of_segments);
					std[cls][attribute][gaussian] = 1.0f;
					mean[cls][attribute][gaussian] = 0.0f;
				}
			}
		}
		
		double prior_prob_class[training_no_of_cls];
		for(i=0;i<training_no_of_cls;i++){
			prior_prob_class[i] = 0.0f;
		}
		for(i=0;i<training_no_of_cls;i++){
			if(training_classes[i]!=-9999.0f){
				prior_prob_class[i] = (float)class_row_count[i]/(float)tr_rows;
			}
			//printf("P(C%d) = %f\n",i,prior_prob_class[i] );
		}
		
					//MISSED	
		for( cls = 0;cls < training_no_of_cls;cls++){
				//Create N number of Gaussian
				int row_clas_count = class_row_count[cls];
				if(row_clas_count>0){
					double pij[row_clas_count][tr_cols][number_of_segments];
					double normal_dist[row_clas_count][tr_cols][number_of_segments];
					double px[row_clas_count][tr_cols][number_of_segments];
					
					for( y=0;y<row_clas_count;y++){
						for( j=0;j<tr_cols-1;j++){
							for( k = 0;k < number_of_segments;k++){
								pij[y][j][k] = 0.0f;
								normal_dist[y][j][k] = 0.0f;
								px[y][j][k] = 0.0f;
							}
						}
					}
					
					double clas_data[row_clas_count][tr_cols];
					for( i=0;i<row_clas_count;i++){
						for( j=0;j<tr_cols;j++){
							clas_data[i][j] = 0.0f;
						}						
					}
					
					if(training_classes[cls]!=-9999.0f){
						int clas_row_c = 0;
						int clas_col_c = 0;
						
						for( i=0;i<tr_rows;i++){
							double clas = training_data_arr[i][tr_no_inp_col];
							if(clas==training_classes[cls]){
								clas_col_c = 0;
								
								for( j=0;j<tr_cols;j++){
									clas_data[clas_row_c][clas_col_c] = training_data_arr[i][j];
									//printf(" %f ", training_data_arr[i][j]);
									clas_col_c = clas_col_c + 1;
								}
								//printf("\n");
								clas_row_c = clas_row_c + 1;
							}
						}

						double min[tr_cols-1];
						double max[tr_cols-1];
						double G[tr_cols-1];
						
						for( i=0;i<tr_cols-1;i++){
							min[i] = 0.0f;
							max[i] = 0.0f;
							G[i] = 0.0f;
						}
						
						for( i=0;i<row_clas_count;i++){
							for( j=0;j<tr_cols-1;j++){
								if(i==0){
									min[j] = clas_data[i][j];
									max[j] = clas_data[i][j];
								}else{
									if(min[j]>clas_data[i][j])
										min[j] = clas_data[i][j];
									if(max[j]<clas_data[i][j])
										max[j] = clas_data[i][j];
								}
							}
						}
						
						for( attribute=0;attribute<tr_cols-1;attribute++){
							//TODO
							double g = (max[attribute]-min[attribute])/(double)number_of_segments;
							if(g<default_g)
								G[attribute] = default_g;
							else	
								G[attribute] = g;
							//printf("Class %d \t Attribute %d, \t Min: %f, \t Max: %f, \t G: %f \n",cls,attribute,min[attribute],max[attribute],G[attribute]); 
						}
						
						
						//Prepare N Gaussian and find STD and MEAN and Update weight
						for( attribute=0;attribute<tr_cols-1;attribute++){
							for( gaussian=0;gaussian<number_of_segments;gaussian++){
								mean[cls][attribute][gaussian] = min[attribute] + (gaussian*G[attribute]) + (G[attribute]/2.0f);
							}
						}
						
						//EMSTEP
						for( stop=0;stop<50;stop++){
							if(row_clas_count>0){
								//E Step
								for( class_row=0;class_row<row_clas_count;class_row++){
									for( attribute=0;attribute<tr_cols-1;attribute++){
										double x = clas_data[class_row][attribute];
										double sum_ni_xj = 0.0f;
										double dist[number_of_segments];
										
										for( gaussian=0;gaussian<number_of_segments;gaussian++){
											double w = weigth[cls][attribute][gaussian];
											double m = mean[cls][attribute][gaussian];
											double s = std[cls][attribute][gaussian];
													
											//Ni(xj)
											double norm_dist = calculate_normal_distribution(x,m,s);
											
											/* if(stop==0 && cls==1 && attribute==0)
												printf("Step %d \t class %d \t Attribute %d \t gaussian %d \t Nij %f \t M %f \t S %f \t W %f\n",stop,cls,attribute,gaussian, norm_dist,m,s,w);
											 */
											
											sum_ni_xj = sum_ni_xj + (norm_dist*w) ;
											dist[gaussian] = norm_dist*w;
										}
										
										for(gaussian=0;gaussian<number_of_segments;gaussian++){
											pij[class_row][attribute][gaussian] = (dist[gaussian])/sum_ni_xj;
											
											
										 //if(stop==0 && cls==1 && attribute==0)
										//		printf("Step %d \t class %d \t Attribute %d \t gaussian %d \t Nij %f \t W %f \t Sum %f \t Pij %f\n",stop,cls,attribute,gaussian, dist[gaussian]/weigth[9][0][gaussian],weigth[9][0][gaussian],sum_ni_xj,pij[class_row][attribute][gaussian]);
										 
										}	
									}
								}
							}//Pij Calculation Complete
							
							/* if(row_clas_count>0){
								for( attribute=0;attribute<1;attribute++){
									for( class_row=0;class_row<row_clas_count;class_row++){
										for( gaussian=0;gaussian<number_of_segments;gaussian++){
											printf("Class %d \t Attribute %d \t Row %d \t Gaussian %d \t Pij %f \n ",cls,attribute,class_row,gaussian,pij[class_row][attribute][gaussian]);
										}
									}
								}
							}
							 */
							
							
							/* if(cls==1){
								for( class_row=0;class_row<row_clas_count;class_row++){
									printf("%f\n",pij[class_row][0][0]);
								}
								printf("Stop %d Number of Record %d ",stop,row_clas_count);
							}
							 */
							
							
							//M Step
							if(row_clas_count>0){
								for(attribute=0;attribute<tr_cols-1;attribute++){
									double sum_gaussian_sum_pij = 0.0f;
									double pij_sum_indv_gaussian[number_of_segments];
									for(gaussian=0;gaussian<number_of_segments;gaussian++){
										double pij_x_sum_each_gaussian = 0.0f;
										double pij_sum_each_gaussian = 0.0f;
										double pij_var_sum_each_gaussian = 0.0f;
										
										
										for( class_row=0;class_row<row_clas_count;class_row++){
											double x = clas_data[class_row][attribute];
											
											/* if(cls==1 && gaussian==0 && attribute==0)
											printf("%f \t %f \t %f \t %f\n",x,m,pow((x-m),2),pij[class_row][attribute][gaussian]);*/
											
											pij_x_sum_each_gaussian = pij_x_sum_each_gaussian + pij[class_row][attribute][gaussian]*x;
											//pij_var_sum_each_gaussian = pij_var_sum_each_gaussian + (pij[class_row][attribute][gaussian] * pow((x-m),2));
											pij_sum_each_gaussian = pij_sum_each_gaussian + pij[class_row][attribute][gaussian];
										}
										
										pij_sum_indv_gaussian[gaussian] = pij_sum_each_gaussian;
										mean[cls][attribute][gaussian] = pij_x_sum_each_gaussian/pij_sum_each_gaussian;
										double m = mean[cls][attribute][gaussian];
										for( class_row=0;class_row<row_clas_count;class_row++){
											double x = clas_data[class_row][attribute];
											
											/* if(cls==1 && gaussian==0 && attribute==0)
											printf("%f \t %f \t %f \t %f\n",x,m,pow((x-m),2),pij[class_row][attribute][gaussian]);*/
											
											//pij_x_sum_each_gaussian = pij_x_sum_each_gaussian + pij[class_row][attribute][gaussian]*x;
											pij_var_sum_each_gaussian = pij_var_sum_each_gaussian + (pij[class_row][attribute][gaussian] * pow((x-m),2));
											//pij_sum_each_gaussian = pij_sum_each_gaussian + pij[class_row][attribute][gaussian];
										}
										
										
										double m_step_std = sqrt((pij_var_sum_each_gaussian/pij_sum_each_gaussian));
										
										if(m_step_std<default_std){
											std[cls][attribute][gaussian] = default_std;
										}else{
											std[cls][attribute][gaussian] = m_step_std;
										} 
										
										/* if(cls==1 && attribute==0)
											printf("Stop %d \t Class %d, attribute %d, Gaussian %d, Mean %f , std = %.2f , Var Num %f  Var deno %f Mean %f\n",stop,cls,attribute,gaussian,mean[cls][attribute][gaussian],m_step_std,pij_var_sum_each_gaussian,pij_sum_each_gaussian,m);
									 */
										sum_gaussian_sum_pij = sum_gaussian_sum_pij + pij_sum_each_gaussian;
									}
									
									//double weight_sum = 0.0f;
									for( gaussian=0;gaussian<number_of_segments;gaussian++){
										weigth[cls][attribute][gaussian] = pij_sum_indv_gaussian[gaussian]/sum_gaussian_sum_pij;
										
										//weight_sum = weight_sum + weigth[cls][attribute][gaussian];
									}
									/* if(cls==1 && attribute==0)
										for( gaussian=0;gaussian<number_of_segments;gaussian++){
											
											printf("Stop %d \t Weight : %f \n",stop,weigth[1][0][gaussian]);
										} */
								}
							}
						}
						//50 Step over
						//Printf
						for( attribute=0;attribute<tr_cols-1;attribute++){
								for( gaussian=0;gaussian<number_of_segments;gaussian++){
									printf("Class %d, attribute %d, Gaussian %d, mean = %.2f, std = %.2f\n",cls,attribute,gaussian,mean[cls][attribute][gaussian],std[cls][attribute][gaussian]);
								}
						} 
						
					}
				
			}
		}//Each Class data End analysis
		double likelihood_prob_each_row_per_class[training_no_of_cls];
		int predicted_class_label_count[ts_rows];
		int predicted_class[ts_rows];
		double prob_each_row[ts_rows];
		
		for(row=0;row<ts_rows;row++){
			predicted_class_label_count[row] = 1;
			predicted_class[row] = 0;
			prob_each_row[row] = 0.0f;
		}
		
		for(row=0;row<ts_rows;row++){
			double max_liklihood_of_class = 0.0f;
			int class_pred = 99999;
			double sum_max_liklihood_all_class = 0.0f;
			for(cls=0;cls<training_no_of_cls;cls++){	
				if(training_classes[cls]!=-9999.0f){
					double mj_all_attribute_all_gaussian_per_class = 1.0f;
					for( attribute=0;attribute<tr_cols-1;attribute++){
						double x = testing_data_arr[row][attribute];
						
						double mj = 0.0f;
						for( gaussian=0;gaussian<number_of_segments;gaussian++){
							double w = weigth[cls][attribute][gaussian];
							double m = mean[cls][attribute][gaussian];
							double s = std[cls][attribute][gaussian];
							//printf("Class %d \t Attribute %d \t Gaussian %d \t Weight %f \t Mean %f \t Std %f \n",cls,attribute,gaussian,w,m,s);
							double ni_w = w*calculate_normal_distribution(x,m,s);
							
							/* if(ni_w==0.0f){
								printf(" ni_w is Zero attribute %d\n",attribute);
							} */
							mj = mj + ni_w;
						}
						/* if(mj==0.0f){
							printf(" Mj is Zero attribute %d\n",attribute);
						} */
						mj_all_attribute_all_gaussian_per_class = mj_all_attribute_all_gaussian_per_class * mj;
					}
					
					likelihood_prob_each_row_per_class[cls] = mj_all_attribute_all_gaussian_per_class*prior_prob_class[cls];
					
					sum_max_liklihood_all_class = sum_max_liklihood_all_class + likelihood_prob_each_row_per_class[cls];
					
				}
			}
			
			
			
			//double max_liklihood_of_class = 0.0f;
			for(cls=0;cls<training_no_of_cls;cls++){	
				if(training_classes[cls]!=-9999.0f){
					double class_prob = likelihood_prob_each_row_per_class[cls]/sum_max_liklihood_all_class;
					
					/* if(cls==1)
						printf("Class %d \t Prob %f \t Sum %f \n ",cls,class_prob,sum_max_liklihood_all_class);
					 */
					if(max_liklihood_of_class==class_prob){
						predicted_class_label_count[row] = predicted_class_label_count[row] + 1;
					}
					
					if(max_liklihood_of_class<class_prob){
						max_liklihood_of_class = class_prob;
						class_pred= cls;
					}
				}
			}
			
			//printf(" prob_each_row %f\n",max_liklihood_of_class);
			//if(class_pred!=99999){
			prob_each_row[row] = max_liklihood_of_class;
			//}
			predicted_class[row] = class_pred;
		}
		
		
		//Predcitve
		int correct_classification = 0;
		for(row=0;row<ts_rows;row++){
			int pred_class_label = predicted_class[row];
			int actual_class_label = (int) testing_data_arr[row][ts_cols-1];
			
			double accuracy = 0.0f;
			if(pred_class_label==actual_class_label && predicted_class_label_count[row] == 1){
				accuracy = 1.0f;
				correct_classification = correct_classification + 1;
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] == 1)
				accuracy = 0.0f;
			else if(pred_class_label==actual_class_label && predicted_class_label_count[row] > 1){
				accuracy = 1.0f/(double)predicted_class_label_count[row];
				correct_classification = correct_classification + 1;
			}
			else if(pred_class_label!=actual_class_label && predicted_class_label_count[row] > 1){
				accuracy = 0.0f;
			}
			
			printf("ID=%5d, predicted=%3d, probability = %.4lf, true=%3d, accuracy=%4.2lf\n",row, predicted_class[row],prob_each_row[row], (int)testing_data_arr[row][ts_cols-1], accuracy);
		}
		printf("classification accuracy=%6.4lf\n",(double)correct_classification/(double)ts_rows);
	}
	
	
	
	return 0;
}
