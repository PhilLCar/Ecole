import java.time.LocalDateTime;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

/**
 * The Class MainAccountingJob, implementing a timer to print every reports
 * needed, at a specific time, always Saturday 00:00 (Friday 12PM).<p>
 * The timer runs the job in a separated thread so there is no bookkeeping to
 * do other than keep a reference to the instance created.<p>
 * See <a href="http://archive.oreilly.com/pub/a/java/archive/quartz.html"> Job
 * Scheduling in Java - O'Reilly </a> for the source inspiration of this class.
 * 
 * @author Gevrai Jodoin-Tremblay
 * @see ReportCreator
 */
public class MainAccountingJob extends TimerTask{

	/** The timer scheduling the job. */
	private Timer timer;
	/** The time at which the next job will be run */
	private Calendar nextJobTime;

	/**
	 * Instantiates a new MainAccountingJob doing everything automatically.
	 */
	public MainAccountingJob(){
		// Construct next date to run task from current date, pushing it to Saturday at 00:00
		nextJobTime = Calendar.getInstance();
		nextJobTime.set(Calendar.DAY_OF_WEEK, Calendar.SATURDAY);
		nextJobTime.set(Calendar.HOUR_OF_DAY, 0);
		nextJobTime.set(Calendar.MINUTE, 0);
		nextJobTime.set(Calendar.SECOND, 0);
		nextJobTime.set(Calendar.MILLISECOND, 0);
		// In case this sets the timestamp before the current time, push it next week
		if (nextJobTime.before(Calendar.getInstance())){
			nextJobTime.add(Calendar.DATE, 7);
		}
		//Delay before first execution
		long delay = nextJobTime.getTimeInMillis() - new Date().getTime();
		//Schedule task every Saturday at 00:00
		timer = new Timer();
		timer.scheduleAtFixedRate(this, delay, 7*24*60*60*1000);
	}

	/**
	 * Runs the method when the timer tells it to, which is at 'nextJobTime'
	 */
	@Override
	public void run() {
		System.out.println("MainAccountingJob started: printing neccessary reports");
		Calendar toDate = (Calendar) nextJobTime.clone();
		Calendar fromDate = (Calendar) nextJobTime.clone();
		fromDate.add(Calendar.DATE, -7);
		nextJobTime.add(Calendar.DATE, 7);
		// Printing all reports
		new ReportCreator().allReports(fromDate.getTime(), toDate.getTime());
		// EFTs update for the week
		ArrayList<Claim> claimsLastWeek = new ClaimDBManager().findClaimsFromDateToDate(fromDate.getTime(), toDate.getTime());
		ArrayList<EFT> newEfts = new ArrayList<EFT>();
		for (Claim c : claimsLastWeek){
			boolean claimTreated = false;
			for (EFT e : newEfts){
				// If a supplier gives many services for a week still 1 EFT/week
				if (e.getSupplierID() == c.getSupID()) {
					e.addAmmountDue(c.getPrice());
					claimTreated = true;
					break;
				}
			}
			if (!claimTreated)
				newEfts.add(new EFT(c.getSupID(),c.getPrice(),
						fromDate.toInstant().atZone(ZoneId.systemDefault()).toLocalDateTime(),
						toDate.toInstant().atZone(ZoneId.systemDefault()).toLocalDateTime()));
		}
		EftDBManager eftdbm = new EftDBManager();
		for (EFT e : newEfts)
			eftdbm.add(e);
	}
}