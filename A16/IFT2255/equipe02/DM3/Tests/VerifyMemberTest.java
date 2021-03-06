import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class VerifyMemberTest {
	UserDBManager userDBM = new UserDBManager();

	@Before
	public void setUp() {
		Address adr = new Address("2313", "Sainte-Catherine", "Montreal", "Quebec", "H3C 2C8");
		Member m1 = new Member(11, "temporaire", "Member One", adr, 0);
		m1.setHashFromPassword("member1pass");
		Member m2 = new Member(22, "temporaire", "Member Two", adr, 1);
		m2.setHashFromPassword("member2pass");
		Member m3 = new Member(33, "temporaire", "Member Three", adr, 2);
		m3.setHashFromPassword("member3pass");
		userDBM.add(m1);
		userDBM.add(m2);
		userDBM.add(m3);
	}

	@After
	public void clearDatabase() {
		userDBM.delete(11);
		userDBM.delete(22);
		userDBM.delete(33);
		SessionManager.deleteSessionFromID(11);
		SessionManager.deleteSessionFromID(22);
		SessionManager.deleteSessionFromID(33);
	}

	@Test
	public void memberStatusTest() {
		assertTrue(SupplierSession.verifyMemberStatus(11).equals("Validé"));
		assertTrue(SupplierSession.verifyMemberStatus(22).equals("Membre suspendu pour frais impayés"));
		assertTrue(SupplierSession.verifyMemberStatus(33).equals("Le membre est en retard sur son payement"));
		assertTrue(SupplierSession.verifyMemberStatus(44).equals("Numéro invalide"));
	}

}
