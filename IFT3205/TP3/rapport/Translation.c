float hyp(float x, float y)
{
  return sqrt(sqr(x) + sqr(y));
}

float deg(float x)
{
  return x / 180 * PI;
}

float interpol(float** img, int width, int height, float x, float y)
{
  // Rounded vals
  int xf, xc, yf, yc;
  // Propotion factors
  float tl, tr, bl, br;

  xf = floor(x);
  xc = ceil(x);
  yf = floor(y);
  yc = ceil(y);

  if (xf == xc) xc++;
  if (yf == yc) yc++;
  
  // CALCUL DES PROPORTIONS
  tl = (xc - x) * (yc - y);
  tr = (x - xf) * (yc - y);
  bl = (xc - x) * (y - yf);
  br = (x - xf) * (y - yf);

  if (xc < 0 || xf >= width || yc < 0 || yf >= height) return 0;

  if (xf < 0) {  
    if (yf < 0)
      return br * img[yc][xc];   
    else if (yc >= height)
      return tr * img[yf][xc];
    else
      return br * img[yc][xc] + tr * img[yf][xc];    
  }
  else if (xc >= width) {
    if (yf < 0)
      return bl * img[yc][xf];
    else if (yc >= height)
      return tl * img[yf][xf];
    else
      return bl * img[yc][xf] + tl * img[yf][xf];
  }
  else if (yf < 0)
    return bl * img[yc][xf] + br * img[yc][xc];
  else if (yc >= height)
    return tl * img[yf][xf] + tr * img[yf][xc];
  return br * img[yc][xc] + tr * img[yf][xc] + bl * img[yc][xf] + tl * img[yf][xf];
}

void Flip(float*** img, int width, int height)
{
  float** tmp = fmatrix_allocate_2d(height, width);
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      tmp[j][i] = (*img)[height - 1 - j][width - 1 - i];

  free_fmatrix_2d(*img);
  *img = tmp;
}

void CopyImg(float** img, float**newImg, int width, int height)
{
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      newImg[j][i] = img[j][i];
}

void PasteImg(float** canvas, float** img, int width, int height, float x, float y)
{
  int i, j;
  int vecx = floor(x), vecy = floor(y);
  float val;
  x = 1.0 - x + vecx;
  y = 1.0 - y + vecy;
  for (i = -1; i < width + 1; i++)
    for (j = -1; j < height + 1; j++) {
      val = interpol(img, width, height, i + x, j + y);
      if (val != 0) canvas[vecy + j + 1][vecx + i + 1] = val;
    }
}

void PasteImg2(float** canvas, float** img, int width, int height, int x, int y)
{
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      if (img[j][i] != 0)
	canvas[y + j][x + i] = img[j][i];
}

void InterCorrelate(float** imgR1, float** imgR2, float** imgSR, float** imgSI, int width, int height)
{
  float** tmpR1 = fmatrix_allocate_2d(height, width);
  float** tmpI1 = fmatrix_allocate_2d(height, width);
  float** tmpR2 = fmatrix_allocate_2d(height, width);
  float** tmpI2 = fmatrix_allocate_2d(height, width);
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      tmpI1[j][i] = 0;
      tmpI2[j][i] = 0;
    }
  
  CopyImg(imgR1, tmpR1, width, height);
  CopyImg(imgR2, tmpR2, width, height);
  
  Flip(&tmpR2, width, height);
  
  FFTDD(tmpR1, tmpI1, height, width);
  FFTDD(tmpR2, tmpI2, height, width);

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      imgSR[j][i] = tmpR1[j][i] * tmpR2[j][i] - tmpI1[j][i] * tmpI2[j][i];
      imgSI[j][i] = tmpR1[j][i] * tmpI2[j][i] + tmpR2[j][i] * tmpI1[j][i];
    }

  free_fmatrix_2d(tmpR1);
  free_fmatrix_2d(tmpR2);
  free_fmatrix_2d(tmpI1);
  free_fmatrix_2d(tmpI2);
}

void SquareMod(float** imgR, float** imgI, float** imgM, int width, int height)
{
  int i, j;
  
  FFTDD(imgR, imgI, height, width);
  Mod(imgM, imgR, imgI, height, width);

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      imgM[j][i] *= imgM[j][i];

  IFFTDD(imgR, imgI, height, width);
}

void PerformOp(float** imgA, float** imgB, float** imgR, int width, int height, float(*f)(float, float))
{
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      imgR[j][i] = f(imgA[j][i], imgB[j][i]);
}

float EvalDiff(float** img1, float** img2, int width, int height)
{
  float sum;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      sum += abs(img1[j][i] - img2[j][i]);

  return sum;
}
