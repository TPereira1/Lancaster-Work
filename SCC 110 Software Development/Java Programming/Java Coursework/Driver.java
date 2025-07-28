public class Driver {

	public static void main(String[] args) {
		Player p1 = new Player("Kit", 18);
		Player p2 = new Player("Riley", 19);
		Snack s1 = new Snack ("Chips", 5, p2);
		BoardGame b1 = new BoardGame("Monopoly",p1,8,120,2,6,54.7);
		GamingParty party = new GamingParty("Birthday", b1);
		party.addPlayer(p1);
		party.addPlayer(p2);
		party.addSnack(s1);
		party.outputPartyDetails();
		party.calculateRecommendedSnacks();

	}

}
