import java.lang.reflect.*;
import java.util.*;
import java.io.*;

public class PerformanceVisualizer {
    boolean debug = false;

    HashMap<String, Long> dataSet = new HashMap<>();
    int minimum;
    int maximum;
    int step;
    int numberOfTests;

    public void debugMessage(String s) {
        if (debug)
            System.out.println("PerformanceVisualizer: " + s);
    }

    public PerformanceVisualizer(String className, int minimum, int maximum, int numberOfTests, int iterations) {
        this.debugMessage("Testing class " + className);

        // Orders of magnitude of iterations to test (start at 10^1)
        this.minimum = minimum;
        this.maximum = maximum;
        this.numberOfTests = numberOfTests;
        step = (maximum - minimum) / numberOfTests;

        try {

            Constructor<?> constructor = Class.forName(className).getConstructor();

            for (int range = 0; range < numberOfTests; range++) {
                int size = minimum + range * step;

                CollectionTest test = (CollectionTest) constructor.newInstance();
                test.setSize(size);

                for (CollectionTest.CollectionType collectionType : CollectionTest.CollectionType.values()) {
                    for (CollectionTest.TestType testType : CollectionTest.TestType.values()) {
                        this.debugMessage("[" + collectionType.name() + "] [" + testType.name() + "] [size: " + size
                                + "] [Iterations: " + iterations + "]");

                        long start = Calendar.getInstance().getTimeInMillis();
                        test.runTest(collectionType, testType, iterations);
                        long end = Calendar.getInstance().getTimeInMillis();
                        long period = end - start;

                        String key = collectionType.name() + "_" + testType.name() + "_" + size;
                        long value = period;

                        dataSet.put(key, value);
                        debugMessage("Created Entry: [KEY: " + key + "] [value: " + value + "]");
                    }
                }
            }

            this.writeHTML("results.html");
        } catch (ClassNotFoundException e) {
            System.out.println("*** Could not find class '" + className + "' ***");
            System.exit(0);
        } catch (Exception e) {
            System.out.println("*** Could not access class '" + className + "' ***");
            System.out.println(
                    "  Verify this class has a default constructor, and implements the CollectionTest interface.");
            System.exit(0);
        }
    }

    public String jsDataList(CollectionTest.TestType testType) {
        String xValues = "[";
        String[] lineColours = { "red", "green", "blue" };
        int lineNumber = 0;
        String html = "<H2>" + "CollectionTest: " + testType.name() + "</H2><BR/><canvas id=\"" + testType.name()
                + "\" style=\"width:100%;max-width:700px\"></canvas> <BR/><BR/>";

        // populate the x axis
        for (int range = 0; range < numberOfTests; range++) {
            int size = minimum + range * step;

            if (range != 0)
                xValues = xValues + ",";

            xValues = xValues + size;
        }
        xValues = xValues + "]";

        // populate each of the y-axis data sets
        String js1 = "<script> new Chart(\"" + testType.name() + "\", {type: \"line\", data: {labels: " + xValues
                + ", datasets: [";
        String js2 = "";

        for (CollectionTest.CollectionType t : CollectionTest.CollectionType.values()) {
            String yValues = "{label: \"" + t.name() + "\", data: [";

            for (int range = 0; range < numberOfTests; range++) {
                int size = minimum + range * step;

                if (range != 0)
                    yValues = yValues + ",";

                String key = t.name() + "_" + testType.name() + "_" + size;
                Long value = dataSet.get(key);
                debugMessage("Accessed Entry: [KEY: " + key + "] [value: " + value + "]");

                yValues = yValues + value;

            }

            yValues = yValues + "],";

            if (lineNumber != 0)
                js2 += ",";

            js2 += yValues;
            js2 += "borderColor: \"" + lineColours[lineNumber] + "\", fill: false}";

            lineNumber++;
        }

        String js3 = "]}, options: { legend: {display: true}}});</script>";

        return html + js1 + js2 + js3;
    }

    public void writeHTML(String filename) {
        String htmlHeader = "<!DOCTYPE html><html><script src=\"https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.9.4/Chart.js\"></script><body>";
        String htmlFooter = "</body></html>";
        String html = "";

        html = htmlHeader;
        for (CollectionTest.TestType t : CollectionTest.TestType.values())
            html += jsDataList(t);

        html += htmlFooter;

        File f = new File(filename);
        f.delete();
        f = new File(filename);

        try {
            FileWriter fw = new FileWriter(f, false);
            fw.write(html);
            fw.close();
        } catch (IOException e) {
            debugMessage("Cannot write file. Exiting.");
            System.exit(0);
        }
    }

    public void setDebug(boolean d) {
        this.debug = d;
    }

    public static void usage() {
        System.out.println(
                "Usage:\n\njava PerformanceVisualizer <CLASS NAME> <MINIMUM SIZE> <MAXIMUM SIZE> <NUMBER OF TESTS> <ITERATIONS>");
        System.out.println(
                "    <CLASS NAME> is the name of a compiled class in this directory that implements the CollectionTest interface.");
        System.out.println("    <MINIMUM SIZE> is the smallest collection to create.");
        System.out.println("    <MAXIMUM SIZE> is the largest collection to create.");
        System.out.println(
                "    <NUMBER OF TESTS> is the number of tests to run, which will be evenly spaced between <MINIMUM SIZE> and <MAXIMUM SIZE>");
        System.out.println(
                "    <ITERATIONS> is the number of times each tests is repeated and averaged to produce a result\n\n");

        System.exit(0);
    }

    public static void main(String[] args) {
        if (args.length != 5)
            usage();

        try {
            String className = args[0];
            int minimum = Integer.parseInt(args[1]);
            int maximum = Integer.parseInt(args[2]);
            int numberOfTests = Integer.parseInt(args[3]);
            int iterations = Integer.parseInt(args[4]);
            new PerformanceVisualizer(className, minimum, maximum, numberOfTests, iterations);

        } catch (Exception e) {
            usage();
        }

    }
}