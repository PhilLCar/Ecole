/**
 * The Class Supervisor, representing a specific type of User in the DataBase.
 * This is the ChocAn supervisor whose role is to control most of the data in
 * the system, as well as printing specific reports.
 * 
 * @author Philippe Caron
 */
public class Supervisor extends User {

	/**
	 * Instantiates a new supervisor.
	 *
	 * @param userID the user ID
	 * @param hash the hash of the supervisor's password
	 * @param name the name of the supervisor
	 */
	public Supervisor(int userID, String hash, String name) {
		setUserID(userID);
		setHash(hash);
		setName(name);
	}
	
	/**
	 * Instantiates a new supervisor from a string formatted correctly, normally
	 * coming from a line of the DataBase directly.<p>
	 * Format (tab separated): <code> supervisorID passwordHash name</code>
	 * 
	 * @param supervisorString the supervisor string from the DB
	 */
	public Supervisor(String supervisorString) {
		String[] memInfo = supervisorString.split("\t", 3);
		setUserID(Integer.parseInt(memInfo[0]));
		setHash(memInfo[1]);
		setName(memInfo[2]);
	}

	/** 
	 * Formats the informations of a supervisor into a string ready for storage
	 * in the DataBase, preceded by the tag "Supervisor"<p>
	 * Format (tab separated): <code>tag supervisorID passwordHash name</code>
	 * 
	 * @return String correctly formatted of the supervisor, ready to dump in the
	 * DataBase.
	 */
	@Override
	public String toString() {
		String ret = "Supervisor\t" + 
					getUserID() + "\t" + 
					getHash() + "\t" +
					getName();
		return ret;
	}
}