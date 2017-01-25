/**
 * The Class Member, representing an entry in the DataBase for a Member
 * of ChocAn.
 * 
 * @author Gevrai Jodoin-Tremblay
* @see UserDBManager
 */
public class Member extends User {
	
	public static final int STATUS_VALID = 0;
	public static final int STATUS_SUSPENDED = 1;
	public static final int STATUS_LATEPAYMENT = 2;

	/** The address. */
	private Address address;
	
	/** The status. */
	private int status;
	
	/**
	 * Instantiates a new member.
	 *
	 * @param userID his user ID
	 * @param hash the hash for his password in the system 
	 * @param name his name
	 * @param address his address
	 * @param status his status
	 */
	public Member(int userID, String hash, String name, Address address, int status) {
		setUserID(userID);
		setHash(hash);
		setName(name);
		this.address = address;
		this.status = status;
	}
	
	/**
	 * Instantiates a new member from a string formatted correctly, normally
	 * coming from a line of the DataBase directly.<p>
	 * Format (tab separated): <code>"userID hash name streetNumber streetName
	 * city province zipcode status"</code> 
	 *
	 * @param memberString the member string from the DB
	 */
	public Member(String memberString) {
		String[] memInfo = memberString.split("\t", 9);
		setUserID(Integer.parseInt(memInfo[0]));
		setHash(memInfo[1]);
		setName(memInfo[2]);
		this.address = new Address(
					memInfo[3],
					memInfo[4],
					memInfo[5],
					memInfo[6],
					memInfo[7]);
		this.status = Integer.parseInt(memInfo[8]);
	}

	/**
	 * Formats the informations of a member into a string for storage in the
	 * DataBase <p>
	 * Format (tab separated): <code>"userID hash name streetNumber streetName
	 * city province zipcode status"</code>
	 * 
	 * @return String correctly formatted of the user, ready to dump in the
	 * DataBase.
	 */
	@Override
	public String toString() {
		return "Member\t" 
				+ getUserID() + "\t" 
				+ getHash() + "\t" 
				+ getName() + "\t" 
				+ address.getStreetNumber() + "\t" 
				+ address.getStreetName() + "\t" 
				+ address.getCity() + "\t" 
				+ address.getProvince() + "\t" 
				+ address.getZipCode() + "\t"
				+ status;
	}
	/**
	 * Gets the address.
	 *
	 * @return the address
	 */
	public Address getAddress() {
		return this.address;
	}

	/**
	 * Sets the address.
	 *
	 * @param address the new address
	 */
	public void setAddress(Address address) {
		this.address = address;
	}

	/**
	 * Gets the status.
	 *
	 * @return the status
	 */
	public int getStatus() {
		return this.status;
	}
	
	/**
	 * Sets the status only if the value is valid, either
	 * <ul><li>STATUS_VALID = 0<li>STATUS_SUSPENDED = 1</ul>
	 *
	 * @param status the new status
	 * @return true if valid and set, false if not valid
	 */
	public boolean setStatus(int status) {
		if (status == STATUS_VALID || status == STATUS_SUSPENDED || status == STATUS_LATEPAYMENT) {
			this.status = status;
			return true;
		}
		return false;
	}

}