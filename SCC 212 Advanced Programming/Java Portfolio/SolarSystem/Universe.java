import java.util.*;

/**
 * This class provides a simple model of our solar system, with the sun, 8
 * planets, 4 moons, and an asteroid belt.
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class Universe {
	/**
	 * ArrayList to hold for all celestial bodies created to represent the model,
	 * except the asteroid belt
	 */
	private ArrayList<celestial> universe = new ArrayList<celestial>();
	/** This is the asteroid belt between Jupiter and Mars. */
	private AsteroidBelt a;
	/**
	 * This variable used to limit number of speed up to be used
	 * not in use due to lack of KeyListener in SolarSystem class
	 */
	private int numSpeedUp = 0;
	/** Instance of SolarSystem to draw to */
	private SolarSystem MainSystem;
	/** Planet with moons that need to kept track of */
	Planet Jupiter;
	/** Planet with moons that need to kept track of */
	Planet Earth;

	/**
	 * This constructor initialises the entire solar system application, from the
	 * SolarSystem GUI, to the celestial bodies.
	 * where all planets, satellites, asteroid belts, stars are initialised.
	 */
	public Universe() {
		MainSystem = new SolarSystem(1250, 700);
		Stars theSun = new Stars(0, 0, 115, "YELLOW", MainSystem, 0);
		Planet Mercury = new Planet(80, 270, 5, "#B7B8B9", MainSystem, 88, false);
		Planet Venus = new Planet(100, 270, 12, "#C18F17", MainSystem, 225, false);
		Earth = new Planet(120, 270, 13, "#34A56F", MainSystem, 365, false);
		Satellite Moon = new Satellite(10, 270, 3, "GREY", MainSystem, 28);
		Moon.setName("Earth");
		Planet Mars = new Planet(150, 270, 7, "#FF0000", MainSystem, 687, false);
		a = new AsteroidBelt(180, MainSystem);
		Jupiter = new Planet(280, 270, 69, "#FDC360", MainSystem, 4333, false);
		Satellite Europa = new Satellite(50, 270, 5, "#44a9f0", MainSystem, 4);
		Satellite Ganymede = new Satellite(60, 270, 3, "#8B7D82", MainSystem, 7);
		Satellite Callisto = new Satellite(70, 270, 7, "#CACFD3", MainSystem, 17);
		Europa.setName("Jupiter");
		Ganymede.setName("Jupiter");
		Callisto.setName("Jupiter");
		Planet Saturn = new Planet(420, 270, 55, "#CC9966", MainSystem, 10756, true);
		Planet Uranus = new Planet(510, 270, 31, "#ACE5EE", MainSystem, 30687, false);
		Planet Neptune = new Planet(580, 270, 29, "#4B70DD", MainSystem, 60190, false);
		universe.add(theSun);
		universe.add(Mercury);
		universe.add(Venus);
		universe.add(Earth);
		universe.add(Moon);
		universe.add(Mars);
		universe.add(Jupiter);
		universe.add(Europa);
		universe.add(Ganymede);
		universe.add(Callisto);
		universe.add(Saturn);
		universe.add(Uranus);
		universe.add(Neptune);
	}

	/**
	 * method to paint all celestial bodies on to the mainSystem GUI.
	 */
	public void orbit() {
		for (celestial object : universe) {
			if (object instanceof Satellite) {
				if (((Satellite) object).getName().equals("Jupiter")) {
					object.orbit(Jupiter.returnDistance(), Jupiter.returnAngle());
				} else if (((Satellite) object).getName().equals("Earth")) {
					object.orbit(Earth.returnDistance(), Earth.returnAngle());
				}
			} else {
				object.orbit();
			}
		}
		a.orbit();
		MainSystem.finishedDrawing();
	}

	/**
	 * Method to change speed of all celestial bodies orbiting in the solar system
	 * model
	 * method goes unused since need key listener to take in inputs while running
	 * 
	 * @param speedUp is a boolean checking to speedup orbit or slow down when
	 *                called.
	 */
	public void changeSpeed(boolean speedUp) {
		if (speedUp && numSpeedUp < 4) {
			numSpeedUp++;
			for (celestial object : universe) {
				object.speedup();
			}
			a.speedUp();
		} else if (!speedUp && numSpeedUp > 0) {
			numSpeedUp--;
			for (celestial object : universe) {
				object.slowdown();
			}
			a.slowDown();
		}
	}
}
