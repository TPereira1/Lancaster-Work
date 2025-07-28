public class Person {
    private int age;
    private String name;

    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public int getAge() {
        return age;
    }

    public boolean equals(Object o) {
        if (o == null || !(o instanceof Person))
            return false;

        Person p = (Person) o;

        if (p.name == this.name)
            return true;

        if (p.name != null)
            return p.name.equals(this.name);

        return false;
    }

    public String toString() {
        return "Person [name: " + name + "] [age: " + age + "]";
    }

    public int hashCode() {
        if (name != null)
            return name.hashCode();
        else
            return 0;
    }
}