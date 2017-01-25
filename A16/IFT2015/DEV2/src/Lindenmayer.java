import java.util.ArrayList;

public class Lindenmayer {
	private static double d = 2;
	private static double delta = 22.5;
	private static double x = 250;
	private static double y = 0;
	private static double a = 90;
	
	private static int n;
	private static String omega;
	
	private static ArrayList<String> rules = new ArrayList<String>();
	private static ArrayList<String> Frules = new ArrayList<String>();
	private static ArrayList<String> frules = new ArrayList<String>();

	public static void main(String[] args) {
		int i;
		for (i = 0; i < args.length; i++) {
			switch (args[i]) {
			case "-D":
				d = Double.parseDouble(args[++i]);
				continue;
			case "-delta":
				delta = Double.parseDouble(args[++i]);
				continue;
			case "-x":
				x = Double.parseDouble(args[++i]);
				continue;
			case "-y":
				y = Double.parseDouble(args[++i]);
				continue;
			case "-a":
				a = Double.parseDouble(args[++i]);
				continue;
			default:
				n = Integer.parseInt(args[i++]);
				omega = args[i++];
			}
			break;
		} for (; i < args.length; i++)
			rules.add(args[i]);
		System.out.println(gen());
	}
	
	private static String gen() {
		part();
		String ret = "/T:x " + x + " def\n" +
					 "/T:y " + y + " def\n" +
					 "/T:a " + a + " def\n\n" +
					 "/T:draw % dessine une arrête\n" +
					 "{\n" +
					 "\tT:x T:y moveto T:step\n" +
					 "\tT:x T:y lineto stroke\n" +
					 "} def\n\n/T:step % avance la tortue d'un pas\n" +
					 "{\n" +
					 "\t/T:x T:a cos " + d + " mul T:x add def\n" +
					 "\t/T:y T:a sin " + d + " mul T:y add def\n" +
					 "} def\n\n/open % sauvegarde l'état\n" +
					 "{\n" +
					 "\t3 dict begin\n" +
					 "} def\n\n/plus % tourne la tortue CCW\n" +
					 "{\n" +
					 "\t/T:a T:a " + delta + " add def\n" +
					 "} def\n\n/minus % tourne la tortue CW\n" +
					 "{\n" +
					 "\t/T:a T:a " + delta + " sub def\n" +
					 "} def\n\n";
		ret += genF() + genf();
		ret += n + "\n";
		ret += genRule(omega);
		return ret;
	}
	
	private static String genRule(String rule) {
		String ret = "";
		for (int i = 0; i < rule.length(); i++) {
			switch (rule.charAt(i)){
			case '+':
				ret += "plus ";
				break;
			case '-':
				ret += "minus ";
				break;
			case '[':
				ret += "open ";
				break;
			case ']':
				ret += "end ";
				break;
			case 'X':
				continue;
			default:
				if (i < rule.length() - 1)
					ret += "dup " + rule.charAt(i) + " ";
				else ret += rule.charAt(i) + " ";
			}
		}
		return ret;
	}
	
	private static void part() {
		for (String rule : rules) {
			if (rule.charAt(0) == 'F')
				Frules.add(rule.substring(2));
			else if (rule.charAt(0) == 'f')
				frules.add(rule.substring(2));
			else System.out.println("FAIL");
		}
	}
	
	private static String genCases(String tl, ArrayList<String> cases) {
		if (cases.size() == 0)
			return tl + "pop";
		if (cases.size() == 1) {
			return tl + "pop " + genRule(cases.get(0));
		}
		return tl + "dup " + (cases.size() - 1) + " eq {\n" +
			   tl + "\tpop " + genRule(cases.get(0)) + "\n" +
		       tl + "}{\n" +
			   genCases("\t" + tl, new ArrayList<String>(cases.subList(1, cases.size()))) + "\n" +
			   tl + "} ifelse";
	}
	
	private static String genF() {
		String ret = "/F % Règle d'expansion de F\n" +
					 "{\n" +
					 "\tdup 0 eq\n" +
					 "\t{\n" +
					 "\t\tT:draw\n\t\tpop\n" +
					 "\t}{\n" +
					 "\t\t1 sub\n";
		if (Frules.size() > 1) {
			ret += "\t\trand " + Frules.size() + " mod\n" +
					genCases("\t\t", Frules) + "\n";
		} else if (Frules.size() == 1) 
			ret += "\t\t" + genRule(Frules.get(0)) + "\n";
		ret += "\t} ifelse\n" +
			   "} def\n\n";
		return ret;
	}
	
	private static String genf() {
		String ret = "/f % Règle d'expansion de f\n" +
					 "{\n" +
					 "\tdup 0 eq\n" +
					 "\t{\n" +
					 "\t\tT:step\n\t\tpop\n" +
					 "\t}{\n" +
					 "\t\t1 sub\n";
		if (frules.size() > 1) {
			ret += "\t\trand " + frules.size() + " mod\n" +
					genCases("\t\t", frules) + "\n";
		} else if (frules.size() == 1) 
			ret += "\t\t" + genRule(frules.get(0)) + "\n";
		ret += "\t} ifelse\n" +
			   "} def\n\n";
		return ret;
	}
}
