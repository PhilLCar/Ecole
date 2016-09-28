float sqr(float x)
{
  return x * x;
}

void CopyImg(float** img, float** newImg, int width, int height)
{
  int i, j;

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++)
      newImg[j][i] = img[j][i];
}

void CropImg(float** canvas, int cw, int ch, float** img, int width, int height, int x, int y) {
  int i, j;
  for (i = 0; i < cw; i++)
    for (j = 0; j < ch; j++)
      canvas[j][i] = img[j + y][i + x];
}


float MSE(float **imgD, float **imgO, int width, int height)
{
  float sum = 0;
  int i, j;
  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      sum += sqr(imgD[j][i] - imgO[j][i]);
    }
  return sum / (float)height / (float)width;
}

void DecImg(float **imgO, float **imgD, int width, int height, int lateral, int vertical)
{
  int i, j;

  CopyImg(imgO, imgD, width, height);

  for (i = 0; i < width; i++)
    for (j = 0; j < height; j++) {
      imgD[j][i] = imgO[(j + vertical) % height][(i + lateral) % width];
    }
}

void RemoveNoise1(float **img, int width, int height, int T)
{
  int i, j, a, b, c, n;
  float **subimg = fmatrix_allocate_2d(8, 8);
  for (i = 0; i < width / 8; i++)
    for (j = 0; j < height / 8; j++) {
      CropImg(subimg, 8, 8, img, width, height, i * 8, j * 8);
      ddct8x8s(-1, subimg);
      n = 0;
      for (c = 0; c < 8; c++)
	if (c % 2)
	  for (a = c, b = 0; a >= 0; a--, b++) {
	    if (n++ > T) subimg[b][a] = 0;
	  }
	else
	  for (a = 0, b = c; b>= 0; a++, b--) {
	    if (n++ > T) subimg[b][a] = 0;
	  }
      for (c = 1; c < 8; c++)
	if (c % 2)
	  for (a = c, b = 7; a < 8; a++, b--) {
	    if (n++ > T) subimg[b][a] = 0;
	  }
	else
	  for (a = 7, b = c; b < 8; a--, b++) {
	    if (n++ > T) subimg[b][a] = 0;
	  }
      ddct8x8s(1, subimg);
      for (a = i * 8; a < i * 8 + 8; a++)
	for (b = j * 8; b < j * 8 + 8; b++) {
	  img[b][a] = subimg[b % 8][a % 8];
	  if (img[b][a] < 0) img[b][a] = 0;
	  if (img[b][a] > 255) img [b][a] = 255;
	}
    }
  free_fmatrix_2d(subimg);
}

void RemoveNoise2(float **img, int width, int height, int T)
{
  int i, j, m, n;
  float **subimg = fmatrix_allocate_2d(8, 8);
  for (i = 0; i < width / 8; i++)
    for (j = 0; j < height / 8; j++) {
      CropImg(subimg, 8, 8, img, width, height, i * 8, j * 8);
      ddct8x8s(-1, subimg);
      for (m = 0; m < 8; m++)
	for (n = 0; n < 8; n++)
	  if (abs(subimg[n][m]) < T)
	    subimg[n][m] = 0;
      ddct8x8s(1, subimg);
      for (m = i * 8; m < i * 8 + 8; m++)
	for (n = j * 8; n < j * 8 + 8; n++) {
	  img[n][m] = subimg[n % 8][m % 8];
	  if (img[n][m] < 0) img[n][m] = 0;
	  if (img[n][m] > 255) img[n][m] = 255;
	}
    }
  free_fmatrix_2d(subimg);
}

void RemoveNoise3(float **img, int width, int height, int c, int T)
{
  int i, j, m, n, s, t;
  int ct = 0;
  float **subimg = fmatrix_allocate_2d(8, 8);
  float **dec    = fmatrix_allocate_2d(height, width);
  float **mean   = fmatrix_allocate_2d(height, width);
  float **tmp    = fmatrix_allocate_2d(height, width);
  
  for (i = 0; i < width; i += width / 8)
    for (j = 0; j < height; j += height / 8) {
      DecImg(img, dec, width, height, i, j);

      for (s = 0; s < width - c; s += 8 - c)
	for (t = 0; t < height - c; t += 8 - c) {
	  CropImg(subimg, 8, 8, dec, width, height, s, t);
	  ddct8x8s(-1, subimg);
	  for (m = 0; m < 8; m++)
	    for (n = 0; n < 8; n++)
	      if (abs(subimg[n][m]) <= T)
		subimg[n][m] = 0;
	  ddct8x8s(1, subimg);
	  for (m = s; m < s + 8; m++)
	    for (n = t; n < t + 8; n++)
	      dec[n][m] = subimg[n - t][m - s];
	}
      DecImg(dec, tmp, width, height, width - i, height - j);

      ct++;
      
      for (s = 0; s < width; s++)
	for (t = 0; t < height; t++) {
	  mean[t][s] = ((ct - 1) * mean[t][s] + tmp[t][s]) / ct;
	}
    }

  CopyImg(mean, img, width, height);
  
  free_fmatrix_2d(subimg);
  free_fmatrix_2d(dec);
  free_fmatrix_2d(mean);
  free_fmatrix_2d(tmp);
}

void RemoveNoise3b(float **img, int width, int height, int c, int T)
{
  int i, j, m, n, s, t;
  int ct = 0;
  float **subimg = fmatrix_allocate_2d(8, 8);
  float **dec    = fmatrix_allocate_2d(height, width);
  float **tmp    = fmatrix_allocate_2d(height, width);
  for (i = 0; i < width; i += width / 8)
    for (j = 0; j < height; j += height / 8) {
      DecImg(img, dec, width, height, i, j);

      for (s = 0; s < width - c; s += 8 - c)
	for (t = 0; t < height - c; t += 8 - c) {
	  CropImg(subimg, 8, 8, dec, width, height, s, t);
	  ddct8x8s(-1, subimg);
	  for (m = 0; m < 8; m++)
	    for (n = 0; n < 8; n++)
	      if (abs(subimg[n][m]) < T)
		subimg[n][m] = 0;
	  ddct8x8s(1, subimg);
	  for (m = s; m < s + 8; m++)
	    for (n = t; n < t + 8; n++) {
	      dec[n][m] = subimg[n - t][m - s];
	      if (dec[n][m] < 0) dec[n][m] = 0;
	      if (dec[n][m] > 255) dec[n][m] = 255;
	    }
	}
      DecImg(dec, tmp, width, height, width - i, height - j);

      ct++;
      
      for (s = 0; s < width; s++)
	for (t = 0; t < height; t++) {
	  img[t][s] = ((ct - 1) * img[t][s] + tmp[t][s]) / ct;
	}
    }
  
  free_fmatrix_2d(subimg);
  free_fmatrix_2d(dec);
  free_fmatrix_2d(tmp);
}

void RemoveNoise4(float **img, int width, int height, int c, int T)
{
  int i, j, k;
  int ct = 0;
  float **tmp    = fmatrix_allocate_2d(height, width);
  float **mean   = fmatrix_allocate_2d(height, width);

  for (i = T - 25 / c; i < T + 25 / c; i++) {
    CopyImg(img, tmp, width, height);
    RemoveNoise3(img, width, height, c, i);

    ct++;

    for (i = 0; i < width; i++)
	for (j = 0; j < height; j++) {
	  mean[j][i] = ((ct - 1) * mean[j][i] + tmp[j][i]) / ct;
	}
  }
  
  free_fmatrix_2d(mean);
  free_fmatrix_2d(tmp);
}
