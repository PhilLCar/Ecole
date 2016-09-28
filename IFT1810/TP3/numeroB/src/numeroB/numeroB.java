package numeroB;

public class numeroB {
	// CONSTANTES
	private static final int MIN = 1;
	private static final int MAX = 0;
	
	// VARIABLES GLOBALES
	private static char[] poste  = { 'A', 'P', 'O', 'P', 'A', 'P', 'A', 'P' };
	private static int[]  nbCafe = {  3 ,  1 ,  5 ,  1 ,  2 ,  1 ,  0 ,  3  };
	private static int[]  age    = { 25 , 18 , 23 , 20 , 50 , 24 , 52 , 29  };
	private static int    tabCounter = 0;
		
	public static void main(String[] args) {
		printHeader();
		prettyPrint("Poste, consommation de café, et âge des employés");

		System.out.println();
		printNumberOf('P');
		printNumberOf('S');
		
		System.out.println("Le programmeur qui consomme le moins de café en consomme " +
							getMinMax(nbCafe, 'P', MIN) + ".");
		
		System.out.println();
		System.out.println("L'analyste le plus âgé a " +
							getMinMax(age, 'A', MAX) + " ans.");
		
		System.out.println();
		System.out.println("Les programmeurs consomment en moyenne " +
							getMean(nbCafe, 'P') + " cafés.");
		
		System.out.println();
		System.out.println("La moyenne d'âge de tout les employés est " +
							getMinMax(age, 'A', MAX) + " ans.");
		
		swapSort();
		
		System.out.println();
		prettyPrint("Premier tableau organisé selon l'âge par ordre croissant");
	}
	
	private static void printHeader() {
		System.out.println("NUMÉRO B:");
		System.out.println("**********************************************\n");
	}
	
	private static void prettyPrint(String descr) {
		System.out.println("Tableau (" + ++tabCounter + "):");
		System.out.println("\t" + descr);
		
		printSeparator('=');
		
		System.out.print("| Postes |");
		for (int i = 0; i < 8; i++) System.out.print("|   " + poste[i] + "  ");
		System.out.println("|");
		
		printSeparator('-');
		
		System.out.print("| Cafés  |");
		for (int i = 0; i < 8; i++) System.out.print("|   " + nbCafe[i] + "  ");
		System.out.println("|");
		
		printSeparator('-');
		
		System.out.print("| Age    |");
		for (int i = 0; i < 8; i++) System.out.print("|  " + age[i] + "  ");
		System.out.println("|");
		
		printSeparator('=');
	}
	
	private static void printSeparator(char sep) {
		for (int i = 0; i < 67; i++) System.out.print(sep);
		System.out.println();
	}
	
	private static String getName(char poste) {
		switch (poste) {
		case 'P':
			return " programmeur";
		case 'O':
			return " opérateur";
		case 'A':
			return " analyste";
		case 'S':
			return " secrétaire";
		default:
			return " poste avec cet attribu";
		}
	}
	
	private static void printNumberOf(char key) {
		int num = 0;
		for (int i = 0; i < poste.length; i++)
			if (poste[i] == key) num++;
		System.out.println("Il y a au total " + num + getName(key) 
											  + (num > 1 ? "s.\n" : ".\n"));
	}
	
	private static int getMinMax(int[] comp, char key, int func) {
		int c = func * 100;
		for (int i = 0; i < comp.length; i++) {
			if (poste[i] == key) {
				if (0 != func) {
					if (comp[i] < c) c = comp[i];
				} else if (comp[i] > c) c = comp[i];
			}
		}
		return c;
	}
	
	private static float getMean(int comp[], char key) {
		int sum = 0, num = 0;
		for (int i = 0; i < comp.length; i++)
			if (poste[i] == key || key == '*') {
				sum += comp[i];
				num++;
			}
		return (float)sum / (float)num;
	}
	
	private static void swapSort() {
		int c;
		for (int i = 0; i < age.length - 1; i++)
			for (int j = 0; j < age.length - 1; j++)
				if (age[j] > age[j + 1]) {
					
					// Swap poste
					c = poste[i];
					poste[j] = poste[j + 1];
					poste[j + 1] = (char)c;
					
					// Swap nbCafe
					c = nbCafe[j];
					nbCafe[j] = nbCafe[j + 1];
					nbCafe[j + 1] = c;
					
					// Swap age
					c = age[j];
					age[j] = age[j + 1];
					age[j + 1] = c;
				}
	}
}
































