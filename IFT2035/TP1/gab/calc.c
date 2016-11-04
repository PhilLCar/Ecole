/*
* To change this license header, choose License Headers in Project Properties.
* To change this template file, choose Tools | Templates
* and open the template in the editor.
*/

/*
* File:   main.c
* Author: Gabriel
*
* Created on 4 octobre 2016, 22:16
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//struct variable { char lettre; int valeur; };
/*utilisable doit etre entre 0 et 1 et est initialisee a 0. Elle devient 1
* lorsque la variable est utilisable.*/

struct stackElement{ char valeur; // valeur de l'élément du stack
struct stackElement* next;  // prochain élément du stack
int isNegative; //1 pour négatif, -1 pour positif
};

struct stack{ struct stackElement* top; };
struct biggerAndSmallerStack { struct stack* bigger; struct stack* smaller; };

void push(struct stack* stk, char element){
	//Ajoute un stackElement sur le top du stack stk
	struct stackElement* current;
	current = malloc(sizeof(struct stackElement));
	current->valeur = element;
	current->isNegative = -1;

	if (stk->top == NULL){
		// S'éxecute lorsque la pile est vide
		stk->top = current;
		stk->top->next = NULL; //initialise le next à NULL
	}
	else{
		// S'éxecute lorsque la pile n'est pas vide
		current->next = stk->top;
		stk->top = current;
	}
}

char pop(struct stack* stk){
	//Retire l'élément au top de la pile et retourne sa valeur
	if (stk->top == NULL){
		// Attrape l'exception lorsque la pile est vide
		printf("Erreur! Pile Vide.\n");
	}
	else{
		// S'exécute lorsque la pile n'est pas vide
		char c;
		struct stackElement* current;
		current = malloc(sizeof(struct stackElement));
		current = stk->top;
		c = current->valeur;
		stk->top = stk->top->next;
		free(current); // libère l'espace mémoire

		return c;
	}
}

struct stack* reverse(struct stack* stk1){
	//Renverse les éléments de la pile
	//Doit obligatoirement être appelé sous forme [stk = reverse(stk)] puisque la pile initiale est vidée.
	if (stk1->top != NULL){
		//Si la pile n'est pas vide
		struct stack* stk2;
		stk2 = malloc(sizeof(struct stack));
		stk2->top = NULL;
		while (stk1->top != NULL){
			push(stk2, pop(stk1));
		}
		return stk2;
	}
	else{
		//Si la pile est vide
		printf("Erreur! Pile Vide non renversible puisque vide.\n");
	}
}

void changeNegativity(struct stack* stk){
	//multiply each of the isNegative in the stack by *-1 to change the negativity

	struct stackElement* se;
	se = malloc(sizeof(struct stackElement));
	se = stk->top;
	while (se != NULL){
		se->isNegative *= -1;
		se = se->next;
	}
	free(se);
}

void setNegative(struct stack* stk){
	//set each stack element isNegative to 1

	struct stackElement* se;
	se = malloc(sizeof(struct stackElement));
	se = stk->top;
	while (se != NULL){
		se->isNegative = 1;
		se = se->next;
	}
	free(se);
}

struct biggerAndSmallerStack* getBiggerStackNumber(struct stack* stk1, struct stack* stk2){
	/*Retourne un struct ordonant les stacks par taille. 
		Si les deux stacks sont identiques, le stk1 sera conservé
		comme étant le plus grand.*/

	//Initialisation des stacks temporaires qui deviendront les bigger et smaller
	struct stack* stk3;
	struct stack* stk4;
	stk3 = malloc(sizeof(struct stack));
	stk4 = malloc(sizeof(struct stack));
	stk3->top = NULL;
	stk4->top = NULL;
	
	//Reversion des stk1 et stk2 afin d'obtenir le bon ordre pour les stacks bigger/smaller
	stk1 = reverse(stk1);
	stk2 = reverse(stk2);

	//Initialisation des variables de comparaison
	char a;
	char b;

	//Initialisation du struct biggerAndSmallerStack qui retournera les deux stacks en ordre
	struct biggerAndSmallerStack* bass;
	bass = malloc(sizeof(struct biggerAndSmallerStack));
	bass->bigger = NULL;
	bass->smaller = NULL;

	while (stk1->top != NULL || stk2->top != NULL){
		//Tant que les deux stacks ne sont pas vide
		if (stk1->top != NULL){
			//Si le stk1 n'est pas vide
			a = pop(stk1);
			push(stk3, a);
		}
		else{
			//Si le stk1 est vide, le stk4 sera obligatoirement plus grand
			a = 0;
			bass->bigger = stk4;
			bass->smaller = stk3;
		}
		if (stk2->top != NULL){
			//Si le stk2 n'est pas vide
			b = pop(stk2);
			push(stk4, b);
		}
		else{
			//Si le stk2 est vide, le stk1 sera obligatoirement plus grand
			b = 0;
			bass->bigger = stk3;
			bass->smaller = stk4;
		}
		if (bass->bigger == NULL){
			//Si aucune sélection de stack plus grand n'a encore été faite (identiques jusqu'à présent)
			if (a > b){
				//stk1 à une plus grande valeur que stk2
				bass->bigger = stk3;
				bass->smaller = stk4;
			}
			else if (b > a){
				//stk2 à une plus grande valeur que stk1
				bass->bigger = stk4;
				bass->smaller = stk3;
			}
		}
	}
	if (bass->bigger == stk4){
		//Set tout les éléments du stack à négatif pour les prochains calculs
		setNegative(bass->bigger);
	}
	if (bass->bigger == NULL){
		//Si les deux ont une valeur identique
		bass->bigger = stk3;
		bass->smaller = stk4;
	}
	return bass;
}

void addition(struct stack* stk, struct stack* stk1, struct stack* stk2){
	//Méthode permettant l'addition de deux nombres

	//Initialisation des variables
	int a; // Nombre 1
	int b; // Nombre 2
	int keepOne; // Combien doit-on ajouter au prochain calcul
	
	//Initialisation du nombre d'ajout au prochain calcul à 0
	keepOne = 0;

	//Vérifie s'il s'agit d'une addition entre deux nombres négatifs
	int resultIsNegative;
	resultIsNegative = -1;
	if (stk1->top->isNegative == 1){
		resultIsNegative = 1;
	}

	push(stk, ' ');

	while (stk1->top != NULL || stk2->top != NULL){
		/*Additionne les unités avec les unités, dizaines avec les dizaines etc afin
			d'obtenir une précision infinie.*/
		if (stk1->top == NULL) {
			/*Transforme a en 0 pour le calcul puisque le nombre 1
				ne possède pas de chiffre dans cette colonne.*/
			a = 0;
		}
		else{
			//Donne à a la valeur du top de la pile du nombre1
			a = pop(stk1) - '0';
		}
		if (stk2->top == NULL){
			/*Transforme b en 0 pour le calcul puisque le nombre 2
				ne possède pas de chiffre dans cette colonne.*/
			b = 0;
		}
		else{
			//Donne à b la valeur du top de la pile du nombre2
			b = pop(stk2) - '0';
		}
		if (keepOne == 1){
			//Incrémente a si le calcul précédent était supérieur ou égal à 10
			a++;
			keepOne = 0;
		}
		if (a + b >= 10){
			//Garde 1 pour le prochain calcul si a+b est >= à 10 et donne les unités du résultat à stk
			keepOne = 1;
			push(stk, (a + b) - 10);
		}
		else{
			//Donne la valeur de a+b à stk
			push(stk, a + b);
		}
		if (resultIsNegative == 1){
			//Retourne un résultat négatif si les deux nombres sont négatifs
			stk->top->isNegative == 1;
		}
	}
}

void soustraction(struct stack* stk, struct stack* stk1, struct stack* stk2){
	//Méthode permettant la soustraction de deux nombres
	struct biggerAndSmallerStack* bass;

	//Par défaut, le résultat n'est pas négatif
	int resultIsNegative = -1;
	
	//Nombre utilisés pour le calcul
	int a; //Nombre 1
	int b; //Nombre 2

	int valeurDemprunt; //Valeur utilisé lorsqu'on emprunte
	valeurDemprunt = 10;

	//Par défaut, on ne vole pas de valeur au chiffre de gauche
	int stealOne = 0;

	//Retourne le plus grand et le plus petit
	bass = getBiggerStackNumber(stk1, stk2);

	//Donne les bonnes position de l'opération aux stacks
	stk1 = bass->bigger;
	stk2 = bass->smaller;

	if (stk1->top->isNegative == 1 && stk2->top->isNegative == -1){
		//Si le stack le plus gros est négatif et le plus petit positif, le résultat sera négatif.
		resultIsNegative = 1;
	}
	
	while (stk1->top != NULL || stk2->top != NULL){
		/*Soustrait les unités avec les unités, dizaines avec les dizaines etc afin
		d'obtenir une précision infinie.*/
		a = pop(stk1) - '0';

		if (stk2->top == NULL){
			//Si b n'a plus de chiffres, il devient 0
			b = 0;
		}
		else {
			//Si b a encore des chiffres, on continue de vider le stack
			b = pop(stk2) - '0';
		}
		if (stealOne == 1){
			//Si on a emprunter à notre voisin de gauche
			if (a != 0){
				//Si a n'est pas égal à 0
				a--;
				stealOne = 0;
				valeurDemprunt = 10;
			}
			else{
				//Si a est égal à 0
				stealOne = 1;
				valeurDemprunt = 9;
			}
		}
		if (a - b < 0){
			//Si le chiffre en bas est plus petit que celui d'en haut
			a = a + valeurDemprunt;
			push(stk, a - b);
			stealOne = 1;
		}
		else{
			//Si le chiffre du haut est plus grand ou égal à celui d'en bas
			if (stealOne == 1){
				//Ajoute la valeur d'emprunt si a emprunte et est emprunté
				a = a + valeurDemprunt;
			}
			push(stk, a - b);
		}
		if (resultIsNegative == 1){
			//Si le résultat est négatif, on donne la valeur de négativité au stack
			stk->top->isNegative = 1;
		}
	}

	//Retirer les valeurs inutiles du stack
	while (stk->top->valeur == 0){
		pop(stk);
	}

	//Bloc de débogage
	//printf("%d\n", stk->top->valeur);
	//printf("%d\n", stk->top->isNegative);
	//printf("%d\n", pop(stk));
	//printf("%d\n", stk->top->isNegative);
	//printf("%d\n", pop(stk));
}

void multiplication(struct stack* stk, struct stack* stk1, struct stack* stk2){
	//Méthode permettant la multiplication entre deux nombres

	//Initialise les variables nécessaires au calcul
	struct stack* stkAdd1;
	struct stack* stkAdd2;
	struct stack* stkAddResult;
	struct stackElement* current;
	stkAdd1 = malloc(sizeof(struct stack));
	stkAdd2 = malloc(sizeof(struct stack));
	stkAddResult = malloc(sizeof(struct stack));
	current = malloc(sizeof(struct stackElement));
	current->next = NULL;
	stkAdd1->top = NULL;
	stkAdd2->top = NULL;
	stkAddResult->top = NULL;

	//Initialise les nombres utilisés pour la multiplication
	int a; //Nombre 1
	int b; //Nombre 2
	int keepNumber; //Chiffre à ajouter à la prochaine itération
	keepNumber = 0;

	//Initialise si le résultat sera négatif ou non
	int resultIsNegative;
	resultIsNegative = -1;
	if ((stk1->top->isNegative == 1 && stk2->top->isNegative == -1) ||
		(stk1->top->isNegative == -1 && stk2->top->isNegative == 1)){
		//Entre si le résultat va être négatif
		resultIsNegative = 1;
	}

	while (stk1->top != NULL){
		/*Multiplie la valeur des unités par toutes les valeurs du nombre 2,
		la valeur des dizaines par toutes les valeurs du nombre 2, etc.*/
		a = pop(stk1) - '0';

		//RESET stkAdd1 à chaque boucle  MAY BE PROBLEMATIC
		stkAdd1->top = NULL;

		//Stack temporaire
		struct stack* temp;
		temp = malloc(sizeof(struct stack));
		temp->top = NULL;

		while (stk2->top != NULL){
			b = pop(stk2) - '0';
			if ((a * b) + keepNumber >= 10){
				int tempInt = (a * b) / 10;
				push(stkAdd1, (a * b) - (tempInt * 10) + keepNumber + '0');
				keepNumber = tempInt;
			}
			else{
				push(stkAdd1, (a * b) + keepNumber + '0');
			}
			push(temp, b + '0');
		}
		stk2 = reverse(temp);
		free(temp);
		if (stkAdd2->top != NULL){
			//do addition entre stkAdd1 et stkAdd2 into stkAddResult
			
			push(stkAddResult, stkAdd2->top->valeur);
			
			/*addition(stkAddResult, stkAdd1, stkAdd2);
			stkAdd2 = reverse(stkAddResult);
			pop(stkAdd2);*/
		}
		else{
			push(stkAdd2, pop(stkAdd1));
			stkAdd2 = reverse(stkAdd2);
		}
	}
	push(stkAddResult, pop(stkAdd1));
	

	stk = stkAddResult;
	printf("%c", pop(stk));
	printf("%c", pop(stk));
	printf("%c", pop(stk));

}

int main(int argc, char** argv)
{
	//Initialisation du stack
	struct stack *stk;
	stk = malloc(sizeof(struct stack));
	stk->top = NULL; //Initialise le top à null. Important de garder.

	//Affichage initial sur la console
	printf("> ");
	fflush(stdout);
	
	//Lecture du input de l'utilisateur et remplissage du stack
	char c = getchar();
	while (c != '\n'){
		push(stk, c);
		c = getchar();
	}
	//Reversion du stack
	stk = reverse(stk);

	//Initialisation des stack des nombres à évaluer
	struct stack *stk1;
	stk1 = malloc(sizeof(struct stack));
	stk1->top = NULL;
	struct stack *stk2;
	stk2 = malloc(sizeof(struct stack));
	stk2->top = NULL;

	//Rempli la pile stk1 avec le premier nombre
	while (stk->top->valeur != ' '){
		/*@TODO: Include exceptions si y'a pas de ' ' avant le EOF et
			vérifier que seulement des chiffres ou des variables sont acceptés. */
		push(stk1, pop(stk));
	}
	pop(stk); //Retire le ' ' de la pile
	//Rempli la pile stk2 avec le deuxième nombre
	while (stk->top->valeur != ' '){
		/*@TODO: Include exceptions si y'a pas de ' ' avant le EOF et
		vérifier que seulement des chiffres ou des variables sont acceptés. */
		if (stk->top->valeur == '='){
			printf("$$$ money shot $$$");
		}
		push(stk2, pop(stk));
	}
	pop(stk); // Retire le ' ' de la pile

	//Prend le symbole d'opération
	char operateur = pop(stk);

	switch (operateur){
		case('+') :
			//Opérateur est une addition
			if ((stk1->top->isNegative == -1 && stk2->top->isNegative == -1) ||
				(stk1->top->isNegative == 1 && stk2->top->isNegative == 1)){
				//Si c'est une addition entre deux nombres positifs ou deux nombres négatifs
				addition(stk, stk1, stk2);
			}
			else{
				//Si c'est une addition entre un nombre positif et un nombre négatif
				soustraction(stk, stk1, stk2);
			}
			break;
		case('-') :
			if ((stk1->top->isNegative == -1 && stk2->top->isNegative == -1) ||
				(stk1->top->isNegative == 1 && stk2->top->isNegative == 1)){
				//Si c'est une soustraction entre deux nombres positifs ou deux nombres négatifs
				soustraction(stk, stk1, stk2);
			}
			else{
				//Si c'est une soustraction entre un nombre positif et un nombre négatif
				addition(stk, stk1, stk2);
			}
				break;
		case('*') :
			multiplication(stk, stk1, stk2);
			break;

		default:
			printf("Erreur! Opération inexistante.");
			break;
	}

	getchar(); //Uniquement pour afficher la console
}
