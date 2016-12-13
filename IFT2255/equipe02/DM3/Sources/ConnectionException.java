
public class ConnectionException extends Exception{
	public static final int USER_ALREADY_CONNECTED = 1;
	public static final int USER_PASSWORD_INVALID = 2;
	public static final int USER_INVALID = 3;	
	
	private String errorMessage;
	private int errorCode;

	public ConnectionException(String errorMessage, int errorCode) {
		this.errorMessage = errorMessage;
		this.errorCode = errorCode;
	}

	public String getErrorMessage() {
		return errorMessage;
	}

	public int getErrorCode() {
		return errorCode;
	}
}
