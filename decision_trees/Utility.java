import java.util.ArrayList;
import java.util.Collections;


public class Utility {
	public ArrayList<Integer> occurance_of_dist(ArrayList<Double> dist,Double max){
		int count = 0;
		ArrayList<Integer> positions = new ArrayList<Integer>();
		for (int i = 0; i < dist.size(); i++) {
			Double array_element = dist.get(i);
			if(Double.compare(array_element, max)==0){
				count++;
				positions.add(i);
			}
		}
		positions.add(count);
		return positions;
	}


	public ArrayList<Node> print_tree_details(Node node,int tree_no){
		ArrayList<Node> nodes = new ArrayList<Node>();
		node.id = 1;
		nodes.add(node);
		print_tree(nodes,tree_no);
		return nodes;
	}

	public void print_tree(ArrayList<Node> nodes,int tree_no){

		ArrayList<Node> node_at_level = new ArrayList<Node>();

		for (Node node : nodes) {

			int feature = (int)node.currentAttribute;
			double threshold = node.threshold;
			double gain = node.entropy;
			if(node.node_type==1 || node.node_type==2){
				feature = -1;
				threshold = -1;
				gain = -1;
			}

			System.out.printf("tree=%2d, node=%3d, feature=%2d, thr=%6.2f, gain=%f\n", tree_no, node.id, feature,threshold , gain);

			if(node.node_type==0){
				node.leftTree.id = 2*node.id;
				node_at_level.add(node.leftTree);
				node.rightTree.id = 2*node.id+1;
				node_at_level.add(node.rightTree);
			}
		}

		if(node_at_level.size()!=0)
			print_tree(node_at_level,tree_no);
	}

	public ArrayList<Double> select_values_of_attribute(ArrayList<ArrayList<Double>> currentMatrix,int attribute){
		ArrayList<Double> attributeValues = new ArrayList<Double>();
		for (int p = 0; p < currentMatrix.size(); p++) {
			attributeValues.add(currentMatrix.get(p).get(attribute));
		}
		return attributeValues;
	}


	public ArrayList<Double> chooseAttributeOptimized(ArrayList<ArrayList<Double>> currentMatrix,
			ArrayList<Double> attributes) {

		double maxGain = -1;
		double bestAttribute = -1;
		double bestThreshold = -1;
		double currentEntropy = calculateEntropy(currentMatrix);

		for (int attribute = 0; attribute < attributes.size(); attribute++) {


			ArrayList<Double> attributeValues = select_values_of_attribute(currentMatrix, (int)attribute);

			double min = Collections.min(attributeValues);
			double max = Collections.max(attributeValues);

			for (int k = 1; k <= 50; k++) {

				double threshold = min + k * (max - min) / 51;
				double gain = calculateInformationGain(currentMatrix, attribute, threshold, currentEntropy);
				////System.out.println("Best Attribute selection for  attr "+attribute+" threshold "+threshold +" gain"+gain);
				if (gain > maxGain) {
					maxGain = gain;
					bestAttribute = attribute;
					bestThreshold = threshold;
				}
			}
		}
		ArrayList<Double> choosed_attribute = new ArrayList<Double>();
		choosed_attribute.add(bestAttribute);
		choosed_attribute.add(bestThreshold);
		choosed_attribute.add(maxGain);

		return choosed_attribute;
	}

	public ArrayList<Double> chooseAttributeRandomized(ArrayList<ArrayList<Double>> currentMatrix,
			ArrayList<Double> attributes) {
		Utility utility = new Utility();
		double maxGain = -1, bestAttribute = -1, bestThreshold = -1;
		double currentEntropy = utility.calculateEntropy(currentMatrix);

		int bestAttribute1 = (int)(Math.random()*1000)%10;
		bestAttribute = (double) bestAttribute1;

		ArrayList<Double> attributeValues = utility.select_values_of_attribute(currentMatrix, bestAttribute1);

		double min = Collections.min(attributeValues);
		double max = Collections.max(attributeValues);

		for (int k = 1; k <= 50; k++) {

			double threshold = min + k * (max - min) / 51;

			double gain = utility.calculateInformationGain(currentMatrix, bestAttribute1, threshold, currentEntropy);
			if (gain > maxGain) {
				maxGain = gain;
				bestThreshold = threshold;
			}
		}

		ArrayList<Double> choosed_attribute = new ArrayList<Double>();
		choosed_attribute.add(bestAttribute);
		choosed_attribute.add(bestThreshold);
		choosed_attribute.add(maxGain);


		return choosed_attribute;
	}

	
	public ArrayList<Double> getTargetClasses(ArrayList<ArrayList<Double>>  testing_data){
		ArrayList<Double> target = new ArrayList<Double>();
		
		int dimensions = testing_data.get(0).size();
		for (int i = 0; i < testing_data.size(); i++) {
			target.add(testing_data.get(i).get(dimensions-1));
		}
		
		return target;
	}


	public double calculateInformationGain(ArrayList<ArrayList<Double>> currentMatrix, int currentAttribute,
			double threshold, double parentEntropy) {

		double infromation_gain;

		ArrayList<ArrayList<Double>> left_child_data = splitLeftChildDataMatrix(currentMatrix,(double) currentAttribute, threshold);
		ArrayList<ArrayList<Double>> right_child_data = splitRightChildDataMatrix(currentMatrix,(double) currentAttribute, threshold);

		double lessThanMatrixEntropy = calculateEntropy(left_child_data);
		double greaterThanMatrixEntropy = calculateEntropy(right_child_data);

		infromation_gain = parentEntropy - (lessThanMatrixEntropy*(left_child_data.size()/(double)currentMatrix.size()) + greaterThanMatrixEntropy*(right_child_data.size()/(double)currentMatrix.size()));

		return infromation_gain;
	}


	public boolean find_same_class_labels_for_data_matrix(ArrayList<ArrayList<Double>> currentMatrix){
		boolean check = true;
		int columnSize = currentMatrix.get(0).size() - 1;
		double classValue = currentMatrix.get(0).get(columnSize);
		for (int i = 0; i < currentMatrix.size(); i++) {
			if (currentMatrix.get(i).get(columnSize) != classValue) {
				check = false;  
				break;
			}
		}

		return check;
	}


	public ArrayList<ArrayList<Double>> splitLeftChildDataMatrix(ArrayList<ArrayList<Double>> currentMatrix,
			double bestAttribute, double bestThreshold) {

		int newBestAttribute = (int) bestAttribute;
		ArrayList<ArrayList<Double>> returnMatrix = new ArrayList<ArrayList<Double>>();
		for (int i = 0; i < currentMatrix.size(); i++) {
			if (currentMatrix.get(i).get(newBestAttribute) < bestThreshold) {
				returnMatrix.add(currentMatrix.get(i));
			}
		}
		return returnMatrix;
	}


	public ArrayList<ArrayList<Double>> splitRightChildDataMatrix(ArrayList<ArrayList<Double>> currentMatrix,
			double bestAttribute, double bestThreshold) {

		int newBestAttribute = (int) bestAttribute;
		ArrayList<ArrayList<Double>> returnMatrix = new ArrayList<ArrayList<Double>>();
		for (int i = 0; i < currentMatrix.size(); i++) {
			if (currentMatrix.get(i).get(newBestAttribute) >= bestThreshold) {
				returnMatrix.add(currentMatrix.get(i));
			}
		}
		return returnMatrix;
	}

	public ArrayList<Double> distribution(ArrayList<ArrayList<Double>> currentMatrix) {

		ArrayList<Double> returnMatrix = new ArrayList<Double>();

		int classesCount[] = find_class_label_count(currentMatrix);

		for (int i : classesCount) {
			double finalValue = i / (double)currentMatrix.size();
			returnMatrix.add(finalValue);
		}

		return returnMatrix;
	}

	public int[] find_class_label_count(ArrayList<ArrayList<Double>> currentMatrix){
		int classesCount[] = new int[10];
		int columnSize = currentMatrix.get(0).size() - 1;
		for (int i = 0; i < currentMatrix.size(); i++) {
			double currentClass = currentMatrix.get(i).get(columnSize);
			int myCurrentClass = (int) currentClass;
			classesCount[myCurrentClass]++;
		}
		return classesCount;

	}


	public double calculateEntropy(ArrayList<ArrayList<Double>> currentMatrix) {

		double gain = 0.0;
		if(currentMatrix.size()!=0){
			int classesCount[] = find_class_label_count(currentMatrix);

			int current_mat_size = currentMatrix.size();

			for (int i : classesCount) {
				double finalValue = i /(double)current_mat_size; 

				if(finalValue!=0.0)
					gain = gain + (-1.0) * finalValue * (Math.log(finalValue));
			}
		}

		return gain;
	}

}
