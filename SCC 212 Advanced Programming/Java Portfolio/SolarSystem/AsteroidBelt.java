import java.util.*;
/**
 * This class provides a representation for an asteroid belt around a celestial body.
 * @author Thomas Pereira, Student ID:38866366
 */
public class AsteroidBelt {
	/**An array that hold the instances of Satellite, used for the asteroids that form the belt.*/
	private ArrayList<Satellite> asteroids;
	
	/**
	 * Constructor that creates an array of asteroids moving at random speeds that will make up the belt.
	 * @param distance from the object it orbits
	 * @param mainSystem is an instance of SolarSystem to draw to
	 */
	public AsteroidBelt(double distance, SolarSystem mainSystem) {
		Random rand = new Random();
		double perimeter = 2*distance*Math.PI;
		int numOfAestroids = (int)perimeter/7;
		asteroids = new ArrayList<>();
		double angle = 0;
		for(int i = 0; i < numOfAestroids; i++) {
			Satellite a = new Satellite(distance, angle, 5, "GREY", mainSystem,rand.nextDouble()/2);
			Satellite b = new Satellite(distance+3, angle + 5, 5, "GREY", mainSystem,rand.nextDouble()/2);
			Satellite c = new Satellite(distance-3, angle + 5, 5, "GREY", mainSystem,rand.nextDouble()/2);
			asteroids.add(a);
			asteroids.add(b);
			asteroids.add(c);
			angle += 10;
		}
		
	}
	
	/**
	 * Method that changes the position of all asteroids in the belt;
	 */
	public void orbit() {
		for (int i = 0; i < asteroids.size(); i++) {
			asteroids.get(i).orbit();
		}
	}
	
	/**
	 * Method to speed up the speed in which the asteroids orbits
	 * Unused since SolarSystem would require a key listener to implement, but useful feature if program extended. 
	 */
	public void speedUp() {
		for(celestial object: asteroids) {
			object.speedup();
		}
	}
	
	/**
	 * Method to slow down the speed in which the asteroids orbits
	 * Unused since SolarSystem would require a key listener to implement, but useful feature if program extended. 
	 */
	public void slowDown() {
		for(celestial object: asteroids) {
			object.slowdown();;
		}
	}

}
