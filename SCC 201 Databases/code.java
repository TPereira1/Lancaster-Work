import java.io.File;
import java.io.FileNotFoundException;
import java.sql.*;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.util.stream.Collectors;

public class code {
    // run these lines to compile and run:
    // - javac code.java
    // - java -cp ".:/usr/share/java/mariadb-java-client.jar:" code.java

    public static void main(String[] args) {
        new Nishant();
        //new Jamie();
        new Thomas();
    }
}

class Thomas {// student 38866366
    public Thomas() {
        setupSciFiDatabase();
        try {
            populateDatabase();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
        runDMLQueries();
    }

    private static void runDDLQuery(String[] statements) {
        Connection connection = null;
        Statement statement = null;

        try {
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/");
            statement = connection.createStatement();

            for (String ddlStatement : statements) {
                statement.executeUpdate(ddlStatement);
            }

        } catch (SQLException e) {
            System.out.println("Error: SQL Exception - " + e.getMessage());
        } finally {
            try {
                if (statement != null) {
                    statement.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close statement - " + e.getMessage());
            }

            try {
                if (connection != null) {
                    connection.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close connection - " + e.getMessage());
            }
        }
    }

    private static void setupSciFiDatabase() {
        String[] ddlStatements = {
                "CREATE DATABASE SciFi",
                "USE SciFi",
                "CREATE TABLE IF NOT EXISTS SciFi_Movies(Name VARCHAR(100) NOT NULL PRIMARY KEY, Duration INT, Release_date DATE, Budget INT, Rating FLOAT, Plot TEXT)",
                "CREATE TABLE IF NOT EXISTS Movie_Staff(StaffID INT NOT NULL, Name TEXT NOT NULL, DOB DATE, JobTitle Text, AvgSalary INT, Phone VARCHAR(20), Email TEXT, PRIMARY KEY (StaffID))",
                "CREATE TABLE IF NOT EXISTS Characters(FilmID VARCHAR(100) NOT NULL, CharID INT NOT NULL, CharName TEXT NOT NULL, ActorID INT, Appearance TEXT, Background TEXT, FOREIGN KEY (FilmID) REFERENCES SciFi_Movies(Name), PRIMARY KEY (FilmID,CharID), FOREIGN KEY (ActorID) REFERENCES Movie_Staff(StaffID))",
                "CREATE TABLE IF NOT EXISTS Segments(FilmID VARCHAR(100) NOT NULL, Number INT NOT NULL, Description TEXT, Setting TEXT, Duration INT, FOREIGN KEY (FilmID) REFERENCES SciFi_Movies(Name), PRIMARY KEY (FilmID,Number))",
                "CREATE TABLE IF NOT EXISTS Works_On(FilmID VARCHAR(100) NOT NULL, StaffID INT NOT NULL, FOREIGN KEY (FilmID) REFERENCES SciFi_Movies(Name), FOREIGN KEY (StaffID) REFERENCES Movie_Staff(StaffID), PRIMARY KEY (FilmID, StaffID))",
                "CREATE TABLE IF NOT EXISTS Part_In(FilmID varchar(100) NOT NULL, SegmentNum INT NOT NULL, CharID INT NOT NULL, FOREIGN KEY (FilmID, CharID) REFERENCES Characters(FilmID,CharID), FOREIGN KEY (FilmID, SegmentNum) REFERENCES Segments(FilmID,Number))"
        };
        runDDLQuery(ddlStatements);
        System.out.println("\nDatabase for Student 38866366 has been created...\n");
    }

    private static List<String> runDMLQuery(String[] queries) {
        List<String> results = new ArrayList<>();
        Connection connection = null;
        Statement statement = null;

        try {
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/");
            statement = connection.createStatement();

            for (String sqlQuery : queries) {
                ResultSet rs = statement.executeQuery(sqlQuery);

                while (rs.next()) {
                    for (int i = 1; i <= rs.getMetaData().getColumnCount(); i++) {
                        results.add(rs.getString(i));
                        if (i < rs.getMetaData().getColumnCount()) {
                            results.add(", ");
                        }
                    }
                }
            }
        } catch (SQLIntegrityConstraintViolationException e) {
            results.add("Failed: Constraint violation - " + e.getMessage());
        } catch (SQLException e) {
            results.add("Error: SQL Exception - " + e.getMessage());
        } finally {

            try {
                if (connection != null) {
                    connection.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close connection - " + e.getMessage());
            }
        }
        return results;
    }

    private static void populateDatabase() throws FileNotFoundException {
        Scanner scanner = new Scanner(new File("38866366.csv"));
        scanner.useDelimiter(",");
        scanner.nextLine();
        while (scanner.hasNextLine()) {
            String[] lineData = scanner.nextLine().split(",");
            if (!lineData[0].equals("null")) {
                String[] ddlStatements = { "USE SciFi",
                        "INSERT INTO SciFi_Movies (Name, Duration, Rating, Release_date, Plot, Budget) VALUES(\""
                                + lineData[0] + "\"," + lineData[1] + "," + lineData[2] + ",'" + lineData[3] + "',\""
                                + lineData[4] + "\"," + lineData[5] + ")" };
                runDDLQuery(ddlStatements);
            }
            if (!lineData[6].equals("null")) {
                String[] ddlStatements = { "USE SciFi",
                        "INSERT INTO Movie_Staff (StaffID, Name, JobTitle, AvgSalary, DOB, EMAIL, Phone) VALUES("
                                + lineData[6] + ",\"" + lineData[7] + "\",\"" + lineData[8] + "\"," + lineData[9]
                                + ",\"" + lineData[10] + "\",\"" + lineData[11] + "\",\"" + lineData[12] + "\")" };
                runDDLQuery(ddlStatements);
            }
            if (!lineData[13].equals("null")) {
                String[] ddlStatements = { "USE SciFi",
                        "INSERT INTO Characters (FilmID, CharID, CharName, ActorID, Appearance, Background) VALUES ('"
                                + lineData[13] + "', " + lineData[14] + ", '" + lineData[15] + "', " + lineData[18]
                                + ", '" + lineData[16] + "', '" + lineData[17] + "')",
                        "INSERT INTO Works_On (FilmID, StaffID) VALUES (\"" + lineData[13] + "\"," + lineData[18]
                                + ")" };
                runDDLQuery(ddlStatements);
            }
            if (!lineData[19].equals("null")) {
                String[] ddlStatements = { "USE SciFi", "INSERT INTO Works_On (FilmID, StaffID) VALUES (\""
                        + lineData[19] + "\"," + lineData[20] + ")" };
                runDDLQuery(ddlStatements);
            }
            if (!lineData[21].equals("null")) {
                String[] ddlStatements = { "USE SciFi",
                        "INSERT INTO Segments (FilmID, Number, Duration, Setting, Description) VALUES('" + lineData[21]
                                + "'," + lineData[22] + "," + lineData[23] + ",'" + lineData[24] + "','" + lineData[25]
                                + "')" };
                runDDLQuery(ddlStatements);
            }
            if (!lineData[26].equals("null")) {
                String[] ddlStatements = { "USE SciFi", "INSERT INTO Part_In (FilmID, SegmentNum, CharID) VALUES('"
                        + lineData[26] + "'," + lineData[27] + "," + lineData[28] + ")" };
                runDDLQuery(ddlStatements);
            }
        }
        scanner.close();
        System.out.println("Database for Student 38866366 has been populated...\n");
    }

    private static void runDMLQueries() {
        String[] query1 = { "USE SciFi", "DELETE FROM SciFi_Movies WHERE Name = \"Wonder Woman\"" };
        System.out.println("Query 1 " + runDMLQuery(query1) + "\n");

        String[] query2 = { "USE SciFi", "DELETE FROM Movie_Staff WHERE Name = \"Charles Haney\"" };
        System.out.println("Query 2 " + runDMLQuery(query2) + "\n");

        String[] query3 = { "USE SciFi", "SELECT FilmID FROM Segments GROUP BY FilmID HAVING COUNT(FilmID) > 8" };
        System.out.println(
                "Query 3 The Movie with more then 8 segments are " + runDMLQuery(query3) + "\n");

        String[] query4 = { "USE SciFi",
                "SELECT FilmID FROM Characters WHERE FilmID LIKE \"Shark%\" GROUP BY FilmID HAVING COUNT(*) < 8" };
        System.out.println("Query 4 The film with less then 8 characters and has shark in it's name is "
                + runDMLQuery(query4) + "\n");

        String[] query5 = { "USE SciFi",
                "SELECT Name FROM SciFi_Movies WHERE Budget > 100000 AND Release_date BETWEEN '2019-01-01' AND '2019-12-31'" };
        System.out.println("Query 5 The name of the movie released in 2019 with a budget larger then 10,000 is "
                + runDMLQuery(query5) + "\n");

        String[] query6 = { "USE SciFi",
                " SELECT Count(*) FROM Works_On WHERE FilmID = 'Frankenstein The True Story' Group BY FilmID" };
        System.out.println(
                "Query 6 In the movie Frankenstein The True Story there were " + runDMLQuery(query6)
                        + " members in the cast and crew.\n");

        String[] query7 = { "USE SciFi",
                "SELECT Characters.CharName AS 'Character Name', Movie_Staff.Name AS Actor FROM Characters JOIN Movie_Staff ON Characters.ActorID = Movie_Staff.StaffID WHERE FilmID = 'Invitation to Hell'" };
        System.out.println("Query 7 These are the characters and actor of the Invitation to Hell: "
                + runDMLQuery(query7) + "\n");

    }

}

class Jamie {
    public Jamie() {
    }
}

class Nishant { // student 38703238
    public Nishant() {
        createDatabase();
        populateDatabase();
        runScriptedQueries();
    }

    private static void runDDLQuery(String[] statements) {
        Connection connection = null;
        Statement statement = null;

        try {
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/");
            statement = connection.createStatement();

            for (String ddlStatement : statements) {
                statement.executeUpdate(ddlStatement);
            }

        } catch (SQLException e) {
            System.out.println("Error: SQL Exception - " + e.getMessage());
        } finally {
            try {
                if (statement != null) {
                    statement.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close statement - " + e.getMessage());
            }

            try {
                if (connection != null) {
                    connection.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close connection - " + e.getMessage());
            }
        }
    }

    private static List<String> runDMLQuery(String[] queries) {
        List<String> results = new ArrayList<>();
        Connection connection = null;
        PreparedStatement statement = null;

        try {
            connection = DriverManager.getConnection("jdbc:mysql://localhost:3306/ActionMovies");

            for (String query : queries) {
                statement = connection.prepareStatement(query);
                ResultSet resultSet = statement.executeQuery();

                ResultSetMetaData metaData = resultSet.getMetaData();
                int columnCount = metaData.getColumnCount();

                while (resultSet.next()) {
                    StringBuilder row = new StringBuilder();
                    for (int i = 1; i <= columnCount; i++) {
                        row.append(resultSet.getString(i));
                        if (i < columnCount) {
                            row.append(", ");
                        }
                    }
                    results.add(row.toString());
                }
            }
        } catch (SQLIntegrityConstraintViolationException e) {
            results.add("Failed: Constraint violation - " + e.getMessage());
        } catch (SQLException e) {
            results.add("Error: SQL Exception - " + e.getMessage());
        } finally {
            try {
                if (statement != null) {
                    statement.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close statement - " + e.getMessage());
            }

            try {
                if (connection != null) {
                    connection.close();
                }
            } catch (SQLException e) {
                System.err.println("Error: Failed to close connection - " + e.getMessage());
            }
        }

        return results;
    }

    private static void createDatabase() {
        String[] ddlStatements = {
                "CREATE DATABASE ActionMovies",
                "USE ActionMovies",
                "CREATE TABLE IF NOT EXISTS Movie (movieID INT AUTO_INCREMENT, title VARCHAR(255) NOT NULL, releaseDate DATE NOT NULL, PRIMARY KEY (movieID))",
                "CREATE TABLE IF NOT EXISTS Studio (studioID INT AUTO_INCREMENT, name VARCHAR(255) NOT NULL, PRIMARY KEY (studioID))",
                "CREATE TABLE IF NOT EXISTS Director (directorID INT AUTO_INCREMENT, firstName VARCHAR(255) NOT NULL, lastName VARCHAR(255) NOT NULL, dateOfBirth DATE NOT NULL, PRIMARY KEY (directorID))",
                "CREATE TABLE IF NOT EXISTS Actor (actorID INT AUTO_INCREMENT, firstName VARCHAR(255) NOT NULL, lastName VARCHAR(255) NOT NULL, dateOfBirth DATE NOT NULL, PRIMARY KEY (actorID))",
                "CREATE TABLE IF NOT EXISTS Award (awardID INT AUTO_INCREMENT, type VARCHAR(255) NOT NULL, PRIMARY KEY (awardID))",
                "CREATE TABLE IF NOT EXISTS MoviesMade (studioID INT, movieID INT, PRIMARY KEY (studioID, movieID), FOREIGN KEY (studioID) REFERENCES Studio(studioID), FOREIGN KEY (movieID) REFERENCES Movie(movieID))",
                "CREATE TABLE IF NOT EXISTS MoviesDirected (directorID INT, movieID INT, PRIMARY KEY (directorID, movieID), FOREIGN KEY (directorID) REFERENCES Director(directorID), FOREIGN KEY (movieID) REFERENCES Movie(movieID))",
                "CREATE TABLE IF NOT EXISTS `Cast` (movieID INT, actorID INT, PRIMARY KEY (movieID, actorID), FOREIGN KEY (movieID) REFERENCES Movie(movieID) ON DELETE CASCADE, FOREIGN KEY (actorID) REFERENCES Actor(actorID) ON DELETE CASCADE)",
                "CREATE TABLE IF NOT EXISTS AwardWinningMovies (movieID INT, awardID INT, PRIMARY KEY (movieID, awardID), FOREIGN KEY (movieID) REFERENCES Movie(movieID) ON DELETE CASCADE, FOREIGN KEY (awardID) REFERENCES Award(awardID) ON DELETE CASCADE)",
                "CREATE TABLE IF NOT EXISTS AwardWinningActors (actorID INT, awardID INT, PRIMARY KEY (actorID, awardID), FOREIGN KEY (actorID) REFERENCES Actor(actorID) ON DELETE CASCADE, FOREIGN KEY (awardID) REFERENCES Award(awardID) ON DELETE CASCADE)"
        };

        runDDLQuery(ddlStatements);
        System.out.println("\nDatabase for Student 38703238 has been created...\n");
    }

    private static void populateDatabase() {
        try (Scanner sc = new Scanner(new File("38703238.csv"))) {
            sc.useDelimiter(",");

            String prevRow = sc.nextLine();
            String currentRow = null;
            Movie currentMovie = new Movie();

            while (sc.hasNextLine()) {
                currentRow = sc.nextLine();
                String[] currentRowArray = currentRow.split(",");
                String[] prevRowArray = prevRow.split(",");

                if (!currentRowArray[0].equals(prevRowArray[0]) && !currentMovie.getName().equals("")) {
                    populateTable(currentMovie);
                    currentMovie = new Movie();
                }

                for (int i = 0; i < currentRowArray.length; i++) {
                    if (!currentRowArray[i].equals(prevRowArray[i])) {
                        switch (i) {
                            case 0:
                                currentMovie.setName(currentRowArray[i]);
                                break;

                            case 1:
                                currentMovie.setReleaseDate(formatDate(currentRowArray[i]));
                                break;

                            case 2:
                                currentMovie.setStudio(currentRowArray[i]);
                                break;

                            case 3:
                                String[] director = { currentRowArray[i], currentRowArray[i + 1],
                                        formatDate(currentRowArray[i + 2]) };
                                currentMovie.setDirector(director);
                                break;

                            case 6:
                                String[] actor = { currentRowArray[i], currentRowArray[i + 1],
                                        formatDate(currentRowArray[i + 2]) };
                                currentMovie.setActor(actor);
                                break;

                            case 9:
                                if (!currentRowArray[i].equals("None") && currentRowArray[i + 1].equals("None")) {
                                    String[] movieAward = { currentRowArray[i] };
                                    currentMovie.setAward(movieAward);
                                } else if (!currentRowArray[i].equals("None")) {
                                    String[] actorAward = { currentRowArray[i], currentRowArray[i + 1],
                                            currentRowArray[i + 2], formatDate(currentRowArray[i + 3]) };
                                    currentMovie.setAward(actorAward);
                                }

                            default:
                                break;
                        }
                    }
                }

                prevRow = currentRow;
            }

            populateTable(currentMovie);
            currentMovie = new Movie();

            System.out.println("Database for Student 38703238 has been populated...\n");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }

    private static void populateTable(Movie currentMovie) {
        String[] statements = {
                "Use ActionMovies",
                "INSERT INTO Movie (title, releaseDate) VALUES ('" + currentMovie.getName() + "', '"
                        + currentMovie.getReleaseDate() + "')"
        };
        runDDLQuery(statements);

        for (String studio : currentMovie.getStudios()) {
            String[] checks = { "USE ActionMovies", "SELECT * FROM Studio WHERE name='" + studio + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] test = {
                        "Use ActionMovies",
                        "INSERT INTO Studio (name) VALUES ('" + studio + "')"
                };
                runDDLQuery(test);
            }
        }

        for (String[] director : currentMovie.getDirectors()) {
            String[] checks = { "USE ActionMovies", "SELECT * FROM Director WHERE firstName='" + director[0]
                    + "' AND lastName='" + director[1] + "' AND dateOfBirth='" + director[2] + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertDirector = {
                        "Use ActionMovies",
                        "INSERT INTO Director (firstName, lastName, dateOfBirth) VALUES ('" + director[0] + "', '"
                                + director[1] + "', '" + director[2] + "')"
                };
                runDDLQuery(insertDirector);
            }
        }

        for (String[] actor : currentMovie.getActors()) {
            String[] checks = { "USE ActionMovies", "SELECT * FROM Actor WHERE firstName='" + actor[0]
                    + "' AND lastName='" + actor[1] + "' AND dateOfBirth='" + actor[2] + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertActor = {
                        "Use ActionMovies",
                        "INSERT INTO Actor (firstName, lastName, dateOfBirth) VALUES ('" + actor[0] + "', '"
                                + actor[1]
                                + "', '" + actor[2] + "')"
                };
                runDDLQuery(insertActor);
            }
        }

        for (String[] award : currentMovie.getAwards()) {
            String[] checks = { "USE ActionMovies", "SELECT * FROM Award WHERE type='" + award[0] + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertAward = {
                        "Use ActionMovies",
                        "INSERT INTO Award (type) VALUES ('" + award[0] + "')"
                };
                runDDLQuery(insertAward);
            }
        }

        for (String studio : currentMovie.getStudios()) {
            String[] getStudioID = { "USE ActionMovies",
                    "SELECT studioID FROM Studio WHERE name='" + studio + "'" };
            String[] getMovieID = { "USE ActionMovies",
                    "SELECT movieID FROM Movie WHERE title='" + currentMovie.getName() + "'" };
            String[] checks = { "USE ActionMovies", "SELECT * FROM MoviesMade WHERE studioID='"
                    + runDMLQuery(getStudioID).get(0) + "' AND movieID='" + runDMLQuery(getMovieID).get(0) + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertStudioMovieRelation = {
                        "Use ActionMovies",
                        "INSERT INTO MoviesMade (studioID, movieID) VALUES ('" + runDMLQuery(getStudioID).get(0)
                                + "', '" + runDMLQuery(getMovieID).get(0) + "')"
                };
                runDDLQuery(insertStudioMovieRelation);
            }
        }

        for (String[] director : currentMovie.getDirectors()) {
            String[] getDirectorID = { "USE ActionMovies", "SELECT directorID FROM Director WHERE firstName='"
                    + director[0] + "' AND lastName='" + director[1] + "' AND dateOfBirth='" + director[2] + "'" };
            String[] getMovieID = { "USE ActionMovies",
                    "SELECT movieID FROM Movie WHERE title='" + currentMovie.getName() + "'" };
            String[] checks = { "USE ActionMovies", "SELECT * FROM MoviesDirected WHERE directorID='"
                    + runDMLQuery(getDirectorID).get(0) + "' AND movieID='" + runDMLQuery(getMovieID).get(0)
                    + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertDirectorMovieRelation = {
                        "Use ActionMovies",
                        "INSERT INTO MoviesDirected (directorID, movieID) VALUES ('"
                                + runDMLQuery(getDirectorID).get(0)
                                + "', '" + runDMLQuery(getMovieID).get(0) + "')"
                };
                runDDLQuery(insertDirectorMovieRelation);
            }
        }

        for (String[] actor : currentMovie.getActors()) {
            String[] getActorID = { "USE ActionMovies", "SELECT actorID FROM Actor WHERE firstName='" + actor[0]
                    + "' AND lastName='" + actor[1] + "' AND dateOfBirth='" + actor[2] + "'" };
            String[] getMovieID = { "USE ActionMovies",
                    "SELECT movieID FROM Movie WHERE title='" + currentMovie.getName() + "'" };
            String[] checks = { "USE ActionMovies", "SELECT * FROM `Cast` WHERE movieID='"
                    + runDMLQuery(getMovieID).get(0) + "' AND actorID='" + runDMLQuery(getActorID).get(0) + "'" };
            if (runDMLQuery(checks).isEmpty()) {
                String[] insertMovieActorRelation = {
                        "Use ActionMovies",
                        "INSERT INTO `Cast` (movieID, actorID) VALUES ('" + runDMLQuery(getMovieID).get(0) + "', '"
                                + runDMLQuery(getActorID).get(0) + "')"
                };
                runDDLQuery(insertMovieActorRelation);
            }
        }

        for (String[] award : currentMovie.getAwards()) {
            if (award.length == 4) {
                String[] getActorID = { "USE ActionMovies", "SELECT actorID FROM Actor WHERE firstName='" + award[1]
                        + "' AND lastName='" + award[2] + "' AND dateOfBirth='" + award[3] + "'" };
                String[] getAwardID = { "USE ActionMovies",
                        "SELECT awardID FROM Award WHERE type='" + award[0] + "'" };
                String[] checks = { "USE ActionMovies", "SELECT * FROM AwardWinningActors WHERE actorID='"
                        + runDMLQuery(getActorID).get(0) + "' AND awardID='" + runDMLQuery(getAwardID).get(0)
                        + "'" };
                if (runDMLQuery(checks).isEmpty()) {
                    String[] insertActorAwardRelation = {
                            "USE ActionMovies",
                            "INSERT INTO AwardWinningActors (actorID, awardID) VALUES ('"
                                    + runDMLQuery(getActorID).get(0) + "', '" + runDMLQuery(getAwardID).get(0)
                                    + "')"
                    };
                    runDDLQuery(insertActorAwardRelation);
                }
            } else if (award.length == 1) {
                String[] getMovieID = { "USE ActionMovies",
                        "SELECT movieID FROM Movie WHERE title='" + currentMovie.getName() + "'" };
                String[] getAwardID = { "USE ActionMovies",
                        "SELECT awardID FROM Award WHERE type='" + award[0] + "'" };
                String[] checks = { "USE ActionMovies", "SELECT * FROM AwardWinningMovies WHERE movieID='"
                        + runDMLQuery(getMovieID).get(0) + "' AND awardID='" + runDMLQuery(getAwardID).get(0)
                        + "'" };
                if (runDMLQuery(checks).isEmpty()) {
                    String[] insertMovieAwardRelation = {
                            "USE ActionMovies",
                            "INSERT INTO AwardWinningMovies (movieID, awardID) VALUES ('"
                                    + runDMLQuery(getMovieID).get(0) + "', '" + runDMLQuery(getAwardID).get(0)
                                    + "')"
                    };
                    runDDLQuery(insertMovieAwardRelation);
                }
            }
        }

    }

    private static String formatDate(String date) {
        SimpleDateFormat normalFormat = new SimpleDateFormat("dd/MM/yyyy");

        try {
            java.util.Date utilDate = normalFormat.parse(date);
            java.sql.Date sqlDate = new java.sql.Date(utilDate.getTime());
            return sqlDate.toString();
        } catch (ParseException e) {
            e.printStackTrace();
            return null;
        }
    }

    private static void runScriptedQueries() {
        String[] query1 = { "USE ActionMovies", "DELETE FROM Director WHERE directorID = 2" };
        System.out.println("Query 1 " + runDMLQuery(query1).get(0) + "\n");

        String[] query2 = { "USE ActionMovies", "DELETE FROM Studio WHERE studioID = 5" };
        System.out.println("Query 2 " + runDMLQuery(query2).get(0) + "\n");

        String[] query3 = { "USE ActionMovies",
                "SELECT Movie.title FROM Movie JOIN `Cast` ON Movie.movieID = `Cast`.movieID GROUP BY Movie.movieID, Movie.title HAVING COUNT(`Cast`.actorID) = (SELECT MAX (actor_count) FROM (SELECT COUNT(actorID) AS actor_count FROM `Cast` GROUP BY movieID) AS actor_counts)" };
        System.out.println(
                "Query 3 The Movie with the most actors is " + runDMLQuery(query3).get(0).replace(",", "") + "\n");

        String[] query4 = { "USE ActionMovies",
                "SELECT Actor.firstName, Actor.lastName FROM Actor JOIN AwardWinningActors ON Actor.actorID = AwardWinningActors.actorID GROUP BY Actor.actorID HAVING COUNT(AwardWinningActors.awardID) = (SELECT MAX(award_count) FROM (SELECT COUNT(awardID) AS award_count FROM AwardWinningActors GROUP BY actorID) AS actor_awards)" };
        System.out.println(
                "Query 4 The Actor with the most awards is " + runDMLQuery(query4).get(0).replace(",", "") + "\n");
    }

}

class Movie { // student 38703238
    private String name;
    private String releaseDate;
    private List<String> studios;
    private List<String[]> directors; // Tuple of director details: [firstName, lastName, dateOfBirth]
    private List<String[]> actors; // Tuple of actor details: [firstName, lastName, dateOfBirth]
    private List<String[]> awards; // Tuple of award detais: [award] or [award, actorFirstName, actorLastName]

    public Movie() {
        this.name = "";
        this.releaseDate = "";
        this.studios = new ArrayList<>();
        this.directors = new ArrayList<>();
        this.actors = new ArrayList<>();
        this.awards = new ArrayList<>();
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setReleaseDate(String date) {
        this.releaseDate = date;
    }

    public void setStudio(String studio) {
        studios.add(studio);
    }

    public void setDirector(String[] directorDetails) {
        directors.add(directorDetails);
    }

    public void setActor(String[] actorDetails) {
        actors.add(actorDetails);
    }

    public void setAward(String[] award) {
        awards.add(award);
    }

    public String getName() {
        return name;
    }

    public String getReleaseDate() {
        return releaseDate;
    }

    public List<String> getStudios() {
        return studios;
    }

    public List<String[]> getDirectors() {
        return directors;
    }

    public List<String[]> getActors() {
        return actors;
    }

    public List<String[]> getAwards() {
        return awards;
    }

    public void printData() {
        String directorsOutput = directors.stream()
                .map(Arrays::toString)
                .collect(Collectors.joining(", "));

        String actorsOutput = actors.stream()
                .map(Arrays::toString)
                .collect(Collectors.joining(", "));

        String awardsOutput = awards.stream()
                .map(Arrays::toString)
                .collect(Collectors.joining(", "));

        System.out.println(name + ", " + releaseDate + ", " + studios + ", {" + directorsOutput + "}, {" + actorsOutput
                + "}, {" + awardsOutput + "}");
    }

}
