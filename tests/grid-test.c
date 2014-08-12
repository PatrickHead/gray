#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "grid.h"

#define S 26

void raw(void);
void cooked(void);

int main(int argc, char **argv)
{
  grid_s *sh;
  grid_size_s *sh_size;
  char *cell;
  char *s;
  int y, x;
  vertex_s *loc;
  char cmd = 0;
  int quit = 0;

  sh_size = grid_size_create();
  grid_size_set(sh_size, S, S);

  sh = grid_create();
  grid_set_size(sh, sh_size);

  for (y = 0; y < grid_size_get_height(sh_size); y++)
  {
    for (x = 0; x < grid_size_get_width(sh_size); x++)
    {
      grid_goto(sh, y, x);
      s = malloc(44);
      loc = grid_get_location(sh);
      sprintf(s, "(%d,%d)", (int)loc->y, (int)loc->x);
      grid_set_cell(sh, s);
    }
  }

  grid_origin(sh);

  printf("\aType 'q' to quit\n");

  raw();

  while (!quit)
  {
    cell = grid_current(sh);
    printf("%s\n", cell);
    cmd = getchar();
    switch (cmd)
    {
      case 'u': grid_up(sh); break;
      case 'd': grid_down(sh); break;
      case 'l': grid_left(sh); break;
      case 'r': grid_right(sh); break;
      case 'o': grid_origin(sh); break;
      case 'e': grid_end(sh); break;
      case 0x1b:
      case 'q': quit = 1; break;
      default: break;
    }
    //while ((cmd = getchar()) != '\n') ;
  }

  cooked();

  grid_destroy(sh);

	return 0;
}
/*
Under any operating system (Unix or otherwise) offering POSIX compatibility, use the tcgetattr and tcsetattr calls on file descriptor 0 to manipulate the termios structure, defined in <termios.h>. In the c_lflag field, clear the ICANON (and perhaps ECHO) bits. Also, set c_cc[VMIN] to 1 and c_cc[VTIME] to 0. 
*/

struct termios tios_orig;

void raw(void)
{
  struct termios tios_new;

    // Save current settings
  tcgetattr(0, &tios_orig);

    // Get a copy for new settings
  tcgetattr(0, &tios_new);

  //tios_new.c_lflag &= ~ICANON;

  //tios_new.c_cc[VMIN] = 1;
  //tios_new.c_cc[VTIME] = 0;
  cfmakeraw(&tios_new);
  tios_new.c_oflag |= OPOST;

  tcsetattr(0, TCSANOW, &tios_new);
}

void cooked(void)
{
  tcsetattr(0, TCSANOW, &tios_orig);
}

