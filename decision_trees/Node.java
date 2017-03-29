import java.util.ArrayList;

public class Node {
	public double currentAttribute;
	public double threshold;

	public Node leftTree;
	public Node rightTree;
	public ArrayList<Double> distribution = new ArrayList<Double>();
	public int classLable;

	public double entropy;
	public int node_type;
	
	public int id=0;
}