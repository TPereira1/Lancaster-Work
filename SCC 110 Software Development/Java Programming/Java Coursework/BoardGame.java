public class BoardGame {
	private String title;
	private Player owner;
	private int minimumAge;
	private int duration;
	private int minimumPlayers;
	private int maximumPlayers;
	private double rating;
	
	public BoardGame(String aTitle, Player theOwner, int minAge, int theDuration, int minPlayers, int maxPlayers, double theRating){
		title = aTitle;
		owner = theOwner;
		minimumAge = minAge;
		duration = theDuration;
		minimumPlayers = minPlayers;
		maximumPlayers = maxPlayers;
		if(theRating >= 0 && theRating <=100) {
			rating = theRating;
		}
		else {
			System.out.println("Rating percentage not valid. Setting to 0.");
			rating = 0;
		}
	}

	public String getTitle() {
		return title;
	}

	public Player getOwner() {
		return owner;
	}

	public int getMinimumAge() {
		return minimumAge;
	}

	public int getDuration() {
		return duration;
	}

	public int getMinimumPlayers() {
		return minimumPlayers;
	}

	public int getMaximumPlayers() {
		return maximumPlayers;
	}

	public double getRating() {
		return rating;
	}

	public void setRating(double theRating) {
		if(theRating >= 0 && theRating <=100) {
			rating = theRating;
		}
		else {
			System.out.println("Rating percentage not valid.");
		}
	}
}
