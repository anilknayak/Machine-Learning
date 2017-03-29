import java.util.ArrayList;


public class Training {
	
	
	
	
	public Node DecisionTree(ArrayList<ArrayList<Double>> currentMatrix, ArrayList<Double> attributes,
			ArrayList<Double> defaultValue, int prunning_threshold,String option) {
		Utility utility = new Utility();
		if (currentMatrix.size() < prunning_threshold) {
			/*
            if the number of objects is less than or equal 
            to the pruning value entered by the user
			 */
			Node tree = new Node();
			tree.distribution = defaultValue;
			tree.node_type = 1;
			tree.id = 0;
			//System.out.println("------Distribution-------");
			return tree;
		} else if (utility.find_same_class_labels_for_data_matrix(currentMatrix)) {     //if all have same class
			/*
	        	To check if all examples have same class
			 */
			int columnSize = currentMatrix.get(0).size() - 1;
			double classValue = currentMatrix.get(0).get(columnSize);
			Node tree = new Node();
			tree.classLable = (int) classValue;
			tree.node_type = 2;
			tree.id = 0;
			//System.out.println("---------Same Class--------------");
			return tree;
		} else {    //otherwise, in general  case

			Node tree = new Node();
			tree.id = 0;
			ArrayList<Double> returnValue = null;
			
			if(option.equals("optimized"))
				returnValue = utility.chooseAttributeOptimized(currentMatrix, attributes);
			else
				returnValue = utility.chooseAttributeRandomized(currentMatrix, attributes);
				
			
			//tree = DecisionTree(currentMatrix, attributes, defaultValue)
			//System.out.println(returnValue);
			tree.currentAttribute = returnValue.get(0);
			tree.threshold = returnValue.get(1);
			tree.entropy = returnValue.get(2);

			//            //System.out.println("currentAttribute "+returnValue.get(0));
			//            //System.out.println("threshold "+returnValue.get(1));

			ArrayList<ArrayList<Double>> leftChildMatrix = utility.splitLeftChildDataMatrix(currentMatrix, returnValue.get(0), returnValue.get(1));
			ArrayList<ArrayList<Double>> rightChildMatrix = utility.splitRightChildDataMatrix(currentMatrix, returnValue.get(0), returnValue.get(1));

			//System.out.println(returnValue);

			ArrayList<Double> default_value = null;
			if(leftChildMatrix.size()<prunning_threshold || rightChildMatrix.size()<prunning_threshold){
				default_value = utility.distribution(currentMatrix);
				//System.out.println(default_value);
				//System.out.println("Left " + leftChildMatrix.size());
				//System.out.println("Right " + rightChildMatrix.size());
			}


			tree.leftTree = DecisionTree(leftChildMatrix, attributes, default_value, prunning_threshold,option);
			tree.rightTree = DecisionTree(rightChildMatrix, attributes, default_value, prunning_threshold,option);

			return tree;
		}

	}

}
