To execute the naive bayes classifier for histograms, gaussians and mixtures following are the steps to follow

Compile Program
--------------------------
1. Unzip the folder naive_bayes.zip
2. Go to Folder naive_bayes inside zip
3. type following command to compile program
	gcc -o naive_bayes naive_bayes.c -lm

Run Program
---------------------------
After the code is compiled follow the instruction to run specific classifier 

General instruction for File to read
----------------------------------------------
1. Keep all the training and testing file in the same directory as naive_bayes.c program file
2. I have already submitted Yeast data set in same folder 
3. If your file is not there is the naive_bayes folder then you have to provide absolute path in your file system for input training and testing file


For Histograms 
-----------------------------------------
1.  ./naive_bayes <training file with extention> <testing file with extension> histograms 7
2. See the output

For Gaussians
-----------------------------------------
1.  ./naive_bayes <training file with extention> <testing file with extension> gaussians
2. See the output

For Mixtures of Gaussians
-----------------------------------------
1.  ./naive_bayes <training file with extention> <testing file with extension> mixtures 3
2. See the output