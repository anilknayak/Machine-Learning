import java.util.ArrayList;
import java.util.Collections;


public class Classification {
	public void classify(ArrayList<Node> decisionTrees,ArrayList<ArrayList<Double>>  testing_data){
		Utility utility = new Utility();
		
		
		ArrayList<Double> target_test = utility.getTargetClasses(testing_data);
		
		
		int correct_classification_count = 0;
		int number_of_record = testing_data.size();
		for (int object_id = 0; object_id < number_of_record; object_id++) {

			ArrayList<ArrayList<Double>> dists = new ArrayList<ArrayList<Double>>();
			double predicted_class;
			double actual_class = target_test.get(object_id);
			double accuracy = 0;

			for (Node node : decisionTrees) {
				ArrayList<Double> dist = classification(testing_data.get(object_id),node);
				dists.add(dist);
			} 

			if(dists.size()==1){
				ArrayList<Double> dist1 = dists.get(0);
				double max = Collections.max(dist1);
				//System.out.println(max);
				ArrayList<Integer> positions = utility.occurance_of_dist(dist1,max);
				//System.out.println(positions);
				int last_index = positions.size()-1;
				int count = positions.get(last_index);
				positions.remove(last_index);

				if(last_index>=2){
					int r = (int)(Math.random()*1000)%count;
					predicted_class = positions.get(r);
				}else{
					predicted_class = positions.get(0);
				}
			}else{
				int size_dist = dists.size();
				ArrayList<Double> sum_dist = new ArrayList<Double>();

				for (int i = 0; i < 10; i++) {
					sum_dist.add(0.0);
				}

				for (int i = 0; i < dists.size(); i++) {
					ArrayList<Double> dist_intr = dists.get(i);

					for (int j = 0; j < dist_intr.size(); j++) {
						double sum = sum_dist.get(j);
						sum = sum + dist_intr.get(j);
						sum_dist.set(j, sum);
					}
				}

				for (Double sum : sum_dist) {
					sum = sum / (double) size_dist;
				}

				double max = Collections.max(sum_dist);
				//System.out.println(max);
				ArrayList<Integer> positions = utility.occurance_of_dist(sum_dist,max);
				//System.out.println(positions);
				int last_index = positions.size()-1;
				int count = positions.get(last_index);
				positions.remove(last_index);

				if(last_index>=2){
					int r = (int)(Math.random()*1000)%count;
					predicted_class = positions.get(r);
				}else{
					predicted_class = positions.get(0);
				}


			}


			if(Double.compare(predicted_class, actual_class)==0){
				accuracy = 1;
				correct_classification_count++;
			}

			System.out.printf("ID=%5d, predicted=%3d, true=%3d, accuracy=%4.2f\n", object_id, (int)predicted_class, (int)actual_class, accuracy);

		}

		double classification_accuracy = correct_classification_count / (double) number_of_record;

		System.out.printf("classification accuracy=%6.4f\n", classification_accuracy);
	}

	public ArrayList<Double> classification(ArrayList<Double> row,Node node){

		ArrayList<Double> value;

		if(node.node_type==2){

			int rowSize = 10;

			ArrayList<Double> dist = new ArrayList<Double>();
			for (int i = 0; i < 10; i++) {
				dist.add(0.0);
			}

			dist.set(node.classLable, 1.0);
			return dist;
		}else if(node.node_type==1){
			ArrayList<Double> dist = node.distribution;
			return dist;
		}else{

			int attribute = (int)node.currentAttribute;		
			double threshold = (double)node.threshold;	

			double attribute_value = (double)row.get(attribute);

			Node child = null;
			if(Double.compare(attribute_value, threshold)==-1){
				child = node.leftTree;
			}else{
				child = node.rightTree;
			}

			value  = classification(row, child);
		}


		return value;

	}
}
