using System;
using System.IO;

namespace Orion {
    class Program {
	public static void Main(string[] args) {
	    String text;
	    using (StreamReader sr = new StreamReader(args[0])) {
		text = sr.ReadToEnd();
	    }
	    using (StreamWriter sw = new StreamWriter(args[0].Replace(".cs", ".pp"))) {
		sw.Write(text);
	    }
	    Console.WriteLine(text);
	}

	
    }

    class _Namespace {
	List<_Class> classes;
	List<_Namespace> namespaces;
    }

    class _Class {
	List<_Class> classes;
	List<_Method> methods;
    }

    class _Method {
	
    }
}
