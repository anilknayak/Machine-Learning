K Mean Clustering is implemented in Java
==========================================
To run the program follow the below steps 

1. Copy following files in one directory 
	1.1: k_means_cluster.jar
	1.2: yeast_test.txt
	
2. Open Command Prompt or Linux Terminal
3. Go to same directory in Command Prompt or Linux Terminal, where you have copied the files (jar and txt)
4. the type the following command to run the program
	java -jar k_means_cluster.jar <data_file_name> <k> <iterations>
	
	Ex: 
	java -jar k_means_cluster.jar yeast_test.txt 2 5
	

	Note:  	First Argument (String): data_file_name shoule be quoted and passed 
	       	Second Argument (Integer): k is number of cluster
		Third Argument (Integer): iterations number of iterations the algorithm will execute
	       	
5. All the training and testing files has to be in same folder as the program file (*.jar) resides, if not then provide the absolute path for data file.

6. After this output will be shown as mentioned in the assignment.