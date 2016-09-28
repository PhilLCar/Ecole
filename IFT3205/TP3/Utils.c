float sqr(float x)
{
  return x * x;
}

float hyp(float x, float y)
{
  return sqrt(sqr(x) + sqr(y));
}

float deg(float x)
{
  return x / 180 * PI;
}

//////////////////// IMAGE LOADING AND FREEING //////////////////////
void LoadImg3(char* img_name, float*** imgR, float*** imgI, float*** imgM, int* length, int* width)
{
  int i, j;
  *imgR = LoadImagePgm(img_name, length, width);
  *imgI = fmatrix_allocate_2d(*length, *width);
  *imgM = fmatrix_allocate_2d(*length, *width);
  for(i = 0; i < *length; i++) 
    for(j = 0; j < *width; j++) 
      {
        (*imgI)[i][j] = 0.0;
	(*imgM)[i][j] = 0.0;
      }
}

void FreeImg3(float** imgR, float** imgI, float** imgM)
{
  free_fmatrix_2d(imgR);
  free_fmatrix_2d(imgI); 
  free_fmatrix_2d(imgM);
}

void LoadImg(char* img_name, float*** imgR, float*** imgI, int* width, int* length)
{
  int i, j;
  *imgR = LoadImagePgm(img_name, length, width);
  *imgI = fmatrix_allocate_2d(*length, *width);
  for(i = 0; i < *length; i++) 
    for(j = 0; j < *width; j++) 
      {
        (*imgI)[i][j] = 0.0;
      }
}

void FreeImg(float** imgR, float** imgI)
{
  free_fmatrix_2d(imgR);
  free_fmatrix_2d(imgI); 
}

///////////////////////// IMAGE MODS /////////////////////////////
/* Fonction log pour rehausser l'image */
void Log(float** img, int width, int height)
{
  // Calcule la différence avec la nouvelle valeur maximale
  // pour éviter d'appeler Recal une seconde fois
  float factor = 255.0 / log(256);
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      img[j][i] = factor * log(1 + img[j][i]);
}

void Normalize(float** img, int width, int height, float f)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      img[j][i] = (img[j][i] - 127.0) * f + 127.0;
      /*if (img[j][i] > 255) img[j][i] = 255;
	else if (img[j][i] < 0) img[j][i] = 0;*/
    } 
}

void Pnormalize(float** img, int width, int height, float f)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      img[j][i] = pow(img[j][i] - 127.0, f);
      if (img[j][i] != img[j][i]) img[j][i] = 0;
    }
  Recal(img, height, width);
}

/*------------FONTCTION DE RECENTRAGE-------------*/
void RCenterImg(float*** img, int width, int height)
{
  float** tmp = fmatrix_allocate_2d(height, width);
  int hcenter = width / 2;
  int vcenter = height / 2;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (i < hcenter)
	if (j < vcenter)
	  tmp[j][i] = (*img)[j + vcenter][i + hcenter];
	else
	  tmp[j][i] = (*img)[j - vcenter][i + hcenter];
      else
  	if (j < vcenter)
	  tmp[j][i] = (*img)[j + vcenter][i - hcenter];
	else
	  tmp[j][i] = (*img)[j - vcenter][i - hcenter];
  free_fmatrix_2d(*img);
  *img = tmp;
}
