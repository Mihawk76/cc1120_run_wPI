#include <stdio.h>

int read_ints ()
{
  FILE* file = fopen ("../id.txt", "r");
  int i = 0;

  fscanf (file, "%d", &i);    
      printf ("%d\n", i);
      fscanf (file, "%d", &i);      
  fclose (file);        
	return i;
}

