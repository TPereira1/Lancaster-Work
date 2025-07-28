public class month {

    public static void main(String[] args){
        int month = 5;
        if(((month%2) == 1 && month <= 7) || ((month%2) == 0 && month > 7)){
            System.out.println("The month has 31 days.");
        }
        else if(month == 2){
            System.out.println("The month has 28 days or 29 day if leap year.");
        }
        else{
            System.out.println("The month has 30 days.");
        }
    }
}
