import java.io.BufferedReader;
import java.io.FileReader;
import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

/**
 * The Class ReceivableAccountsJob, implementing a timer scheduling everyday at
 * 21:00 the job to check the member status file sent by ACME everyday, and
 * detailing the change in status of any member. It updates the database with
 * the new statuses and sets itself for the next day at the same time. <p>
 * The sent file should imperatively be correctly, formated with each line
 * describing an integer memberID and an integer status. See 
 * {@link #parseLineAndModifyUserStatus(String)} documentation for more
 * details about the correct format <p>
 * The timer runs the job in a separated thread so there is no bookkeeping to
 * do other than keep a reference to the instance created.<p>
 * See <a href="http://archive.oreilly.com/pub/a/java/archive/quartz.html"> Job
 * Scheduling in Java - O'Reilly </a> for the source inspiration of this class.
 *
 * @author Gevrai Jodoin-Tremblay
 * @see Member
 */
public class ReceivableAccountsJob extends TimerTask {

	/** The file location/name where ACME sends account informations. */
	private static final String MEMBERSTATUS_FILENAME = "Database/MemberStatusFromACME.txt";

	/** The time at which the next job will be run */
	private Calendar nextJobTime;

	/** The timer scheduling the job. */
	private Timer timer;

    /**
     * Instantiates a new receivable accounts job doing everything automatically.
     *
     *
     */
    public ReceivableAccountsJob(){
        // Construct next date to run task from current date, pushing it to 21:00
		nextJobTime = Calendar.getInstance();
		nextJobTime.set(Calendar.HOUR_OF_DAY, 21);
		nextJobTime.set(Calendar.MINUTE, 0);
		nextJobTime.set(Calendar.SECOND, 0);
		nextJobTime.set(Calendar.MILLISECOND, 0);
		// In case this sets the timestamp before the current time, push it next day
		if (nextJobTime.before(Calendar.getInstance())){
			nextJobTime.add(Calendar.DATE, 1);
		}
		//Delay before first execution
		long delay = nextJobTime.getTimeInMillis() - new Date().getTime();
		//Schedule task every day at 21:00
		timer = new Timer();
		timer.scheduleAtFixedRate(this, delay, 7*24*60*60*1000);
    }

    /**
     * Parse a line of text that should contain a memberID and its new status,
     * and then applies this status to the concerned member<p>
     * The line should have exactly two tab separated integer arguments, the
     * first being the memberID and the second the new status. 
     * @see Member for correct values of the status.
     *
     * This method will fail and return false if:
     * <ul>
     * <li> It has more than two arguments;
     * <li> Either argument is not a parsable integer;
     * <li> The first integer, MemberID, doesn't correspond to any member in the database;
     * <li> The ID doesn't correspond to a member (supplier or supervisor?)
     * <li> The new status value is invalid
     * </ul>
     *
     * @param line text line containing a memberID and its new status
     * @return true if all went well, false otherwise
     */
    public boolean parseLineAndModifyUserStatus(String line){
		int userID, newStatus;
		User modifiedUser;
		UserDBManager udbm = new UserDBManager();
		String[] info = line.split("\t", 2);
		try {
			userID = Integer.parseInt(info[0]);
			newStatus = Integer.parseInt(info[1]);
			modifiedUser = udbm.find(userID);
			if (modifiedUser == null) {
				System.out.format("Couldn't find user in database : ID=%d\n", userID);
				return false;
			}
			if (!(modifiedUser instanceof Member)) {
				System.out.format("User with ID=%d is type %s, not member.\n", userID, modifiedUser.getClass().getName());
				return false;
			}
			if (((Member)modifiedUser).setStatus(newStatus)) {
				return true;
			}
			System.out.format("Trying to assign invalid status number %d to member with ID=%d.\n",newStatus, userID);
		} catch (NumberFormatException e) {
			System.out.println("Invalid entry : " + line);
		}
		return false;
    }

	/**
	 * Runs the method parsing MEMBERSTATUS_FILENAME when the timer tells it to, which is at 'nextJobTime'
	 */
    @Override
	public void run() {
		System.out.println("ReceivableAccountsJob started: updating member statuses from " + MEMBERSTATUS_FILENAME);
		nextJobTime.add(Calendar.DATE, 1);
		try {
			FileReader fr = new FileReader(MEMBERSTATUS_FILENAME);
			BufferedReader br = new BufferedReader(fr);
			String line;
			while ((line = br.readLine()) != null) {
				parseLineAndModifyUserStatus(line);
			}
			br.close();
			fr.close();
		} catch (Exception e) {
			e.printStackTrace(System.err);
		}
	}
}
