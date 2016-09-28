float sqr(float x) {
  return x * x;
}

float iR (float r, float i) {
  return r / (sqr(r) + sqr(i));
}

float iI (float r, float i) {
  return i / (sqr(r) + sqr(i));
}

float findmax(float *vct, int length, int *start)
{
  int i;
  float max = 0;
  for (i = *start; i < length / 2; i++)
    if (vct[i] > max) {
      max = vct[i];
      *start = i;
    }
  return max;
}

float findmaxnonharmonic(float *vct, int zero, int harmony, int *start, int tolerance)
{
  int i;
  float max = 0;
  for (i = *start; i < zero; i++) {
    if ((zero - i) % harmony <= tolerance ||
	(zero - i) % harmony >= harmony - tolerance) continue;
    if (vct[i] > max) {
      max = vct[i];
      *start = i;
    }
  }
  return max;
}

int estimateRPM(float fe, float Tech, int cylinders)
{
  return (60 * fe / Tech) / cylinders;
}

void sample(float *signal, float *sample, int start, int length)
{
  int i;
  for (i = 0; i < length; i++)
    sample[i] = signal[i + start];
}

void meanFFT(float *signal, float *mean, int length, int block_size)
{
  int i, j;
  float *tmpR = fmatrix_allocate_1d(block_size);
  float *tmpI = fmatrix_allocate_1d(block_size);
  float *tmpM = fmatrix_allocate_1d(block_size);
  for (i = 0; i < length / (block_size / 2) - 1; i ++) {
    memset(tmpI, 0, block_size * sizeof(float));
    sample(signal, tmpR, i * block_size / 2, block_size);
    FFT1D(tmpR, tmpI, block_size);
    ModVct(tmpM, tmpR, tmpI, block_size);
    for (j = 0; j < block_size; j++)
      mean[j] = (i * mean[j] + tmpM[j]) / (i + 1);
  }
  CenterVct(mean, block_size);
  free(tmpR);
  free(tmpI);
  free(tmpM);
}

void Hamming(float *signal, float *mean, int length, int block_size)
{
  int i, j, T = 0;
  float *tmpR = fmatrix_allocate_1d(block_size);
  float *tmpI = fmatrix_allocate_1d(block_size);
  float *tmpM = fmatrix_allocate_1d(block_size);

  //meanFFT(signal, tmpR, length, block_size);
  //findmax(tmpR, block_size, &T);
  //T = block_size / (block_size / 2 - T);
  T = block_size;
  
  for (i = 0; i < length / (block_size / 2) - 1; i ++) {
    memset(tmpR, 0, block_size * sizeof(float));
    memset(tmpI, 0, block_size * sizeof(float));
    sample(signal, tmpR, i * block_size / 2, T);
    for (j = 0; j < T; j++) tmpR[j] *= 0.54 - 0.46 * cos(2.0 * PI * j / (float)T);
    FFT1D(tmpR, tmpI, block_size);
    ModVct(tmpM, tmpR, tmpI, block_size);
    for (j = 0; j < block_size; j++)
      mean[j] = (i * mean[j] + tmpM[j]) / (i + 1);
  }
  CenterVct(mean, block_size);
  free(tmpR);
  free(tmpI);
  free(tmpM);
}

void Hann(float *signal, float *mean, int length, int block_size)
{
  int i, j, T = 0;
  float *tmpR = fmatrix_allocate_1d(block_size);
  float *tmpI = fmatrix_allocate_1d(block_size);
  float *tmpM = fmatrix_allocate_1d(block_size);

  //meanFFT(signal, tmpR, length, block_size);
  //findmax(tmpR, block_size, &T);
  //T = block_size / (block_size / 2 - T);
  T = block_size;
  
  for (i = 0; i < length / (block_size / 2) - 1; i ++) {
    memset(tmpR, 0, block_size * sizeof(float));
    memset(tmpI, 0, block_size * sizeof(float));
    sample(signal, tmpR, i * block_size / 2, T);
    for (j = 0; j < T; j++) tmpR[j] *= 0.5 * (1 - cos(2.0 * PI * j / (float)T));
    FFT1D(tmpR, tmpI, block_size);
    ModVct(tmpM, tmpR, tmpI, block_size);
    for (j = 0; j < block_size; j++)
      mean[j] = (i * mean[j] + tmpM[j]) / (i + 1);
  }
  CenterVct(mean, block_size);
  free(tmpR);
  free(tmpI);
  free(tmpM);
}

void filter1(float *signal, float* filt, int length, float rho, float theta)
{
  int i;
  memset(filt, 0, length * sizeof(float));
  for (i = 0; i < length; i++) {
    float C1 = signal[i] - signal[(i - 1) % length];
    float C2 = sqr(rho) * filt[(i - 2) % length];
    float C3 = 2 * rho * cos(theta) * filt[(i - 1) % length];
    filt[i] = C1 - C2 + C3;
  }
}

void filter2(float *signal, float *filt, int length, float rho, float fe, float f)
{
  int i;
  float Xn, Xn1, Xn2, Yn1, Yn2;

  float ct = cos(fe / f * PI * 2);
  
  memset(filt, 0, length * sizeof(float));

  for (i = 0; i < length; i++){
    Xn = signal[i];
    Xn1 = signal[(i - 1) % length];
    Xn2 = signal[(i - 2) % length];
    Yn1 = filt[(i - 1) % length];
    Yn2 = filt[(i - 2) % length];
    filt[i] = Xn - 2.0 * ct * Xn1 + Xn2 + 2 * ct * rho * Yn1 - sqr(rho) * Yn2;
  }
}

void filter3(float *signal, float *sim, int length)
{
  memset(sim, 0, length * sizeof(float));
  int i;
  for (i = 2; i < length; i++)
    sim[i] = signal[i] - 7.0 / 3.0 * signal[i - 1] + 2.0 / 3.0 * signal[i - 2];
}

void filter4(float *signal, float *sim, int length)
{
  //memset(sim, 0, length * sizeof(float));
  int i;
  
  for (i = 3; i < length; i++) {
    sim[i] = 0.1*(signal[i] - 2.0 * signal[i - 1] -
	      (2.0/3.0 * sim[i - 3] - 3.0 * sim[i - 2] + 10.0/3.0 * sim[i - 1]));
  }
}

float RSB(float *signal, float *noise,  int length) {
  int i;
  float s = 0;
  for (i = 0; i < length; i++)
    s += abs(signal[i] - noise[i]);
  return log(s) / log(10);
}

float RSB2(float *signal, int length)
{
  int i;
  float m = 0;
  float s = 0;
  for (i = 0; i < length; i++) m += signal[i];
  m /= length;
  for (i = 0; i < length; i++) s += sqr(signal[i] - m);
  s /= length - 1;
  return m / s;
}

float* LoadSignalDat2(char *name, int *length, float *Tech)
{
  int i;
  float ech1,ech2;
  char buff[NBCHAR];
  float f;
  FILE *fic;

  //Allocation
  float** Vct2D = fmatrix_allocate_2d(2, 10000000);

  //nom du fichier dat
  strcpy(buff, name);
  strcat(buff, ".dat");
  printf("\n  >> Ouverture de %s", buff);

  //ouverture du fichier
  fic = fopen(buff, "r");
  if (fic == NULL) {
    printf("\n- Grave erreur a l'ouverture de %s  -\n",buff);
    exit(-1);
  }

  // Remove comments
  while (fgetc(fic) == ';') {
    while(fgetc(fic) != '\n');
  }
 
  //Lecture Donnée & Longueur & Periode Ech
  for (i = 0;; i++) {
    fscanf(fic,"%f %f", &ech1, &ech2);
    if (feof(fic)) break;
    Vct2D[0][i] = ech1;
    Vct2D[1][i] = ech2;
  }

  (*length) = i;
  *Tech = Vct2D[0][1];
  f = 1.0 / *Tech;
  f = (int)f;
  printf(" (%d echantillons)", (*length));
  printf("\n  >> Techantillonnage:: %.0f echantillons/seconde", f);

  //Chargement
  float* VctFinal = fmatrix_allocate_1d((*length));
  for(i = 0; i < (*length); i++) VctFinal[i] = Vct2D[1][i];
 
  //End 
  fclose(fic);
  (*length) = i;
  free_fmatrix_2d(Vct2D);
  return VctFinal;
}
