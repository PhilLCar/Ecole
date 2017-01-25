import java.util.ArrayList;

/**
 * The Class UserDBManager, implementation of the DataBaseManager interface for
 * Service.
 * 
 * @author Philippe Caron
 * @author Gevrai Jodoin-Tremblay 
 * @see User
 */
public class UserDBManager implements DataBaseManager<User> {

	private static final ArrayList<User> users = DataBase.getDataBase().getUsers();

	@Override
	public int getUniqueID() {
		int highestID = -1;
		for (User u : users){
			if (u.getUserID() > highestID){
				highestID = u.getUserID();
			}
		}
		return highestID+1;
	}

	@Override
	public User find(int id) {
		for (User u : users)
			if (u.getUserID() == id)
				return u;
		return null;
	}

	@Override
	public boolean exists(User key) {
		return users.contains(key);
	}

	@Override
	public boolean add(User elem) {
		if (find(elem.getUserID()) != null) {
			return false;
		}
		return users.add(elem);
	}

	@Override
	public boolean delete(User elem) {
		return users.remove(elem);
	}

	@Override
	public User delete(int id) {
		for (int i = 0; i < users.size(); i++) {
			if (users.get(i).getUserID() == id) {
				return users.remove(i);
			}
		}
		return null;
	}

	@Override
	public ArrayList<User> filter(String search) {
		ArrayList<User> filteredList = new ArrayList<User>();
		for (User u : users) {
			if (u.getName().contains(search)) {
				filteredList.add(u);
			}
		}
		return filteredList;
	}
	
}