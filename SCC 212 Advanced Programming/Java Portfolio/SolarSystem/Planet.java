/**
 * The class provides a representation of a planet object, to be drawn on
 * SolarSystem's GUI.
 * This class extends celestial.
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class Planet extends celestial {
	/** Boolean that checks if a planet should be drawn with a ring or not. */
	private boolean ring;

	/**
	 * Constructor for a planet object that orbits the sun.
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
	 * @param ring       states weather an object should have a ring or not.
	 */
	public Planet(double distance, double angle, double diameter, String col, SolarSystem mainSystem, double round,
			boolean ring) {
		super(distance, angle, diameter, col, mainSystem, round);
		this.ring = ring;
	}

	/**
	 * @return the angle in which the planet is at.
	 */
	public double returnAngle() {
		return angle;
	}

	/**
	 * @return the distance from the sun the object is at.
	 */
	public double returnDistance() {
		return distance;
	}

	/**
	 * Method for planet to orbit the sun, with the included drawing of a ring round
	 * the planet.
	 */
	@Override
	public void orbit() {
		if (ring) {
			mainSystem.drawSolarObject(distance, angle, diameter + 20, "#996515");
			mainSystem.drawSolarObject(distance, angle, diameter + 6, "BLACK");
		}
		mainSystem.drawSolarObject(distance, angle, diameter, col);
		angle += orbitSpeed;
	}
}
