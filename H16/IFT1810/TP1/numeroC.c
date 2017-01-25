/*------------------------------------------*/
/* AUTEUR:  Philippe Caron                  */ 
/* DATE:    11 Feb 2016                     */
/* COURS:   IFT1810                         */
/* TRAVAIL: TP1 Num. C                      */
/*------------------------------------------*/

#include <stdio.h>

// Calcul de la somme avec do..while
double dowhilesum()
{
  float denom = 1;
  float sum = 0;
  do {
    sum += 1 / denom;
    denom += 2;
  } while (denom < 9999);
  return sum;
}

// Calcul de la somme avec while
double whilesum()
{
  float denom = 1;
  float sum = 0;
  while (denom <= 9999) {
    sum += 1 / denom;
    denom += 2;
  }
  return sum;
}

// Calcul de la somme avec for
double forsum()
{
  float sum = 0;
  float denom;
  for (denom = 1; denom < 9999; denom += 2) sum += 1 / denom;
  return sum;
}

int main()
{
  printf("Somme calculée avec le do...while : %f\n", dowhilesum());
  printf("Somme calculée avec le while :      %f\n", whilesum());
  printf("Somme calculée avec le for :        %f\n", forsum());
}