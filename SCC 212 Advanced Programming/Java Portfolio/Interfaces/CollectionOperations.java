import java.util.*;

/**
 * This class provides a test program for all the different type of collection tests specified under the CollectionTest class.
 * @author Thomas Pereira, Student ID:38866366
 */
public class CollectionOperations implements CollectionTest {
	/**This is the size of the collections that should be created when the ADD test is run.**/
    private int size = 0;
    /**This is an ArrayList in which the tests will be run on**/
    private ArrayList<Person> array;
    /**This is an LinkedList in which the tests will be run on**/
    private LinkedList<Person> linked;
    /**This is an HashMap in which the tests will be run on**/
    private HashMap<Integer, Person> hash;
    /**
     * This will hold the hash code of the middle element in the hash map, used for searching the hash map and set by indexing for the middle element
     */
    private Integer midHash = 0;

    /**
     * Default constructor
     */
    public CollectionOperations() {
    }

    /**
     * Defines the size of the collections that should be created when the ADD test is run.
     * @param the number of unique items to add to each collection.
     */
    @Override
    public void setSize(int size) {
        this.size = size;
    }

    /**
     * Performs the requested test on the requested collection type, makes use of a switch statement calling upon the relevant methods for each type of test for each test iteration.
     * @param type the type of collection to be tested.
     * @param test the type of test to be undertaken.
     * @param iterations the number of times to repeat the given test before returning.
     */
	@Override
	public void runTest(CollectionType type, TestType test, int iterations) {
		for (int i = 0; i < iterations; i++) {
			switch (test) {
				case ADD:
					add(type);
					break;
				case INDEX:
					index(type);
					break;
				case SEARCH:
					search(type);
					break;
				default:
					break;
			}
		}
	}
	/**
	 * Creates new instances of each collection then populates it with a 'size' unique instances of Person object.
	 * @param type of collection the test will be performed on.
	 */
    public void add(CollectionType type) {
    	switch (type) {
    		case ARRAY_LIST:
    			array = new ArrayList<Person>();
    			for (int x = 0; x < size; x++) {
    				array.add(new Person("Person" + x, x));
                }
    			break;
    		case LINKED_LIST:
    			linked = new LinkedList<Person>();
    			for (int x = 0; x < size; x++) {
    					linked.add(new Person("Person" +  x, x));
    			}
    			break;
    		case HASH_MAP:
               	hash = new HashMap<Integer, Person>();
               	for (int x = 0; x < size; x++) {
               		Person a = new Person("Person" +  x, x);
           			hash.put(a.hashCode(), a);
           		}
               	break;
    	}
    }

    /**
     * Gets the instance of Person by their index position in a collection. Approximately the middle index chosen each time.
     * @param type of collection the test will be performed on.
     */
    public void index(CollectionType type) {
        switch (type) {
            case ARRAY_LIST:
                array.get(array.size() / 2);
                break;
            case LINKED_LIST:
                linked.get(linked.size() / 2);
                break;
            case HASH_MAP:
                int count = 0;
                int index = hash.size() / 2;
                for (Map.Entry<Integer, Person> set : hash.entrySet()) {
                    if (count == index) {
                        midHash = set.getKey();
                        break;
                    }
                    count++;
                }
        }
    }

    /**
     * Performs a search on each collection using a name of a person who is approximately in the middle of the collection.
     * @param type of collection the test will be performed on.
     */
    public void search(CollectionType type) {
        Person a;
        switch (type) {
            case ARRAY_LIST:
                a = array.get((int) array.size() / 2);
                for (int y = 0; y < array.size(); y++) {
                    if (a.equals(array.get(y))) {
                        break;
                    }
                }
                break;
            case LINKED_LIST:
                a = linked.get((int) linked.size() / 2);
                Iterator<Person> n = linked.iterator();
                while (n.hasNext()) {
                    if (n.next().equals(a)) {
                        break;
                    }
                }
                break;
            case HASH_MAP:
                if (midHash == 0) {
                    midHash = ("Person" + hash.size() / 2).hashCode();
                }
                hash.get(midHash);
                break;
        }
    }

}
