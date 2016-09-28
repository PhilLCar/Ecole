/*---------------------------------------------------*/
/* module  : FonctionDemo7.c                         */
/* auteur  : Mignotte Max                            */
/* date    : --/--/2010                              */    
/* langage : C                                       */
/* labo    : DIRO                                    */
/*---------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "FonctionDemo7.h"

/*------------------------------------------------*/
/* FONCTIONS -------------------------------------*/
/*------------------------------------------------*/
/*---------------------------------------------------------*/
/*  Alloue de la memoire pour une matrice 1d de float      */
/*---------------------------------------------------------*/
float* fmatrix_allocate_1d(int hsize)
 {
  float* matrix;

  matrix=(float*)malloc(sizeof(float)*hsize); 
  if (matrix==NULL) printf("probleme d'allocation memoire");

  return matrix; 
 }

/*----------------------------------------------------------*/
/*  Alloue de la memoire pour une matrice 2d de float       */
/*----------------------------------------------------------*/
float** fmatrix_allocate_2d(int vsize,int hsize)
 {
  int i;
  float** matrix;
  float *imptr;

  matrix=(float**)malloc(sizeof(float*)*vsize);
  if (matrix==NULL) printf("probleme d'allocation memoire");

  imptr=(float*)malloc(sizeof(float)*hsize*vsize);
  if (imptr==NULL) printf("probleme d'allocation memoire");
 
  for(i=0;i<vsize;i++,imptr+=hsize) matrix[i]=imptr;
  return matrix;
 }

//----------------------------------------------------------
// Allocation matrix 3d float
//----------------------------------------------------------
float*** fmatrix_allocate_3d(int dsize,int vsize,int hsize)
 {
  int i;
  float*** matrix;

  matrix=(float***)malloc(sizeof(float**)*dsize); 

  for(i=0;i<dsize;i++)
    matrix[i]=fmatrix_allocate_2d(vsize,hsize);
  return matrix;
 }

/*----------------------------------------------------------*/
/* Libere la memoire de la matrice 1d de float              */
/*----------------------------------------------------------*/
void free_fmatrix_1d(float* pmat)
 { 
  free(pmat); 
 }

//----------------------------------------------------------*/
/* Libere la memoire de la matrice 2d de float              */
/*----------------------------------------------------------*/
void free_fmatrix_2d(float** pmat)
 { 
  free(pmat[0]);
  free(pmat);
 }

//----------------------------------------------------------
// Free Memory 3d de float
//----------------------------------------------------------
void free_fmatrix_3d(float*** pmat,int dsize)
{
 int i;
 for(i=0;i<dsize;i++)
  {
   free(pmat[i][0]);
   free(pmat[i]);
   }
 free(pmat);
}

/*--------------*/
/* FOURIER FFT--*/
/*--------------*/
/*------------------------------------------------*/
/*  FOURN ----------------------------------------*/
/*------------------------------------------------*/
void fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
						tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}

/*----------------------------------------------------------*/
/* FFT1D                                                    */
/*----------------------------------------------------------*/
void FFT1D(float* vctR,float* vctI,int lgth)
{
 int i;
 float* data;
 unsigned long* nn;

 /*allocation memoire*/
 data=(float*)malloc(sizeof(float)*((2*lgth)+1));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*2); 
 nn[1]=lgth; 

 /*Remplissage de data*/
 for(i=0;i<lgth;i++)  
   { data[(2*i)+1]=vctR[i];
     data[(2*i)+2]=vctI[i]; 
    }

 /*FFTDD*/
 fourn(data,nn,1,FFT);

 /*Resultat*/
 for(i=0;i<lgth;i++)
  { vctR[i]=data[(2*i)+1];
    vctI[i]=data[(2*i)+2];  }

 /*Liberation memoire*/
 free(data);
 free(nn);
}


/*----------------------------------------------------------*/
/* IFFT1D                                                   */
/*----------------------------------------------------------*/
void IFFT1D(float* vctR,float* vctI,int lgth)
{
 int i;
 float* data;
 unsigned long* nn;

 /*allocation memoire*/
 data=(float*)malloc(sizeof(float)*((2*lgth)+1));
 nn=(unsigned long*)malloc(sizeof(unsigned long)*2); 
 nn[1]=lgth; 

 /*Remplissage de data*/
 for(i=0;i<lgth;i++)  
   { data[(2*i)+1]=vctR[i];
     data[(2*i)+2]=vctI[i]; }

 /*FFTDD*/
 fourn(data,nn,1,IFFT);

 /*Resultat*/
 for(i=0;i<lgth;i++)
  { vctR[i]=data[(2*i)+1];
    vctI[i]=data[(2*i)+2];  }

 /*Liberation memoire*/
 free(data);
 free(nn);
}

//--------------------------//
//--- OUTILS SPECTRAL 1D ---//
//--------------------------//
/*----------------------------------------------------------*/
/* ModVct                                                     */
/*----------------------------------------------------------*/
void ModVct(float* vctM,float* vctR,float* vctI,int lgth)
{
 int i,j;

 /*Calcul du module*/
 for(i=0;i<lgth;i++)
 vctM[i]=sqrt((vctR[i]*vctR[i])+(vctI[i]*vctI[i]));
}

/*----------------------------------------------------------*/
/*  CenterVct                                               */
/*----------------------------------------------------------*/
void CenterVct(float* vct,int lgth)
{
 int i;
 int ci;
 float* vcttmp;

 /*Initialisation*/
 ci=(int)(lgth/2);

 /*Allocation memoire*/
 vcttmp=fmatrix_allocate_1d(lgth);

 /*Recadrage*/
 for(i=0;i<ci;i++) vcttmp[ci+i]=vct[i];
 for(i=ci;i<lgth;i++) vcttmp[i-ci]=vct[i];

 /*Transfert*/
 for(i=0;i<lgth;i++) vct[i]=vcttmp[i];

 /*desallocation memoire*/
 free_fmatrix_1d(vcttmp);
}

//-------------------//
//--- DEGRADATION ---//
//-------------------//
//----------------------------------------------------------
//  Gaussian noise  
//----------------------------------------------------------
float gaussian_noise(float var)
{
 float noise,theta;

 //Noise generation 
 noise=sqrt(-2*var*log(1.0-((float)rand()/RAND_MAX)));
 theta=(float)rand()*1.9175345E-4-PI;
 noise=noise*cos(theta);
 return noise;
}

//-----------------------------//
// -LECTURE/SAUVEGARDE SIGNAL -//
//-----------------------------//
/*----------------------------------------------------------*/
/* Chargement du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
float* LoadSignalDat(char* name,int* length)
 {
  int i;
  float ech1,ech2;
  char buff[NBCHAR];
  float Tech;
  FILE *fic;

  //Allocation
  float** Vct2D=fmatrix_allocate_2d(2,10000000);

  //nom du fichier dat
  strcpy(buff,name);
  strcat(buff,".dat");
  printf("\n  >> Ouverture de %s",buff);

  //ouverture du fichier
  fic=fopen(buff,"r");
  if (fic==NULL)
    { printf("\n- Grave erreur a l'ouverture de %s  -\n",buff);
      exit(-1); }
 
  //Lecture Donnée & Longueur & Periode Ech
  for(i=0;;i++)
       { fscanf(fic,"%f %f",&ech1,&ech2);
         if (feof(fic)) break;
         //printf("\n[%f::%f]",ech1,ech2);
         Vct2D[0][i]=ech1;
         Vct2D[1][i]=ech2; }

  (*length)=i;
  Tech=Vct2D[0][1];
  Tech=1.0/Tech;
  Tech=(int)Tech;
  printf(" (%d echantillons)",(*length));
  printf("\n  >> Techantillonnage:: %.0f echantillons/seconde",Tech);

  //Chargement
  float* VctFinal=fmatrix_allocate_1d((*length));
  for(i=0;i<(*length);i++) VctFinal[i]=Vct2D[1][i];
 
   //End 
  fclose(fic);
  (*length)=i;
  free_fmatrix_2d(Vct2D);
  return VctFinal;
 }

/*----------------------------------------------------------*/
/* Sauvegarde du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
void SaveSignalDat(char* name,float* vct,int length)
 {
  int i;
  char buff[NBCHAR];
  FILE* fic;

  /*--extension--*/
  strcpy(buff,name);
  strcat(buff,".dat");

  /*--ouverture fichier--*/
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format dat\n",name);

  /*--enregistrement--*/
  for(i=0;i<length;i++) fprintf(fic,"%f %f\n",(float)i,vct[i]);

   /*--fermeture fichier--*/
   fclose(fic);  
 }

/*----------------------------------------------------------*/
/* Sauvegarde du signal de nom <name>.dat                   */
/*----------------------------------------------------------*/
void SaveSignalDat2(char* name,float* vct,int length,float Tech)
 {
  int i;
  char buff[NBCHAR];
  FILE* fic;

  /*--extension--*/
  strcpy(buff,name);
  strcat(buff,".dat");

  /*--ouverture fichier--*/
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format dat\n",name);

  /*--enregistrement--*/
  for(i=0;i<length;i++) fprintf(fic,"%f %f\n",(float)i*Tech,vct[i]);

   /*--fermeture fichier--*/
   fclose(fic);  
 }

//-------------------------------//
//-- LOAD/SAVE IMAGE COULEUR-----//
//-------------------------------//
//----------------------------------------------------------
// Chargement de l'image de nom <name> (en ppm)             
//----------------------------------------------------------
void load_image_ppm(char* path,float*** img,int lgth,int wdth)
 {
  int i,j;
  int temp;
  char* tempc;
  unsigned char varb,varv,varr;

  char stringTmp1[200],stringTmp2[200];
  int ta1,ta2,ta3;
  FILE *fic;

  //enregistrement du chemin;
  if (strstr(path,".ppm")==NULL) strcat(path,".ppm");

   //ouverture du fichier
  fic=fopen(path,"r");
  if (fic==NULL) 
   { printf("\n ->> Grave erreur a l'ouverture de %s -\n",path);
     exit(-1); }

  //recuperation de l'entete
  tempc=fgets(stringTmp1,100,fic); //P6
  tempc=fgets(stringTmp2,100,fic); //Comment

  for(;;)
    { if (((int)stringTmp2[0]==10)||((int)stringTmp2[0]==35))
      tempc=fgets(stringTmp2,100,fic); 

      else break; }

  temp=sscanf(tempc," %d %d",&ta1,&ta2);
  temp=fscanf(fic," %d",&ta3);

  //affichage de l'entete
  printf("\n\n -- Entete --");
  printf("\n ------------");
  printf("\n %s  %d %d \n %d",stringTmp1,ta1,ta2,ta3);
  printf("\n ----------------------------------------------------------\n\n");
   
  //chargement dans la matrice
  for(i=0;i<lgth;i++) for(j=0;j<(wdth*3);j+=3)  
    { temp=fread(&varr,1,1,fic);
      temp=fread(&varv,1,1,fic);
      temp=fread(&varb,1,1,fic);
     
      img[2][i][j/3]=varr;        
      img[0][i][j/3]=varv;        
      img[1][i][j/3]=varb; }      

  //fermeture du fichier
  fclose(fic);
 }

//----------------------------------------------------------
// Sauvegarde de l'image de nom <name> au format couleur ppm        
//----------------------------------------------------------
void SaveImagePpm(char* Name,float*** matrvb,int lgth,int wdth)
 {
  int i,j;
  char buff[200];
  FILE* fuser;
  time_t tm;

  //extension
  strcpy(buff,Name);
  strcat(buff,".ppm");

  //ouverture fichier
  fuser=fopen(buff,"w");
    if (fuser==NULL) 
        { printf(" probleme dans la sauvegarde de %s",buff); 
          exit(-1); }

  //affichage
  printf("\n sauvegarde dans -> %s au format %s [%d][%d]",buff,".ppm",lgth,wdth);
  fflush(stdout);

  //sauvegarde de l'entete
  fprintf(fuser,"P6");
  fprintf(fuser,"\n# IMG Module, %s",ctime(&tm));
  fprintf(fuser,"%d %d",wdth,lgth);
  fprintf(fuser,"\n255\n");

  //enregistrement
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
    { fprintf(fuser,"%c",(char)matrvb[0][i][j]);
      fprintf(fuser,"%c",(char)matrvb[1][i][j]);
      fprintf(fuser,"%c",(char)matrvb[2][i][j]); }
       
  //fermeture fichier
   fclose(fuser); 
 }


//-------------------------------//
//------- LOAD IMAGE N&B---------//
//-------------------------------//
//----------------------------------------------------------
// Sauvegarde de l'image de nom <name> au format pgm        
//----------------------------------------------------------
void SaveImagePgm(char* name,float** mat,int length,int width)
 {
  int i,j;
  char buff[NBCHAR];
  FILE* fic;
  time_t tm;

  //extension
  strcpy(buff,name);
  strcat(buff,".pgm");

  //ouverture fichier
  fic=fopen(buff,"w");
    if (fic==NULL) 
        { printf(" Probleme dans la sauvegarde de %s",buff); 
          exit(-1); }
  printf("\n Sauvegarde de %s au format pgm\n",name);

  //sauvegarde de l'entete
  fprintf(fic,"P5");
  if ((ctime(&tm))==NULL) fprintf(fic,"\n#\n");
  else fprintf(fic,"\n# IMG Module, %s",ctime(&tm));
  fprintf(fic,"%d %d",width,length);
  fprintf(fic,"\n255\n");

  //enregistrement
     for(i=0;i<length;i++)
      for(j=0;j<width;j++) 
        fprintf(fic,"%c",(char)mat[i][j]);
   
  //fermeture fichier
   fclose(fic); 
 } 

//---------------------------------------------//
//-- CONVERT IMAGE N&B en fausse couleur ------//
//              et SAUVEGARDE                  //
//---------------------------------------------//
void ConvertAndSaveImg(char* name,float** mat,int lgth,int wdth)
 {
  int i,j;
  float lum;

  //Chargement LUT
  float*** Clut=fmatrix_allocate_3d(3,256,12);
  load_image_ppm("clut.ppm",Clut,256,12);

  //Allocation
  float*** ImgCoul=fmatrix_allocate_3d(3,lgth,wdth);

  //Recal
  Recal(mat,lgth,wdth);
 
  //Conversion N&B > FausseCouleur
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
   {
     lum=(int)mat[i][j];

     ImgCoul[0][i][j]=Clut[0][(int)lum][6];
     ImgCoul[1][i][j]=Clut[1][(int)lum][6]; 
     ImgCoul[2][i][j]=Clut[2][(int)lum][6];
   }

  //SauveImage
  SaveImagePpm(name,ImgCoul,lgth,wdth);
  
  //Liberation
  free_fmatrix_3d(ImgCoul,3);
  free_fmatrix_3d(Clut,3);
 }

//----------------------------------------------------------
// Recal                                                    
//----------------------------------------------------------
void Recal(float** mat,int lgth,int wdth)
{
 int i,j;
 float max,min;

 //Initialisation
 max=-1000000.0;
 min=100000000;

 //Recherche du min
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
    if (mat[i][j]<min) min=mat[i][j];

 //plus min
   for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
    mat[i][j]-=min;

 //Recherche du max
  for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) 
    if (mat[i][j]>max) max=mat[i][j];

 //Recalibre la matrice
 for(i=0;i<lgth;i++) for(j=0;j<wdth;j++)
   if (max) mat[i][j]*=(GREY_LEVEL/max); 
}     


