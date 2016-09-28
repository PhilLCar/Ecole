package numeroD;

public class Personne {
	private String naissance;
	private int nbCafe;
	private boolean def_flag = false;
	
	public Personne(String naissance, int nbCafe) {
		this.naissance = naissance;
		this.nbCafe = nbCafe;
	}
	
	public Personne(String naissance) {
		this(naissance, 1);
		def_flag = true;
	}
	
	public void decConsumption() {
		if (nbCafe > 0) nbCafe--;
		def_flag = false;
	}
	
	public int monthOfBirth() {
		return Integer.parseInt(naissance.substring(3, 5));
	}
	
	@Override
	public String toString() {
		return String.format("\"%s\"  %-20s", naissance,
				(nbCafe <= 1 ? nbCafe + " tasse" : nbCafe + " tasses") +
				(def_flag ? " (par défaut)" : ""));
	}
	
	public int compareTo(Personne b) {
		if (b.nbCafe > nbCafe) return -1;
		if (b.nbCafe < nbCafe) return 1;
		return 0;
	}
	
	public Personne Clone() {
		return new Personne(naissance, nbCafe);
	}
}
