#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

FILE *open(char *filename) 
{
  FILE *file = fopen(filename, "r");

  if (file == NULL) 
  {
    int error = errno;

    if (error == ENOENT) 
    {
      fprintf(stderr, "File '%s' does not exist\n", filename);
    }

    else if (error == EACCES) 
    {
      fprintf(stderr, "Permission denied for file '%s'\n", filename);
    } 

    else 
    {
      fprintf(stderr, "Something went wrong when opening file '%s'\n", filename);
    }

    exit(1);
  }
  
  return file;
};