import static org.junit.Assert.*;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class SessionManagerTest {
	UserDBManager userDBM = new UserDBManager();

	@Before
	public void setUp() {
		Address adr = new Address("2313","Sainte-Catherine","Montreal","Quebec","H3C 2C8");
		Member m1 = new Member(1, "temporaire", "Member One",adr,0);
		m1.setHashFromPassword("member1pass");
		Member m2 = new Member(2, "temporaire", "Member Two",adr,1);
		m2.setHashFromPassword("member2pass");
		userDBM.add(m1);
		userDBM.add(m2);
	}
	
	@After
	public void clearDatabase() {
		userDBM.delete(1);
		userDBM.delete(2);
		SessionManager.deleteSessionFromID(1);
		SessionManager.deleteSessionFromID(2);
	}

	@Test
	public void memberConnectionAndDeconnectionTest() {
		assertEquals(SessionManager.getNumberOfActiveSessions(), 0);
		Session member1Session, sameMember1Session;
		Session member2Session, sameMember2Session;
		try {
			// Authentication tests and type return
			member1Session = SessionManager.authenticate(1, "member1pass");
			assertNotNull(member1Session);
			assertTrue(member1Session instanceof MemberSession);
			member2Session = SessionManager.authenticate(2, "member2pass");
			assertNotNull(member2Session);
			assertTrue(member2Session instanceof MemberSession);
			assertEquals(SessionManager.getNumberOfActiveSessions(), 2);

			// Try to get the created sessions from id and hash
			sameMember1Session = SessionManager.getSessionFromID(1, member1Session.getUser().getHash());
			assertNotNull(sameMember1Session);
			assertEquals(member1Session, sameMember1Session);
			sameMember2Session = SessionManager.getSessionFromID(2, member2Session.getUser().getHash());
			assertNotNull(sameMember2Session);
			assertEquals(member2Session, sameMember2Session);
			assertEquals(SessionManager.getNumberOfActiveSessions(), 2);

			// Delete the sessions of each member from session and from ID
			assertTrue(SessionManager.deleteSession(sameMember1Session));
			assertNull(SessionManager.getSessionFromID(1, member1Session.getUser().getHash()));
			assertTrue(SessionManager.deleteSessionFromID(2));
			assertNull(SessionManager.getSessionFromID(2, sameMember2Session.getUser().getHash()));
			assertEquals(SessionManager.getNumberOfActiveSessions(), 0);

			// Reconnect same users to verify they can
			member1Session = SessionManager.authenticate(1, "member1pass");
			assertNotNull(member1Session);
			member2Session = SessionManager.authenticate(2, "member2pass");
			assertNotNull(member2Session);
			assertEquals(SessionManager.getNumberOfActiveSessions(), 2);
		} catch (ConnectionException e) {
			assertTrue(false);
		}

	}
		
	@Test
	public void memberConnectTwiceTest() {
		assertEquals(SessionManager.getNumberOfActiveSessions(), 0);
		Session member1Session, sameMember1Session;
		// Can't connect twice, should throw ConnectionExeption
		try {
			member1Session = SessionManager.authenticate(1, "member1pass");
			sameMember1Session = SessionManager.authenticate(1, "member1pass");
			assertTrue(false);
		} catch (ConnectionException e) {
			assertTrue(e.getErrorCode() == ConnectionException.USER_ALREADY_CONNECTED);
		}
	}
	
	@Test
	public void memberConnectWrongPasswordTest() {
		assertEquals(SessionManager.getNumberOfActiveSessions(), 0);
		Session member1Session, member2Session;
		// Can't connect with wrong password
		try {
			member1Session = SessionManager.authenticate(1, "anything");
			assertTrue(false);
		} catch (ConnectionException e) {
			assertTrue(e.getErrorCode() == ConnectionException.USER_PASSWORD_INVALID);
		}
		//Can't connect with password of other user
		try {
			member2Session = SessionManager.authenticate(2, "member1pass");
			assertTrue(false);
		} catch (ConnectionException e) {
			assertTrue(e.getErrorCode() == ConnectionException.USER_PASSWORD_INVALID);
		}
	}

}
