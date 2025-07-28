/**
 * This class provides a representation of a satellite object that orbit about
 * other celestial bodies
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class Satellite extends celestial {
	/** This is the name of the planet or celestial body a satellite orbits */
	private String name;

	/**
	 * Constructor for a Satellite object that orbits a celestial body.
	 * 
	 * @param distance   from the object it orbits or the centre of screen depends
	 *                   on solar object
	 * @param angle      of the object on the screen from the object orbiting/centre
	 *                   of the screen
	 * @param diameter   of solar object
	 * @param col        of the solar object
	 * @param mainSystem Instance of SolarSystem to draw to
	 * @param round      the number or Earth days the object takes to orbit another
	 *                   celestial object
	 */
	public Satellite(double distance, double angle, double diameter, String col, SolarSystem mainSystem, double round) {
		super(distance, angle, diameter, col, mainSystem, round);
	}

	/**
	 * This method take in the name of celestial body it orbits
	 * 
	 * @param name of celestial body being orbited
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the name of celestial body being orbited
	 */
	public String getName() {
		return name;
	}

}
