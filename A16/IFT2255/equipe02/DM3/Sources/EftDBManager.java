
import java.time.LocalDateTime;
import java.util.ArrayList;

/**
 * The Class EftDBManager, implementation of the DataBaseManager interface for
 * EFT, with some specific methods, namely {@link #findEftForSupplierID} and 
 * {@link #findEftInDateRange(LocalDateTime, LocalDateTime)}.
 * 
 * @author Gevrai Jodoin-Tremblay
 * @see EFT
 */
public class EftDBManager implements DataBaseManager<EFT> {

	private static final ArrayList<EFT> eftData = DataBase.getDataBase().getDataEFT();

	@Override
	public int getUniqueID() {
		int highestID = -1;
		for (EFT e : eftData){
			if (e.getEftID() > highestID){
				highestID = e.getEftID();
			}
		}
		return highestID+1;
	}

	@Override
	public EFT find(int id) {
		for (EFT e : eftData)
			if (e.getEftID() == id)
				return e;
		return null;
	}

	@Override
	public boolean exists(EFT key) {
		return eftData.contains(key);
	}

	@Override
	public boolean add(EFT elem) {
		if (find(elem.getEftID()) != null){
			return false;
		}
		return eftData.add(elem);
	}

	@Override
	public boolean delete(EFT elem) {
		return eftData.remove(elem);
	}

	@Override
	public EFT delete(int id) {
		for (int i=0; i < eftData.size(); i++) {
			if (eftData.get(i).getEftID() == id) {
				return eftData.remove(i);
			}
		}
		return null;
	}

	@Override
	public ArrayList<EFT> filter(String search) {
		// This method doesn't really make sense...
		throw new UnsupportedOperationException();
	}
	
	/**
	 * Find all the eft for a specific supplier, given this
	 * supplier's ID.
	 *
	 * @param id the id
	 * @return the array list containing all efts in the DB for a supplier
	 */
	public ArrayList<EFT> findEftForSupplierID(int id) {
		ArrayList<EFT> filteredList = new ArrayList<EFT>();
		for (EFT e : eftData) {
			if (e.getSupplierID() == id) {
				filteredList.add(e);
			}
		}
		return filteredList;
	}
	
	/**
	 * Find all the eft in a date range given as arguments. This will return all
	 * eft touching, even only partly, to the timeframe in question.
	 *
	 * @param begin the begin date of the looked up timeframe
	 * @param end the end date of the looked up timeframe
	 * @return the array list containing all efts in the DB for the date range
	 */
	public ArrayList<EFT> findEftInDateRange(LocalDateTime begin, LocalDateTime end) {
		ArrayList<EFT> filteredList = new ArrayList<EFT>();
		for (EFT e : eftData) {
			if (!(e.getFromDate().isAfter(end) || e.getToDate().isBefore(begin))){
				filteredList.add(e);
			}
		}
		return filteredList;
	}

}
