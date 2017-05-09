Dynamic Time Warping is implemented in Java
==========================================
To run the program follow the below steps 

1. Copy following files in one directory 
	1.1: dtw_classify.jar
	1.2: asl_training.txt
	1.3: asl_test.txt
	
2. Open Command Prompt or Linux or Windows Terminal
3. Go to same directory in Command Prompt or Linux Terminal, where you have copied the files (jar and txt)
4. Type the following command to run the program
	java -jar dtw_classify.jar <training_file_name> <testing_file_name>
	
	Ex: 
	java -jar dtw_classify.jar asl_training.txt asl_test.txt
	

	Note:  	First Argument (String): training_file_name shoule be quoted and passed 
	       	Second Argument (String): testing_file_name shoule be quoted and passed 
	       	
5. All the training and testing files has to be in same folder as the program file (*.jar) resides, if not then provide the absolute path for training and testing file.

6. After this output will be shown as mentioned in the assignment, answer but it may change a little becasuse of randomized algorithm.