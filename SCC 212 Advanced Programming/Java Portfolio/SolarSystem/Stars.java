/**
 * The class provides a representation of a stars object, to be drawn on
 * SolarSystem's GUI.
 * This class extends celestial.
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class Stars extends celestial {

	/**
	 * Constructor for a star object (e.g. Sun).
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
	public Stars(double distance, double angle, double diameter, String col, SolarSystem mainSystem, double round) {
		super(distance, angle, diameter, col, mainSystem, round);
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

}
