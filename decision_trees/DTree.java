import java.util.ArrayList;

public class DTree {
	public static void main(String[] args) throws Exception {
		
		String training_file = args[0];
		String testing_file = args[1];
		String option = args[2];
		int pruning_th = Integer.valueOf(args[3]);
		
//		String training_file = "pendigits_training.txt";
//		String testing_file = "pendigits_test.txt";
//		String option = "forest3";
//		int pruning_th = 50;
		
		ReadFile readFile = new ReadFile();
		Utility utility = new Utility();
		Training training = new Training();
		ArrayList<Node> decisionTrees = new ArrayList<Node>();
		
		ArrayList<ArrayList<Double>>  training_data = readFile.readFile(training_file);
		int dimensions = training_data.get(0).size()-1;
		
		ArrayList<Double> attributes = new ArrayList<Double>();
		for(int i =0;i<dimensions;i++){
			attributes.add((double)i);
		}
		
		if(option.equals("optimized") || option.equals("randomized")){
			Node node = training.DecisionTree(training_data, attributes, null, pruning_th,option);
			ArrayList<Node> nodes = utility.print_tree_details(node,0);
			decisionTrees.add(node);
		}if(option.equals("forest3")){
			for (int i = 0; i < 3; i++) {
				Node node = training.DecisionTree(training_data, attributes, null, pruning_th,option);
				utility.print_tree_details(node,i);
				decisionTrees.add(node);
			}
		}if(option.equals("forest15")){
			for (int i = 0; i < 15; i++) {
				Node node = training. DecisionTree(training_data, attributes, null, pruning_th,option);
				utility.print_tree_details(node,i);
				decisionTrees.add(node);
			}
		}
				
		//Testing
		ArrayList<ArrayList<Double>>  testing_data = readFile.readFile(testing_file);

		//Classification
		Classification cls = new Classification();
		cls.classify(decisionTrees, testing_data);
		
	}
}
