import java.lang.Math;

public class GamingParty {
	private String theme;
	private BoardGame boardGame;
	private Player[] players;
	private Snack[] snacks;
	private Player winner;
	
	public GamingParty(String aTheme, BoardGame aBoardGame) {
		theme = aTheme;
		boardGame = aBoardGame;
		int maxPlayers = this.boardGame.getMaximumPlayers();
		players = new Player[maxPlayers];
		snacks = new Snack[maxPlayers*2];
		winner = null;
	}
	
	public int getNumberOfPlayers() {
		int numPlayer = 0;
		for(int i = 0; i < players.length;i++) {
			if(players[i] == null) {
				break;
			}
			numPlayer++;
		}
		return numPlayer;
	}
	
	public void addPlayer(Player aPlayer) {
		boolean addPlayer = true;
		if(aPlayer.getAge() >= boardGame.getMinimumAge()) {
			int numPlayer = getNumberOfPlayers();
			for(int i = 0; i < numPlayer;i++) {
				if(aPlayer == players[i]) {
					System.out.println("The player is already playing the game at this party.");
					addPlayer = false;
					break;
				}
			}
			
			if(addPlayer == true && numPlayer == boardGame.getMaximumPlayers()) {
				System.out.println("The maximum number of players has been reached for the game at this party.");
				addPlayer = false;
			}
			else if(addPlayer == true) {
				players[numPlayer] = aPlayer;
			}
		}
		else {
			System.out.println("The player does not meet the age requirements for the board game at this party.");
		}
	}
	
	public void play() {
		int numPlayers = getNumberOfPlayers();
		int minPlayers = boardGame.getMinimumPlayers();
		
		if( numPlayers >= minPlayers) {
			System.out.println("Play!");
		}
		else {
			int playersReq = minPlayers - numPlayers;
			System.out.println("You need "+playersReq+" more player(s)!");
		}
	}
	
	public void addSnack(Snack aSnack) {
		int numSnacks = 0;
		for(int i = 0; i < snacks.length; i++) {
			if(snacks[i] == null) {
				break;
			}
			numSnacks++;
		}
		if (numSnacks == snacks.length) {
			System.out.println("There are enough snacks!");
		}
		else {
			snacks[numSnacks] = aSnack;
		}
	}
	
	public double getPartyCost() {
		double total = 0;
		int i = 0;
		while(i < snacks.length && snacks[i]!= null) {
			total += snacks[i].getCost();
			i++;
		}
		return total;
	}

	public Player getWinner() {
		return winner;
	}

	public void setWinner(Player aWinner) {
		if(winner != null) {
			System.out.println("The winner has already been decided! It was "+winner.getName()+".");
		}
		else{
			boolean wasAtParty = false;
			for(int i = 0; i < players.length; i++) {
				if(players[i]==aWinner) {
					wasAtParty = true;
					break;
				}
			}
			if(wasAtParty) {
				winner = aWinner;
			}
			else {
				System.out.println("The player didn't even play the game so cannot win!");
			}
		}
	}
	
	public void outputPartyDetails() {
		System.out.print("Theme: "+theme+"\nBoard game: "+boardGame.getTitle()+"\nPlayers:\n");
		for(int i = 0; i < getNumberOfPlayers(); i++) {
			System.out.println(players[i].getName());
		}
		System.out.println("Snacks:");
		for(int i = 0; i < snacks.length; i++) {
			if(snacks[i]==null) {
				break;
			}
			System.out.println(snacks[i].getDescription()+" provided by "+snacks[i].getProvider().getName());
		}
		if(winner != null) {
			System.out.println("The winner is "+winner.getName()+"!");
		}
		else {
			System.out.println("No winner yet!");
		}
	}
	
	public void calculateRecommendedSnacks() {
		int duration = Math.round(boardGame.getDuration()/60);
		int numPlayer = getNumberOfPlayers();
		int numSnacks = numPlayer * duration;
		if(numPlayer > 0) {
			if(numSnacks > snacks.length) {
				System.out.println("A total of "+snacks.length+" snack(s) are recommended for the game.");
			}
			else if(numSnacks < numPlayer) {
				System.out.println("A total of "+numPlayer+" snack(s) are recommended for the game.");
			}
			else {
				System.out.println("A total of "+numSnacks+" snack(s) are recommended for the game.");
			}
		}
	}

}
