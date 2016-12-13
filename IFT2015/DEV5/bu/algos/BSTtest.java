
public class BSTtest {

	public static void main(String[] args) {
		BinarySearchTree<String> test = new BinarySearchTree<String>();
		// Fill
		Node<String> root = test.root();
		root.setleft("B");
		root.setright(new Node<String>(
				"D", new Node<String>(
						new Node<String>("G", "A"), new Node<String>(
								"E", new Node<String>("F", "C")))));
		
		test.fillIDX();
		
		System.out.println("ARBRE BINAIRE:");
		System.out.print(test);
		System.out.println();
		
		System.out.println("5.1 a)");
		System.out.print("key   ");
		for (String s : test.key()) {
			System.out.print(s + " ");
		}
		System.out.println();
		System.out.print("idx[] ");
		for (int i : test.idx()) {
			System.out.print(i + " ");
		}

		System.out.println("\n");
		System.out.println("5.1 b)");
		
		System.out.println("                                 L R N");
		System.out.println("--------------------------------------");
		System.out.print("Avec les fonctions simples:      ");
		System.out.print(test.L(root.right()) + " ");//1
		System.out.print(test.R(root.right()) + " ");//6
		System.out.println(test.N(root.right()));//6
		test.fillH();
		int[] lrn = test.fillH(root.right(), true);
		System.out.print("Avec la fonction de remplissage: ");
		System.out.print(lrn[0] +  " ");//1
		System.out.print(lrn[1] +  " ");//6
		System.out.println(lrn[2]);//6
		
		System.out.println();
		System.out.println("5.2");
		System.out.println("H[]  L R");
		for (int i = 0; i < test.H().length; i++) {
			System.out.println(" "  + i + "  [" + test.H()[i][0] + " " + test.H()[i][1] + "]");
		}
		
		System.out.println();
		BinarySearchTree<String> b = new BinarySearchTree<String>();
		BinarySearchTree<String> c = new BinarySearchTree<String>();
		BinarySearchTree<String> e = new BinarySearchTree<String>();
		b.root().setright("Gorille");
		b.root().setleft(new Node<String>(
				new Node<String>("Pygmé", "Chimpanzé"),
				"Homme"));
		c.root().setright(new Node<String>("Homme", "Gorille"));
		c.root().setleft(new Node<String>("Pygmé", "Chimpanzé"));
		e.root().setright("Gorille");
		e.root().setleft(new Node<String>(
				"Homme",
				new Node<String>("Pygmé", "Chimpanzé")));
		System.out.println("Arbre b:\n" + b);
		System.out.println("Arbre c:\n" + c);
		System.out.println("Arbre e:\n" + e);
		
		System.out.println("c est le même que e? " + c.equals(e));
		System.out.println("b est le même que e? " + b.equals(e));
	}

}
