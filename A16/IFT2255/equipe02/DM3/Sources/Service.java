/**
 * The Class Service, representing a service offered to members of ChocAn in
 * the DataBase.
 * 
 * @author Gevrai Jodoin-Tremblay
 * @see ServiceDBManager
 */
public class Service {

	/** The service ID. */
	private int servID;
	
	/** The name of the service. */
	private String name;
	
	/** The description of the service. */
	private String description;

	/**
	 * Instantiates a new service.
	 *
	 * @param servID the service ID
	 * @param name the name of the service
	 * @param description the description of the service
	 */
	public Service(int servID, String name, String description) {
		this.servID = servID;
		this.name = name;
		this.description = description;
	}
	
	/**
	 * Instantiates a new service without specifying a specific ID, will thus
	 * find a unique one to create the Service properly in the DataBase.
	 *
	 * @param name the name of the service
	 * @param description the description of the service
	 */
	public Service(String name, String description) {
		this.servID = new ServiceDBManager().getUniqueID();
		this.name = name;
		this.description = description;
	}

	/**
	 * Instantiates a new service from a string formatted correctly, normally
	 * coming from a line of the DataBase directly.<p>
	 * Format (tab separated): <code>serviceID name description</code>
	 * 
	 * @param service the service string from the DB
	 */
	public Service(String service) {
		String[] servInfo = service.split("\t", 3);
		this.servID = Integer.parseInt(servInfo[0]);
		this.name = servInfo[1];
		this.description = servInfo[2];
	}

	/** 
	 * Formats the informations of a Service into a string ready for storage
	 * in the DataBase<p>
	 * Format (tab separated): <code>serviceID name description</code>
	 * 
	 * @return String correctly formatted of the service, ready to dump in the
	 * DataBase.
	 */
	@Override
	public String toString(){
		return servID + "\t" + name + "\t" + description;
	}

	/**
	 * Gets the service ID.
	 *
	 * @return the service ID
	 */
	public int getServID() {
		return this.servID;
	}

	/**
	 * Sets the service ID.
	 *
	 * @param servID the new service ID
	 */
	public void setServID(int servID) {
		this.servID = servID;
	}

	/**
	 * Gets the name of the service.
	 *
	 * @return the name of the service
	 */
	public String getName() {
		return this.name;
	}

	/**
	 * Sets the name of the service.
	 *
	 * @param name the new name of the service
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * Gets the description of the service
	 *
	 * @return the description of the service
	 */
	public String getDescription() {
		return this.description;
	}

	/**
	 * Sets the description of the service.
	 *
	 * @param description the new description of the service
	 */
	public void setDescription(String description) {
		this.description = description;
	}
	

}