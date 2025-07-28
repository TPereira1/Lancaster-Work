Database Application Project
Overview
This project is a collaborative database application built using Java, JDBC, and MySQL. Each group member contributes individually by providing:
A CSV file containing data, DDL scripts to define the database structure, DML scripts to query the data. The Java application processes each 
student's components one at a time, automating the creation, population, and querying of databases.

Features
For each student in the group, the application:
1) Connects to the MySQL server using JDBC.
2) Executes DDL scripts to:
  + Create a new database.
  + Define tables.
3) Reads the student's CSV file and populates the tables with data.
4) Prints a confirmation message after successful population.
5) Executes DML scripts to query the database.
6) Displays the query results on the console.
This system allows for modular contributions and seamless integration of individual work into a single application.

Project Structure
/project-root
│
├── /src                  # Java source files
│   └── code.java         # Main application logic
│
├── /resources
│   ├── 38541181.csv      # CSV data files
│   ├── 38703238.csv
│   └── 38866366.csv
│
├── /scripts
│   ├── student1_ddl.sql  # DDL scripts
│   ├── student1_dml.sql  # DML scripts
│   ├── student2_ddl.sql
│   └── ...
│
└── README.md             # Project documentation

Prerequisites
+ Java (JDK 8 or higher)
+ MySQL Server
+ JDBC driver for MySQL

Setup Instructions
1) Clone the repository.
2) Add your own CSV, DDL, and DML files under the appropriate folders.
3) Configure MySQL credentials in the Java application.
4) Compile and run the application using your preferred IDE or command line.

Notes
+ Ensure each student's scripts and data are properly named and formatted.
+ All operations are handled sequentially for clarity and traceability.

License
This project is for educational purposes and follows your institution's guidelines.
