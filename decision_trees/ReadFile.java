import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Scanner;


public class ReadFile {
	public ArrayList<ArrayList<Double>> readFile(String training_file) throws FileNotFoundException{
		ArrayList<Double> target = new ArrayList<Double>();
		ArrayList<ArrayList<Double>> dataMatrix = new ArrayList<ArrayList<Double>>();
		Scanner input = new Scanner(new File(training_file));

		while (input.hasNextLine()) {
			Scanner colReader = new Scanner(input.nextLine());
			ArrayList<Double> row = new ArrayList<Double>();
			while (colReader.hasNextDouble()) {
				row.add(colReader.nextDouble());
			}
			dataMatrix.add(row);
			target.add(row.get(row.size() - 1));

		}
		
		return dataMatrix;
	}
}
