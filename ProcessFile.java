import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.util.Scanner;
import java.io.IOException;

public class ProcessFile {
	public static void main(String[] args) {
		try {
			String filename = "soc-LiveJournal1.txt";
			File myObj = new File(filename);
			FileWriter myWriter = new FileWriter("graph.txt");
			Scanner myReader = new Scanner(myObj);
			while (myReader.hasNextLine()) {
				String data = myReader.nextLine();
				if(data.charAt(0) == '#') continue;
				String newdata = data.replace("\t", " ");
				myWriter.write(newdata + "\n");
			}
			myReader.close();
			myWriter.close();
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
	}
}
