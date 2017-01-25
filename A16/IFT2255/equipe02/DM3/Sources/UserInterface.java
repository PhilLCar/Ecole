import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
* As its name implies, it exists to provide a UI to the user. In this case,
* it's mainly for the login UI
*/
public class UserInterface extends JFrame {

	/**
	 *
	 */
	private static final long serialVersionUID = 1L;

	/**
	 * Processes the user's login and will launch a session if the
	 * information provided is correct.
	 * @param userID ID of a user, from a corresponding textfield
	 * @param userPassword unhashed password from the correspond textfield
	 */
	private void login(String user, String userPassword) {
		int userID;
		Session session;
		try {
			userID = Integer.parseInt(user);
		} catch (Exception e) {
			e.printStackTrace(System.err);
			return;
		}

		try {
			session = SessionManager.authenticate(userID, userPassword);
			if (session != null) session.getUI(this);
		} catch (ConnectionException e) {
			JOptionPane.showMessageDialog(new JPanel(), e.getErrorMessage(), "Erreur", JOptionPane.ERROR_MESSAGE);
		}

	}
/**
* Closes the session on which it is called.
* @param session active user's session
*/
	public void disconnect(Session session) {
		SessionManager.deleteSession(session);
		new UserInterface();
		dispose();
	}
/**
* Default constructor, prompts the user to provide data for launching a session.
*/
	public UserInterface() {
		setLayout(new FlowLayout());
		JLabel loglab = new JLabel("Numéro d'utilisateur:");
		JTextField login = new JTextField(20);
		JLabel paslab = new JLabel("Mot de passe:");
		JTextField pass = new JTextField(20);
		JButton submit = new JButton("Connexion");
		submit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				login(login.getText(), pass.getText());
			}
		});
		JButton quit = new JButton("Quitter");
		quit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent arg0) {
				dispose();
			}
		});
		add(loglab);
		add(login);
		add(paslab);
		add(pass);
		add(quit);
		add(submit);
		setSize(250, 180);
		setVisible(true);
	}

}
