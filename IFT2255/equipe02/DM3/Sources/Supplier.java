/**
 * The Class Supplier, representing a specific type of User in the DataBase.
 * This is a ChocAn supplier who offers services to ChocAn's member.
 * 
 * @author Philippe Caron
 */
public class Supplier extends User {

	/** The address of the supplier. */
	private Address address;
	
	/**
	 * Instantiates a new supplier.
	 *
	 * @param userID the user ID of the supplier
	 * @param hash the hash of the supplier's password
	 * @param name the name of the supervisor
	 * @param address the address of the supervisor
	 */
	public Supplier(int userID, String hash, String name, Address address) {
		setUserID(userID);
		setHash(hash);
		setName(name);
		this.address = address;
	}
	
	/**
	 * Instantiates a new supplier from a string formatted correctly, normally
	 * coming from a line of the DataBase directly.<p>
	 * Format (tab separated): <code>"supplierID passwordHash name streetNumber
	 * streetName city province zipcode status"</code>
	 * 
	 * @param supplierString the supplier string from the DB
	 */
	public Supplier(String supplierString) {
		String[] memInfo = supplierString.split("\t", 8);
		setUserID(Integer.parseInt(memInfo[0]));
		setHash(memInfo[1]);
		setName(memInfo[2]);
		this.address = new Address(
					memInfo[3],
					memInfo[4],
					memInfo[5],
					memInfo[6],
					memInfo[7]);
	}

	/**
	 * Formats the informations of a supplier into a string for storage in the
	 * DataBase <p>
	 * Format (tab separated): <code>"supplierID passwordHash name streetNumber
	 * streetName city province zipcode status"</code>
	 * 
	 * @return String correctly formatted of the supplier, ready to dump in the
	 * DataBase.
	 */
	@Override
	public String toString() {
		String ret = "Supplier\t" + 
					getUserID() + "\t" + 
					getHash() + "\t" +
					getName() + "\t" +
					address.getStreetNumber() + "\t" +
					address.getStreetName() + "\t" +
					address.getCity() + "\t" +
					address.getProvince() + "\t" +
					address.getZipCode();
		return ret;
	}

	/**
	 * Gets the address of the supplier.
	 *
	 * @return his address
	 */
	public Address getAddress() {
		return this.address;
	}

	/**
	 * Sets the address of the supplier.
	 *
	 * @param address his new address
	 */
	public void setAddress(Address address) {
		this.address = address;
	}

}