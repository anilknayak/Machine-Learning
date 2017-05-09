To run the program follow the below steps

1. Open knn_classify.m file in matlab 

2. Then in Matlab command window type following
	knn_classify( <training_file_name>,<testing_file_name>,<k> )

	Ex: 
	    knn_classify('pendigits_training.txt','pendigits_test.txt',1)
    	    knn_classify('pendigits_training.txt','pendigits_test.txt',3)
	    knn_classify('pendigits_training.txt','pendigits_test.txt',5)
	    knn_classify('satellite_training.txt','satellite_test.txt',1)
	    knn_classify('satellite_training.txt','satellite_test.txt',3)
	    knn_classify('satellite_training.txt','satellite_test.txt',5)
	    knn_classify('yeast_training.txt','yeast_test.txt',1)
	    knn_classify('yeast_training.txt','yeast_test.txt',3)
	    knn_classify('yeast_training.txt','yeast_test.txt',5)

	Note:  First Argument (String): training_file_name shoule be quoted and passed 
	          Second Argument (String): testing_file_name shoule be quoted and passed 
                          Third Argument (Integer): number of K nearest neighbour 
	         
	         
3. All the input files has to be in same folder as the program file (*.m) resides, if not then provide the absolute path for input file.

4. After this output will be shown as mentioned in the assignment answer.