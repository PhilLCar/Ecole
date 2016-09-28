//------------------------------------------------------
// module  : FunctionDemo4.h
// auteur  : Mignotte Max
// date    :
// version : 1.0
// langage : C++
// labo    : DIRO
// note    :
//------------------------------------------------------
// 

#ifndef FONCTIONDEMO_H
#define FONCTIONDEMO_H
//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//------------------------------------------------
// CONSTANTES & DEFINITIONS ----------------------
//------------------------------------------------
#define CARRE(X)   ((X)*(X))
#define PI  3.14159
#define GREY_LEVEL 255
#define NBCHAR 200

//------------------------------------------------
// PROTOTYPE -------------------------------------
//------------------------------------------------

//>Matrix
float*    fmatrix_allocate_1d(int);
float**   fmatrix_allocate_2d(int,int);
void  free_fmatrix_1d(float*);
void  free_fmatrix_2d(float**);

//>Load/Save File
float** LoadImagePgm(char*,int*,int*);
void SaveImagePgm(char*,float**,int,int);

//>Fourier
void IFFTDD(float**,float**,int,int);
void FFTDD(float**,float**,int,int);
void fourn(float*,unsigned long*,int,int);

//>Degradation
float randomize(void);
float gaussian_noise(float,float);
void degradation(float**,float**,int,int,int,int,float);
void convolveUB(float**,float**,int,int,int);
void convolvePAR(float**,float**,int,int,int);
void convolveGAUSS(float**,float**,int,int);


#endif
