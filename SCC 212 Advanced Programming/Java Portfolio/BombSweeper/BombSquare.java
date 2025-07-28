import java.util.*;
import javax.swing.*;

/**
 * This class is the subclass of GameSquare,
 * used to represent a single game square that can hold a mine or not.
 * 
 * @author Thomas Pereira, Student ID: 38866366
 */
public class BombSquare extends GameSquare {
	/** Variable used to check if the square is a mine or not **/
	private boolean thisSquareHasBomb = false;
	/** The constant probability that a square will have a bomb or not **/
	public static final int MINE_PROBABILITY = 10;
	/** A variable used to check if the square has already been clicked before **/
	private boolean clicked = false;

	/**
	 * Creates a BombSquare, which can be placed on the GameBoard.
	 * A square is randomly assigned to have a bomb on it, depending on
	 * MINE_PROBABILITY
	 * 
	 * @param x     the x co-ordinate of this square on the game board.
	 * @param y     the y co-ordinate of this square on the game board.
	 * @param board the GameBoard upon which this square resides.
	 */
	public BombSquare(int x, int y, GameBoard board) {
		super(x, y, "images/blank.png", board);

		Random r = new Random();
		thisSquareHasBomb = (r.nextInt(MINE_PROBABILITY) == 0);
	}

	/**
	 * This method invoked when the user clicks on a square.
	 * If a square has a mine it displays a mine on the clicked square.
	 * If not it will show the number of bombs surrounding the square.
	 * If that number is zero will reveal the adjacent squares surrounding it.
	 */
	public void clicked() {
		if (clicked) {
			return;
		} else if (thisSquareHasBomb) {
			endGame();
		} else {
			int bombs = getNumBombs();
			setImage("images/" + bombs + ".png");
			clicked = true;
			if (bombs == 0) {
				for (int x = -1; x < 2; x++) {
					for (int y = -1; y < 2; y++) {
						if (board.getSquareAt(xLocation + x, yLocation + y) != null) {
							board.getSquareAt(xLocation + x, yLocation + y).clicked();
						}
					}
				}
			}

		}
	}

	/**
	 * Method that ends a game when bomb is clicked on
	 */
	public void endGame() {
		for (int x = 0; x <= getXmax(); x++) {
			for (int y = 0; y <= getYmax(); y++) {
				board.getSquareAt(x, y).setImage("images/bomb.png");
			}
		}
		JOptionPane.showMessageDialog(null, "Game Over! You've have hit a mine.");
		board.dispose();
		board = new GameBoard("BombSweeper", getXmax(), getYmax());
	}

	/**
	 * @return the width of the board
	 */
	public int getXmax() {
		int x = 0;
		while (board.getSquareAt(x, 1) != null) {
			x++;
		}
		return (x - 1);
	}

	/**
	 * @return the height of the board
	 */
	public int getYmax() {
		int y = 0;
		while (board.getSquareAt(1, y) != null) {
			y++;
		}
		return (y - 1);
	}

	/**
	 * Checks if a square has a mine or not.
	 * 
	 * @return true/false depending if the square has a mine.
	 */
	public boolean HasMine() {
		return thisSquareHasBomb;
	}

	/**
	 * Checks all the adjacent squares if they contain a bomb or not
	 * 
	 * @return the number of bombs surrounding a square.
	 */
	public int getNumBombs() {
		int numBombs = 0;
		for (int x = -1; x < 2; x++) {
			for (int y = -1; y < 2; y++) {
				if (board.getSquareAt(xLocation + x, yLocation + y) != null
						&& ((BombSquare) board.getSquareAt(xLocation + x, yLocation + y)).HasMine()) {
					numBombs++;
				}
			}
		}
		return numBombs;
	}
}
