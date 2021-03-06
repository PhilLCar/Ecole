import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
* Methods for regular Member activities and the GUI needed for them.
*/
public class MemberSession extends Session {

	/**
	 * Instantiates a MemberSession using Session constructor
	 * @param member used to create a specific session for the user
	 *
	 */
	public MemberSession(Member member) {
		super(member);
	}
	/**
	* Serves the GUI for the Member.
	*
	* @param UI the method will change the active GUI, thus needing a reference to it.
	*/
	@Override
	public void getUI(UserInterface UI) {
		UI.getContentPane().removeAll();
		UI.setTitle("Menu membre");

		UI.add(new JLabel("Entrez le montant que vous souhaitez payer:"));
		// On ne peut afficher le montant dû, car Acme s'en charge.

		JTextField payfield = new JTextField(25);
		UI.add(payfield);

		JButton paybutton = new JButton("Payer");
		paybutton.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				JOptionPane.showMessageDialog(new JPanel(), "Félicitations, paiement complété");
			}

		});

		JButton quit = new JButton("Quitter");
		Session thisSession = this;
		quit.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				UI.disconnect(thisSession);
			}

		});

		UI.add(paybutton);
		UI.add(quit);
		UI.setSize(330, 120);
		UI.setVisible(true);
	}
}
