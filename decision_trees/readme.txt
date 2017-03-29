Decision Tree is implemented in Java
==========================================
To run the program follow the below steps 

1. Copy following files in one directory 
	1.1: dtree.jar
	1.2: pendigits_training.txt
	1.3: pendigits_test.txt
	
2. Open Command Prompt or Linux Terminal
3. Go to same directory in Command Prompt or Linux Terminal, where you have copied the files (jar and txt)
5. the type the following command to run the program
	java -jar dtree.jar <training_file> <test_file> <option> <pruning_thr>
	
	Ex: 
	java -jar dtree.jar pendigits_training.txt pendigits_test.txt optimized 50
	java -jar dtree.jar pendigits_training.txt pendigits_test.txt randomized 50
	java -jar dtree.jar pendigits_training.txt pendigits_test.txt forest3 50
	java -jar dtree.jar pendigits_training.txt pendigits_test.txt forest15 50

	Note:  First Argument (String): training_file_name shoule be quoted and passed 
	       Second Argument (String): testing_file_name shoule be quoted and passed 
           Third Argument (String): option [ optimized , randomized , forest3 , forest15 ]
	       Fourth Argument (Integer): pruning_thr

3. All the training and testing files has to be in same folder as the program file (*.jar) resides, if not then provide the absolute path for training and testing file.

4. After this output will be shown as mentioned in the assignment, answer but it may change a little becasuse of randomized algorithm.