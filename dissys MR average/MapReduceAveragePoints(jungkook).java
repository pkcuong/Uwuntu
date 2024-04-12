import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

public class MapReduceAveragePoints {

    // Map function
    public static class MapFunction {
        public void map(String line, Map<String, Double> output) {
            String[] tokens = line.split(",");
            if (tokens.length == 2) {
                String subject = tokens[0].trim();
                double points = Double.parseDouble(tokens[1]);
                output.put(subject, points);
            }
        }
        
    }

    // Reduce function
    public static class ReduceFunction {
        public void reduce(Map<String, Double> input) {
            Map<String, Double> sum = new HashMap<>();
            Map<String, Integer> count = new HashMap<>();

            // Aggregate points and count for each subject
            for (Map.Entry<String, Double> entry : input.entrySet()) {
                String subject = entry.getKey();
                double points = entry.getValue();

                sum.put(subject, sum.getOrDefault(subject, 0.0) + points);
                count.put(subject, count.getOrDefault(subject, 0) + 1);
            }

            // Calculate average points for each subject and print
            for (Map.Entry<String, Double> entry : sum.entrySet()) {
                String subject = entry.getKey();
                double totalPoints = entry.getValue();
                int totalCount = count.get(subject);
                double average = totalPoints / totalCount;
                System.out.println("Subject: " + subject + ", Average Points: " + average);
            }
        }
    }

    public static void main(String[] args) throws IOException {
        Map<String, Double> input = new HashMap<>();

        // Read data from CSV file
        try (BufferedReader br = new BufferedReader(new FileReader("input.txt"))) {
            String line;
            while ((line = br.readLine()) != null) {
                new MapFunction().map(line, input);
            }
        }

        // Reduce phase
        new ReduceFunction().reduce(input);
    }
}
