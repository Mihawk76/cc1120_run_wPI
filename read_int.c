#include <stdio.h>

int read_ints ()
{
  FILE* file = fopen ("/home/id.txt", "r");
  int i = 0;
  if (file == NULL)
{
    printf("Error opening file!\n");
    exit(1);
}
  fscanf (file, "%d", &i);    
      printf ("%d\n", i);
      fscanf (file, "%d", &i);      
  fclose (file);        
	return i;
}

