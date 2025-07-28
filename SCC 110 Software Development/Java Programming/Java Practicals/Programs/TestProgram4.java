//Test Program 4: Summing the numbers 1..10
public class TestProgram4 {
    public static void main(String[] args) {
        int TOP_NUMBER = 10;
        int i=0;
        int total=0; 
        // work out the sum of the numbers from 1 to TOP_NUMBER // i.e. 1 + 2 + 3 + ... + 10
        for (i=1; i<=TOP_NUMBER; i++){
                total = total + i;
        }
        System.out.println("The sum of the numbers 1 to " + TOP_NUMBER + " was " + total);
    }
}