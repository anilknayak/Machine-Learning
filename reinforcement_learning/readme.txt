Value Iteration : Reinforcement  is implemented in Java
==========================================
To run the program follow the below steps 

1. Copy following files in one directory 
	1.1: value_iteration.jar
	1.2: environment2.txt
	1.3: environment1.txt
	
2. Open Command Prompt or Linux Terminal
3. Go to same directory in Command Prompt or Linux Terminal, where you have copied the files (jar and txt)
4. the type the following command to run the program
	java -jar value_iteration.jar <environment_file> <non_terminal_reward> <gamma> <K>
	
	Ex: 
	java -jar value_iteration.jar environment2.txt -0.04 0.9 20
	

	Note:  	First Argument (String): environment file name [without qoute]
	       	Second Argument (Integer): non_terminal_reward is the non terminal state reward
		Third Argument (Integer): gamma
		Fourth Argument (Integer): K number of iterations
	       	
5. All the training and testing files has to be in same folder as the program file (*.jar) resides, if not then provide the absolute path for environment file.

6. After this output will be shown as mentioned in the assignment.