#include <stdio.h>
#include <stdlib.h>

#include "reference.h"

int reference_new(void)
{
  FILE *f;
  char *home;
  char ref_name[2048];
  int ref;

  home = getenv("HOME");
  snprintf(ref_name, 2048, "%s/.blue", home);

  f = fopen(ref_name, "r");
  if (!f)
  {
    f = fopen(ref_name, "w");
    if (!f)
      return 0;
    fprintf(f, "1\n");
    fclose(f);
    return 1;
  }
    
  fscanf(f, "%d", &ref);
  fclose(f);

  ++ref;

  f = fopen(ref_name, "w");
  if (f)
  {
    fprintf(f, "%d\n", ref);
    fclose(f);
  }
  
  return ref;
}

