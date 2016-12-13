import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class ReceivableAccountsJobTest {
	private ReceivableAccountsJob raj = new ReceivableAccountsJob();
	UserDBManager userDBM = new UserDBManager();

	@Before
	public void setUp() {
		Address adr = new Address("2313","Sainte-Catherine","Montreal","Quebec","H3C 2C8");
		Member u1 = new Member(200000, "temporaire", "Member One",adr,0);
		Member u2 = new Member(2, "temporaire", "Member Two",adr,1);
		Supplier u3 = new Supplier(300000,	"b473e4ec409f4f288a77f256c74aa52e","Le Centre sportif", adr);
		Supervisor u4 = new Supervisor(100000, "b468a378f511e9e84a225f313c315f07", "Robert Plante");
		userDBM.add(u1);
		userDBM.add(u2);
		userDBM.add(u3);
		userDBM.add(u4);
	}
	
	@After
	public void clearDatabase() {
		userDBM.delete(200000);
		userDBM.delete(2);
		userDBM.delete(300000);
		userDBM.delete(100000);
	}

	/**
	 * For the parseLineAndModifyUserStatus(String line) method
	 */
	@Test
	public void validInputShouldReturnTrue() {
		// Valid statuses for a Member
		assertTrue(raj.parseLineAndModifyUserStatus("200000\t0"));
		assertTrue(raj.parseLineAndModifyUserStatus("000002\t1"));
	}

	/**
	 * For the parseLineAndModifyUserStatus(String line) method
	 */
	@Test
	public void InvalidInputShouldReturnFalse() {
		// users that are not members
		assertFalse(raj.parseLineAndModifyUserStatus("100000\t1"));
		assertFalse(raj.parseLineAndModifyUserStatus("100000\t0"));
		assertFalse(raj.parseLineAndModifyUserStatus("300000\t1"));
		assertFalse(raj.parseLineAndModifyUserStatus("300000\t0"));
		// Space instead of tab
		assertFalse(raj.parseLineAndModifyUserStatus("200000 0"));
		// Entry not a number
		assertFalse(raj.parseLineAndModifyUserStatus("one 0"));
		assertFalse(raj.parseLineAndModifyUserStatus("0 two"));
		// invalid status code
		assertFalse(raj.parseLineAndModifyUserStatus("200000\t8"));
		// invalid memberID
		assertFalse(raj.parseLineAndModifyUserStatus("1992221\t0"));

	}


}
