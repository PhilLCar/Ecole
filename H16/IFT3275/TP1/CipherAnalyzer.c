#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


#define CHUNK_SIZE 256

#define FILTER_SPECIAL_CHARS   1
#define FILTER_WHITE_SPACES    2
#define FILTER_NUMBERS         4
#define FILTER_MINUSCULES      8
#define FILTER_SYMBOLS        16
#define FILTER_ALL            31


// GLOBAL VARS
char* cipher;

char* read()
{
  int buffer_size = CHUNK_SIZE;
  int buffer_length = 0;
  char* buffer = malloc(buffer_size);
  char c;
  
  if (buffer == NULL) {
    fprintf(stderr, "ERROR: Memory allocation failed!\n");
    return "";
  }

  while ((c = getchar()) != '\n') {
    // Memory extension
    if (buffer_length == buffer_size) {
      buffer_size += CHUNK_SIZE;
      char* temp = realloc(buffer, buffer_size);
      if (temp == NULL) {
	fprintf(stderr, "ERROR: Memory reallocation failed!\n");
	return buffer;
      } else buffer = temp;
    }
    *(buffer + buffer_length++) = c;
  }
  
  *(buffer + buffer_length) = 0;

  return buffer;
}

int saveFile(char *fileName, char* content)
{
  FILE* file = fopen(fileName, "w");
  
  if (file == NULL) {
    fprintf(stderr, "ERROR: Could not open file!\n");
    return 1;
  }

  fprintf(file, content);

  fclose(file);
  
  return 0;
}

char *readFile(char *fileName)
{
  FILE* file = fopen(fileName, "r");
  int buffer_size = CHUNK_SIZE;
  int buffer_length = 0;
  char* buffer;
  char c;
  
  if (file == NULL) {
    fprintf(stderr, "ERROR: Could not open file!\n");
    return NULL;
  }

  buffer = malloc(buffer_size);
  
  while ((c = fgetc(file)) != EOF) {
    // Memory extension
    if (buffer_length == buffer_size) {
      buffer_size += CHUNK_SIZE;
      char* temp = realloc(buffer, buffer_size);
      if (temp == NULL) {
	fprintf(stderr, "ERROR: Memory reallocation failed!\n");
	return buffer;
      } else buffer = temp;
    }
    *(buffer + buffer_length++) = c;
  }
  
  *(buffer + buffer_length) = 0;   

  fclose(file);   
  
  return buffer;
}

char getdec(char plain, char cipher) {
  return (plain + cipher - 0x82) % 26 + 0x41;
}

char getinvdec(char plain, char cipher) {
  return (cipher - plain + 52) % 26 + 0x41;
}

char getmax(int data[128]) {
  int i, max = 0;
  for (i = 1; i < 128; i++)
    if (data[i] > data[max]) max = i;
  return (char)max;
}

char* filterout(char* cipher, int flags) {
  int i, length, count = 0;
  char* newcipher, c;
  length = strlen(cipher);
  newcipher = malloc(length);
  if (!newcipher) {
    fprintf(stderr, "ERROR: Memory allocation failed");
    return "";
  }
  memset(newcipher, 0, length);
  
  for (i = 0; i < length; i++) {
    c = *(cipher + i);
    if (flags & FILTER_SPECIAL_CHARS)
      if ((c < 0x40 && c != '\t' && c != '\n' & c != '\r') || c == 0x7F)
	continue;
    if (flags & FILTER_SYMBOLS)
      if ((c > 0x20 && c < 0x30) || (c > 0x39 && c < 0x41) || (c > 0x5A && c < 0x61) || c > 0x7A)
	continue;
    if (flags & FILTER_WHITE_SPACES)
      if (c == '\n' || c == '\r' || c == ' ' || c == '\t')
	continue;
    if (flags & FILTER_NUMBERS)
      if (c > 0x30 && c < 0x3A)
	continue;
    if (flags & FILTER_MINUSCULES)
      c = toupper(c);
    *(newcipher + count++) = c;
  }
  char* temp = realloc(newcipher, count);
  if (temp) return temp;
  fprintf(stderr, "ERROR: Memory reallocation failed!");
  return newcipher;
}

char* vigenere_decrypt(char* cipher, char* key, int flags)
{
  int i, k = 0, lk = strlen(key);
  int length, count = 0;
  char* newcipher, c;
  length = strlen(cipher);
  newcipher = malloc(length);
  if (!newcipher) {
    fprintf(stderr, "ERROR: Memory allocation failed!\n");
    return "";
  }
  memset(newcipher, 0, length);
  
  for (i = 0; i < length; i++) {
    c = *(cipher + i);
    if (flags & FILTER_SPECIAL_CHARS)
      if ((c < 0x20 && c != '\t' && c != '\n' & c != '\r') || c == 0x7F) {
	*(newcipher + count++) = c;
	continue;
      }
    if (flags & FILTER_SYMBOLS)
      if ((c > 0x20 && c < 0x30) || (c > 0x39 && c < 0x41) || (c > 0x5A && c < 0x61) || c > 0x7A) {
	*(newcipher + count++) = c;
	continue;
      }
    if (flags & FILTER_WHITE_SPACES)
      if (c == '\n' || c == '\r' || c == ' ' || c == '\t') {
	*(newcipher + count++) = c;
	continue;
      }
    if (flags & FILTER_NUMBERS)
      if (c > 0x2F && c < 0x3A) {
	*(newcipher + count++) = c;
	continue;
      }
    if (flags & FILTER_MINUSCULES) 
      if (c != toupper(c)) {
	c = toupper(c);
	*(newcipher + count++) = tolower(getinvdec(*(key + (k++ % lk)), c));
	continue;
      }
    *(newcipher + count++) = getinvdec(*(key + (k++ % lk)), c);
  }
  return newcipher;
}

int vig_getkeylength(char* cipher) {
  int i, j, k = 0;
  int tot = 0, count = 0, length = strlen(cipher);
  float min, max, maxdiff = 0;
  float num[128];
  char c;
  printf("Length of text: %d\n", length);
  for (i = 1; i < length /  2 && i < 50; i++) {
    memset(num, 0, sizeof(num));
    count = 0;
    while (count < length) {
      c = *(cipher + count);
      count += i;
      num[c]++;
      tot++;
    }
    min = length;
    max = 0;
    for (j = 0; j < 128; j++) {
      if (num[j]) {
	if (num[j] / tot < min) min = num[j] / tot;
	else if (num[j] / tot > max) max = num[j] / tot;
      }
    }
    if (max - min > maxdiff) {
      maxdiff = max - min;
      k = i;
    }
    tot = 0;
  }
  printf("KEY LENGTH : %d\n", k);
  return k;
}

char* vig_getkey(char* cipher, int flags)
{
  char* filteredcipher = filterout(cipher, FILTER_ALL);
  if (!filteredcipher) {
    fprintf(stderr, "ERROR: Could not get cipher filtered\n");
    return "";
  }
  int i, j, k = vig_getkeylength(filteredcipher);
  if (!k) {
    fprintf(stderr, "ERROR: Can't have a key with a length of 0\n");
    return "";
  }
  int length = strlen(filteredcipher);
  int num[128];
  char* key = malloc(k);
  if (!key) {
    fprintf(stderr, "ERROR: Memory allocation failed!\n");
    return "";
  }
  char c;
  for (i = 0; i < k; i++) {
    memset(num, 0, sizeof(num));
    for (j = i; j < length; j += k) {
      c = *(filteredcipher + j);
      num[c]++;
    }
    *(key + i) = getinvdec('E', getmax(num));
  }
  printf("KEY :\n%s\n", key);
  free(filteredcipher);
  return key;
}

int main() {
  char* cipher = readFile("german.txt");
  char* key = vig_getkey(cipher, FILTER_ALL);
  char* plain = vigenere_decrypt(cipher, key, FILTER_ALL);
  printf("Voici le texte décrypté:\n\n%s", plain);
  saveFile("germanplain.txt", plain);
  free(cipher);
  free(key);
  free(plain);
  return 0;
}
