package numeroC;

public class numeroC {
	private static final int PARITY_EVEN = 0;
	private static final int PARITY_ODD = 1;
	static String telUDM = "5143436111", telJean = "4503861504";

	public static void main(String[] args) {
		System.out.println("Téléphone d'UdM : " + format(telUDM));
		System.out.println("Telephone de Jean : " + format(telJean));
		
		System.out.println();
		
		System.out.println("Il y a " + numberOf(telUDM, 3) +
				" fois le chiffre 3 dans le numéro de téléphone d'UdM");
		System.out.println("Il y a " + numberOf(telUDM, 1) +
				" fois le chiffre 1 dans le numéro de téléphone d'UdM");
		System.out.println("Il y a " + numberOf(telJean, 4) +
				" fois le chiffre 4 dans le numéro de téléphone de Jean");
		
		System.out.println();
		
		System.out.println("Les chiffres impairs communs de ces 2 téléphones :" +
							compare(telUDM, telJean, PARITY_ODD));
		System.out.println("Les chiffres pairs communs de ces 2 téléphones :" +
				compare(telUDM, telJean, PARITY_EVEN));
	}
	
	private static String format(String tel) {
		String t1, t2, t3;
		t1 = tel.substring(0, 3);
		t2 = tel.substring(3, 6);
		t3 = tel.substring(6);
		return "(" + t1 + ") " + t2 + "-" + t3;
	}	
	
	private static int numberOf(String tel, int key) {
		int num = 0;
		char k = (char)(key + 0x30);
		for (int i = 0; tel.indexOf(k, i) != -1; i = tel.indexOf(k, i) + 1)
			num++;
		return num;
	}
	
	private static String compare(String tel1, String tel2, int flags) {
		String r = "";
		for (char i = (char)(0x30 + flags); i < 0x3A; i += 2)
			if (tel1.indexOf(i) != -1 && tel2.indexOf(i) != -1)
				r += " " + i;
		return r;
	}
}