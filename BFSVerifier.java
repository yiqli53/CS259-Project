import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.*;


public class BFSVerifier {
	public static void main(String[] args) {
		Map<Integer, Integer> expected_map = new HashMap<>();
		Map<Integer, Integer> actual_map = new HashMap<>();
		String expected = args[0];
		String actual = args[1];
		try {
			File myObj = new File(expected);
			Scanner myReader = new Scanner(myObj);
			while (myReader.hasNextLine()) {
				String data = myReader.nextLine();
				String[] parts = data.split(":");
				expected_map.put(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
			}
			myReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
		
		try {
			File myObj = new File(actual);
			Scanner myReader = new Scanner(myObj);
			while (myReader.hasNextLine()) {
				String data = myReader.nextLine();
				String[] parts = data.split(":");
				actual_map.put(Integer.parseInt(parts[0]), Integer.parseInt(parts[1]));
			}
			myReader.close();
		} catch (FileNotFoundException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
		
		int num_err = 0;
		
		for(Map.Entry<Integer,Integer> entry : expected_map.entrySet()) {
			if(actual_map.get(entry.getKey()) != entry.getValue()) num_err++;
		}
		
		if(num_err == 0) System.out.println("PASS!");
		else System.out.println("FAIL with " + num_err + " errors");
	}
}