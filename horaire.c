#include <stdio.h>
#include <string.h>
#include <time.h>

#include "list.c"

struct CLASS
{
  
};

void loadclass(char* class)
{
  
}

int main()
{
  time_t sec = time(NULL);

  printf(ctime(&sec));
  
  return 0;
}
