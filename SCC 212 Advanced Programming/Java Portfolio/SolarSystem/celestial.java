/**
 * This class provides for the representation of celestial spherical object that
 * get drawn to SolarSystem's GUI.
 * This class should be extended as it contains all common variables and methods
 * for a typical celestial body to model in SolarSystem's GUI.
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class celestial {
	/**
	 * Distance from the object it orbits or the centre of screen depends on solar
	 * object
	 */
	protected double distance;
	/**
	 * Angle of the object on the screen from the object orbiting/centre of the
	 * screen
	 */
	protected double angle;
	/** The diameter of solar object */
	protected double diameter;
	/** Colour of the solar object */
	protected String col;
	/** Instance of SolarSystem to draw to */
	protected SolarSystem mainSystem;
	/** Angle increase of an orbiting object per frame draw */
	protected double orbitSpeed;

	/**
	 * Constructor takes in the relevant values of a celestial object being
	 * modelled.
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
	public celestial(double distance, double angle, double diameter, String col, SolarSystem mainSystem, double round) {
		this.distance = distance;
		this.angle = angle;
		this.diameter = diameter;
		this.col = col;
		if (round != 0) {
			this.orbitSpeed = 360 / (round * 4);
		} else {
			this.orbitSpeed = 0;
		}
		this.mainSystem = mainSystem;
	}

	/**
	 * Method that updates the position of a celestial object that orbits the Sun.
	 */
	public void orbit() {
		mainSystem.drawSolarObject(distance, angle, diameter, col);
		angle += orbitSpeed;
	}

	/**
	 * Method that updates the position of an object that orbits another celestial
	 * body which isn't the sun
	 * 
	 * @param centreOfRotationDistance
	 * @param centreOfRotationAngle
	 */
	public void orbit(double centreOfRotationDistance, double centreOfRotationAngle) {
		mainSystem.drawSolarObjectAbout(distance, angle, diameter, col, centreOfRotationDistance,
				centreOfRotationAngle);
		angle += orbitSpeed;
	}

	/**
	 * Method to slow down the speed in which the object orbits
	 * Unused since SolarSystem would require a key listener to implement, but
	 * useful feature if program extended.
	 */
	public void slowdown() {
		this.orbitSpeed = orbitSpeed / 2;
	}

	/**
	 * Method to speed up the speed in which the object orbits
	 * Unused since SolarSystem would require a key listener to implement, but
	 * useful feature if program extended.
	 */
	public void speedup() {
		this.orbitSpeed = orbitSpeed * 2;
	}
}
