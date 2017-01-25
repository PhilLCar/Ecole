
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.Date;

/**
 * The Class ClaimDBManager, implementation of the DataBaseManager interface for
 * Claim, with some specific methods, namely
 * {@link #findClaimsForMemberID(int)},
 * {@link #findClaimsForSupplierID(int)} and
 * {@link #findClaimsFromDateToDate(Date, Date)}
 *
 * @author Gevrai Jodoin-Tremblay
 * @see Claim
 */
public class ClaimDBManager implements DataBaseManager<Claim> {

	private static final ArrayList<Claim> claims = DataBase.getDataBase().getClaims();

	@Override
	public int getUniqueID() {
		int highestID = -1;
		for (Claim c : claims){
			if (c.getClaimID() > highestID){
				highestID = c.getClaimID();
			}
		}
		return highestID+1;
	}

	@Override
	public Claim find(int id) {
		for (Claim c : claims)
			if (c.getClaimID() == id)
				return c;
		return null;
	}

	@Override
	public boolean exists(Claim key) {
		return claims.contains(key);
	}

	@Override
	public boolean add(Claim elem) {
		if (find(elem.getServID()) != null){
			return false;
		}
		return claims.add(elem);
	}

	@Override
	public boolean delete(Claim elem) {
		return claims.remove(elem);
	}

	@Override
	public Claim delete(int id) {
		for (int i=0; i < claims.size(); i++) {
			if (claims.get(i).getClaimID() == id) {
				return claims.remove(i);
			}
		}
		return null;
	}

	@Override
	public ArrayList<Claim> filter(String search) {
		ArrayList<Claim> filteredList = new ArrayList<Claim>();
		for (Claim c : claims) {
			if (c.getComment().contains(search)) {
				filteredList.add(c);
			}
		}
		return filteredList;
	}

	/**
	 * Find all the claims for a specific supplier, given this supplier's ID.
	 *
	 * @param id the id of the supplier
	 * @return the array list containing all claims in the DB for a supplier
	 */
	public ArrayList<Claim> findClaimsForSupplierID(int id) {
		ArrayList<Claim> filteredList = new ArrayList<Claim>();
		for (Claim c : claims) {
			if (c.getSupID() == id) {
				filteredList.add(c);
			}
		}
		return filteredList;
	}

	/**
	 * Find all the claims for a specific member, given this member's ID.
	 *
	 * @param id the id of the member
	 * @return the array list containing all claims in the DB for a member
	 */
    public ArrayList<Claim> findClaimsForMemberID(int id) {
        ArrayList<Claim> filteredList = new ArrayList<Claim>();
		for (Claim c : claims) {
			if (c.getMemID() == id) {
				filteredList.add(c);
			}
		}
		return filteredList;
    }

    /**
	 * Find all the claims in a date range given as arguments.
	 *
	 * @param fromDate the begin date of the looked up timeframe
	 * @param toDate the end date of the looked up timeframe
	 * @return the array list containing all claims in the DB for the date range
	 */
    public ArrayList<Claim> findClaimsFromDateToDate(Date fromDate,Date toDate){
    	ArrayList<Claim> filteredList = new ArrayList<Claim>();
    	for (Claim c : claims){
    		Date serviceDate = Date.from(c.getServiceDate().atStartOfDay().atZone(ZoneId.systemDefault()).toInstant());
    		if(serviceDate.after(toDate) && serviceDate.before(fromDate)) filteredList.add(c);
    	}
    	return filteredList;
    }


}
