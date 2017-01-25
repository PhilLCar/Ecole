import static org.junit.Assert.*;

import java.util.ArrayList;

import org.junit.Before;
import org.junit.Test;

public class ServiceDBManagerTest {
	private ServiceDBManager sdbm = new ServiceDBManager();
	private ArrayList<Service> services;
	int validServID;
	String name;
	String description;
	@Before
	public void setUp() { 
		validServID = 123456;
		name = "test123456";
		description = "descriptionTest123456";
		sdbm.add(new Service(validServID, name, description));
		services = DataBase.getDataBase().getServices();
	}
	/**
	  * For the delete(int id) method
	  */
	@Test
	public void invalidID() {
		
		//ID trop long : 7 chiffres au lieu de 6
		assertNull(sdbm.delete(1111111));
		//ID trop court : 5 chiffres au lieu de 6
		assertNull(sdbm.delete(11111));
		//ID non existant : 000000 (non attribuable)
		assertNull(sdbm.delete(000000));
		
	}
	@Test
	public void validID(){
		//Bon elem deleted
		int id1 = sdbm.find(validServID).getServID();
		int id2 = sdbm.delete(validServID).getServID();
		assertEquals(id1,id2);

		
	}
}
