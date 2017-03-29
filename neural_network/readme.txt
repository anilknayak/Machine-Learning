To run the program follow the below steps

1. Open neural_network.m file in matlab 

2. Then in Matlab command window type following
	neural_network(<training_file_name>,<testing_file_name>,<layers>,<units_per_layer>,<rounds>)

	Ex: neural_network('pendigits_training.txt','pendigits_test.txt',3,20,20)

	Note:  First Argument (String): training_file_name shoule be quoted and passed 
	          Second Argument (String): testing_file_name shoule be quoted and passed 
                          Third Argument (Integer): layers
	          Fourth Argument (Integer): number of units per hidden layer
	          Fifth Argument (Integer): number of rounds 

3. All the training and testing files has to be in same folder as the program file (*.m) resides, if not then provide the absolute path for training and testing file.

4. After this output will be shown as mentioned in the assignment answer but it may change a little as the weights are choosen randomly from -0.05 to 0.05