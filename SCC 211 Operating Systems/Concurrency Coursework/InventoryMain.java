import java.util.ArrayList;
/**
 * SCC 211 Java thread coursework - Warehouse stock management 
 * @author Thomas Pereira
 */
public class InventoryMain {
    /**
     * This is a counter that record the stock held in the warehouse. Set to zero at the start.
     */
    private static int WarehouseStock = 0;
    /**
     * A variables that holds the number of stock items needed to be add to the warehouse.
     */
	static int add;
    /**
     * A variables hold the number of stock needed to be removed from the warehouse.
     */
	static int remove;
	
    /**
     * Method used to increment warehouse stock by 1, while also blocking access to other threads to WarehouseStock during it execution.
     */
	private synchronized static void add() {
		WarehouseStock++;
		System.out.println("Added. Inventory size = "+ WarehouseStock);
	}
	
    /**
     * Method used to decrement warehouse stock by 1, while also blocking access to other thread to WarehouseStock during it execution.
     */
	private synchronized static void remove() {
		WarehouseStock--;
		System.out.println("Removed. Inventory size = "+ WarehouseStock);
	}

    /**
     * Program Main where the add and remove threads are generated and run from, depending on arguments passed into the program on execution.
     */
	public static void main(String[] args) throws Exception {
		ArrayList<Thread> threads = new ArrayList<Thread>();
		add = Integer.parseInt(args[0]);
		remove = Integer.parseInt(args[1]);
		
		for(int i = 0; i < add; i++) {
			Thread t = new Thread(new Runnable() {
				public void run() {
					add();
				}
			});
			threads.add(t);
		}
		for(int i = 0; i < remove; i++) {
			Thread t2 = new Thread(new Runnable() {
				public void run() {
					remove();
				}
			});
			threads.add(t2);
		}
		for(Thread t: threads) {
			t.start();
		}
		for(Thread t: threads) {
			t.join();
		}
		System.out.println("Final inventory size = "+WarehouseStock);

	}

}