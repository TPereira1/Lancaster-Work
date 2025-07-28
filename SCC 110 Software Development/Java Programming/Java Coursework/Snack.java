public class Snack {
	private String description;
	private double cost;
	private Player provider;
	
	public Snack(String theDescription,double theCost, Player theProvider) {
		description = theDescription;
		cost = theCost;
		provider = theProvider;
	}
	
	public String getDescription() {
		return description;
	}

	public double getCost() {
		return cost;
	}

	public Player getProvider() {
		return provider;
	}
}
