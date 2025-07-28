// Test Program 2: Calculating an average grade

public class TestProgram2 {
    public static void main( String[] args ) {
        int myMathsGrade = 88;
        int myEnglishGrade = 61;
        int myPhysicsGrade = 70;

        // calculate the average as part of the if condition
        if ( (myMathsGrade + myEnglishGrade + myPhysicsGrade) /3 > 75 ){
            System.out.println("Congratulations - you're a genius!\n" + "You got an average over 75%");
        }
        System.out.println("The average of your grades was: " + (myMathsGrade + myEnglishGrade + myPhysicsGrade) /3);
    }
}