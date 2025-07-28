SCC 211 Java threads Course Work 

The aim of this exercise to make students familiar with basic concepts in concurrency, such as threads, critical sections, 
and race conditions. As a practical matter, the exercise will be done in Java and will make students familiar with the use 
of Java threads and synchronization via joining threads and the synchronized keyword in Java.


The InventoryMain.java file contains the source code for inventory manager program using synchronized java threads. The 
program takes in two arguments from command line (first argument added items to inventory and second argument removed items 
from the inventory) it then creates the same number of threads to add and remove items from the 'WarehouseStock' counter. 
Once changes are made output the final stock value of the inventory. The inventory is initially set to zero at start of the 
program.