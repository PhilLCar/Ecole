package numeroD;

public class TestPersonne {
	public static Personne[] pers = {
			new Personne("16/11/1992", 2),
			new Personne("02/05/1990"),
			new Personne("23/05/1996", 5),
			new Personne("19/02/1985", 0),
			new Personne("30/05/1991", 2)
	};
	
	public static void main(String[] args) {
		printTable();
		
		System.out.println();
		System.out.println("Personne consommant le plus : " + findMax() + ".");
		System.out.println();
		
		for (Personne p : pers) p.decConsumption();
		printTable();
		
		System.out.println();
		swapSort();
		
		printTable();
		
		System.out.println();
		System.out.println("Il y a " + numberOf(5) + " personnes qui sont nées en mai.");
	}
	
	private static void printTable() {
		System.out.println("      Indice      Tableau pers");
		System.out.println("---------------------------------------------------");
		for (int i = 0; i < pers.length; i++) 
			System.out.println("      " + i + "           " + pers[i]);
	}
	
	private static Personne findMax() {
		Personne max = pers[0];
		for (int i = 1; i < pers.length; i++)
			if (pers[i].compareTo(max) > 0)
				max = pers[i];
		return max;
	}
	
	private static void swapSort() {
		Personne p;
		for (int i = 0; i < pers.length; i++)
			for (int j = 0; j < pers.length - 1; j++)
				if (pers[i].compareTo(pers[j]) > 0) {
					p = pers[j].Clone();
					pers[j] = pers[i].Clone();
					pers[i] = p;
				}
	}
	
	private static int numberOf(int month) {
		int num = 0;
		for (Personne p : pers)
			if (p.monthOfBirth() == month) num++;
		return num;
	}
}
