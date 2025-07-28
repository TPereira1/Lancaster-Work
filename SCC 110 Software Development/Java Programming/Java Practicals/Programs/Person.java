/**
 * this class used to create an instance of a person
 * contains only two attributes a person name and age
 */
public class Person {
    private String name = "John Doe";
    private int age = 0;

    public Person(String aName){
        this.name = aName;
        this.age = 0;
    }

    public String getName(){
        return(this.name);
    }

    public int getAge(){
        return(this.age);
    }

    public void Display(){
        System.out.println("This person is named: "+ this.name +" and their age is "+ this.age);
    } 

    public void Birthday(){
        this.age++;
    }

    public void changeName(String newName){
        this.name = newName;
    }
    
}