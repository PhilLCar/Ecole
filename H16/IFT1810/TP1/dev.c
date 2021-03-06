#include <stdio.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

int main()
{
  printf("Quel numéro voulez-vous exécuter?\n" +
	 "(A) Calcul de prime\n" +
	 "(B) Information du patient\n" +
	 "(C) Somme\n" +
	 "(Q) Quitter...\n\n");
  char c = 'M';
  while (c == 'M')
    {
      for (c = toupper(getchar()); c < 0x41 && c > 0x43; c = toupper(getchar()))
	{
	  if (c == 'Q') return 0;
	  printf("\"%c\" n'est pas un choix valide. SVP entrez une lettre " +
		 "entre A et C, ou Q pour quitter\n", c);
	}
      switch (c)
	{
	case 'A':
	  system(CLEAR);
	  c = numA();
	  break;
	case 'B':
	  system(CLEAR);
	  c = numB();
	  break;
	case 'C':
	  system(CLEAR);
	  c = numC();
	  break;
	}
    }
  return c;
}
