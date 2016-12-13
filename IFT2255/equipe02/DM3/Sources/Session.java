import javax.swing.JFrame;
import javax.swing.JPanel;

/**
 * The abstract Class Session, representing a session on the server for a user.
 *
 * @author Philippe Caron
 * @see SessionManager
 */
public abstract class Session {

	/** The user whose session this is. */
	private User user;

	/**
	 * Instantiates a new session for a specific user.
	 *
	 * @param user the user
	 */
	public Session(User user) {
		this.user = user;
	}

	/**
	 * Gets the user of this session.
	 *
	 * @return the user
	 */
	public User getUser() {
		return this.user;
	}

	/**
	 * Modifies the input UI for a given User type.
	 *
	 * @param UI current GUI
	 */
	public abstract void getUI(UserInterface UI);
}