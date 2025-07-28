public interface CollectionTest {
    public enum CollectionType {
        LINKED_LIST,
        ARRAY_LIST,
        HASH_MAP
    }

    public enum TestType {
        ADD,
        INDEX,
        SEARCH
    }

    /**
     * Defines the size of the collections that should be created when the ADD test
     * is run.
     *
     * @param size the number of unique items to add to each collection.
     */
    public void setSize(int size);

    /**
     * Performs the requested test on the requested collection type.
     *
     * ADD test should create a NEW collection of the given type, and populate it
     * with "size" *unique* instances of a Person (as defined by a previous call to
     * setSize).
     * INDEX test should retrieve an item from the collection based on its index in
     * that collection. The position should be approximately half-way through the
     * collection. (e.g. the 42nd item in a collection of size 84)
     * SEARCH test should retrieve an item from the collection based only the
     * Person's name (e.g. the Person with the name "Person1234"). An item should be
     * chosen that you expect to be approximatley half way through the collection.
     *
     * @param type       the type of collection to be tested.
     * @param test       the type of test to be undertaken.
     * @param iterations the number of times to repeat the given test before
     *                   returning.
     */
    public void runTest(CollectionType type, TestType test, int iterations);
}