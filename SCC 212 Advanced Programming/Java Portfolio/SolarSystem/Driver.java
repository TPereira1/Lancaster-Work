/**
 * Driver class to start the program that models our solar system.
 * 
 * @author Thomas Pereira, Student ID:38866366
 */
public class Driver {
	static Universe a;
	static boolean gameOver;

	/**
	 * The main calls for an instance of Universe to be created and calls for the
	 * orbiting of solar object to start until GameOver.
	 * This can be extend for further functionality if KeyListener added to
	 * SolarSystem class.
	 */
	public static void main(String[] args) {
		a = new Universe();
		gameOver = false;
		while (!gameOver) {
			a.orbit();
		}
	}

	// @Override
	// public void keyPressed(KeyEvent e) {
	// switch(e.getKeyChar()) {
	// case 'Q':
	// gameOver = true;
	// break;
	// case KeyEvent.VK_LEFT:
	// a.changeSpeed(false);
	// break;
	// case KeyEvent.VK_RIGHT:
	// a.changeSpeed(true);
	// break;
	// }
	// }

}
