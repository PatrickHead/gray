#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <assert.h>

#include "grid-api.h"

typedef struct data
{
  char value[41];
} data;

void raw(void);
void cooked(void);
static void help(void);
static int add_digit(int n, char digit);
static void status(grid_api_status_s *stat);

void free_data(data* const d);
void* edit(void* const d);
void* show(void* const d);
void* delete(void* const d);
void* copy(void* const d);
void* paste(void* const d);

int main(int argc, char **argv)
{
  grid_api_s* fr;
  char cmd = 0;
  int quit = 0;
  grid_api_status_s *stat;
  grid_api_data_u dat;
  int n1, n2;
  int which_num = 1;
  int done = 0;
  int y, x;
  data* cd;
  vertex_s *v;

  fr = grid_api_create();

  v = vertex_create();

    // Setup utitily callback functions

  grid_api_set_free(fr, (grid_api_free_func)free_data);
  grid_api_set_edit(fr, edit);
  grid_api_set_show(fr, show);
  grid_api_set_delete(fr, delete);
  grid_api_set_copy(fr, copy);
  grid_api_set_paste(fr, paste);

    // Create a default test sheet (5x5) with data of "Yn.Xn"

  dat.location = v;
  vertex_set(dat.location, "none", 5, 5, 0);

  stat = grid_api_do(fr, grid_api_command_set_size, dat);

  for (y = 0; y < 5; y++)
  {
    for (x = 0; x < 5; x++)
    {
      dat.location = v;
      vertex_set(dat.location, "none", x + 1, y + 1, 0);
      stat = grid_api_do(fr, grid_api_command_goto, dat);
      cd = (data*)malloc(sizeof(data));
      sprintf(cd->value,
              "Y%1.1d.X%1.1d",
              (int)vertex_get_y(dat.location),
              (int)vertex_get_x(dat.location));
      dat.data = cd;
      stat = grid_api_do(fr, grid_api_command_set_data, dat);
    }
  }

  dat.location = v;
  vertex_set(dat.location, "none", 1, 1, 0);
	stat = grid_api_do(fr, grid_api_command_goto, dat);

  n1 = n2 = 0;
  //memset(&stat, 0, sizeof(stat));
  memset(&dat, 0, sizeof(dat));

  //dat.location = v;
  //dat.location.y = n1;
  //dat.location.x = n1;
  //stat = grid_api_do(fr, grid_api_command_set_size, dat);

  printf("\aType '?' for help\n");

  status(stat);

  raw();

  while (!quit)
  {
    cmd = getchar();
    switch (cmd)
    {
      case '?':
        putchar(cmd);
        help();
        done = 1;
        break;

      case 'q':
        putchar(cmd);
        quit = 1;
        break;

      case 0x1b:
        done = 1;
        break;

      case 'k':
        putchar(cmd);
        dat.repeat = n1 ? n1 : 1;
        stat = grid_api_do(fr, grid_api_command_up, dat);
        done = 1;
        break;

      case 'j':
        putchar(cmd);
        dat.repeat = n1 ? n1 : 1;
        stat = grid_api_do(fr, grid_api_command_down, dat);
        done = 1;
        break;

      case 'h':
        putchar(cmd);
        dat.repeat = n1 ? n1 : 1;
        stat = grid_api_do(fr, grid_api_command_left, dat);
        done = 1;
        break;

      case 'l':
        putchar(cmd);
        dat.repeat = n1 ? n1 : 1;
        stat = grid_api_do(fr, grid_api_command_right, dat);
        done = 1;
        break;

      case 'g':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, n1 ? n1 : 1);
        vertex_set_x(dat.location, n2 ? n2 : 1);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 'G':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, stat->rows);
        vertex_set_x(dat.location, stat->columns);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 'K':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, 1);
        vertex_set_x(dat.location, vertex_get_x(stat->location) + 1);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 'J':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, stat->rows);
        vertex_set_x(dat.location, vertex_get_x(stat->location) + 1);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 'H':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, vertex_get_y(stat->location) + 1);
        vertex_set_x(dat.location, 1);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 'L':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, vertex_get_y(stat->location) + 1);
        vertex_set_x(dat.location, stat->columns);
        stat = grid_api_do(fr, grid_api_command_goto, dat);
        done = 1;
        break;

      case 's':
        putchar(cmd);
        dat.location = v;
        vertex_set_y(dat.location, n1);
        vertex_set_x(dat.location, n2);
        stat = grid_api_do(fr, grid_api_command_set_size, dat);
        done = 1;
        break;

      case 'a':
        putchar(cmd);
        switch (cmd = getchar())
        {
          case 'r':
            putchar(cmd);
            dat.repeat = n1 ? n1 : 1;
            stat = grid_api_do(fr, grid_api_command_new_row, dat);
            break;
          case 'c':
            putchar(cmd);
            dat.repeat = n1 ? n1 : 1;
            stat = grid_api_do(fr, grid_api_command_new_column, dat);
            break;
          default: break;
        }
        done = 1;
        break;

      case 'A':
        putchar(cmd);
        n1 *= -1;
        switch (cmd = getchar())
        {
          case 'r':
            putchar(cmd);
            dat.repeat = n1 ? n1 : -1;
            stat = grid_api_do(fr, grid_api_command_new_row, dat);
            break;
          case 'c':
            putchar(cmd);
            dat.repeat = n1 ? n1 : -1;
            stat = grid_api_do(fr, grid_api_command_new_column, dat);
            break;
          default: break;
        }
        done = 1;
        break;

      case 'd':
        putchar(cmd);
        switch (cmd = getchar())
        {
          case 'r':
            putchar(cmd);
            dat.repeat = n1 ? n1 : 1;
            stat = grid_api_do(fr, grid_api_command_del_row, dat);
            break;
          case 'c':
            putchar(cmd);
            dat.repeat = n1 ? n1 : 1;
            stat = grid_api_do(fr, grid_api_command_del_column, dat);
            break;
          default: break;
        }
        done = 1;
        break;

      case 'D':
        putchar(cmd);
        n1 *= -1;
        switch (cmd = getchar())
        {
          case 'r':
            putchar(cmd);
            dat.repeat = n1 ? n1 : -1;
            stat = grid_api_do(fr, grid_api_command_del_row, dat);
            break;
          case 'c':
            putchar(cmd);
            dat.repeat = n1 ? n1 : -1;
            stat = grid_api_do(fr, grid_api_command_del_column, dat);
            break;
          default: break;
        }
        done = 1;
        break;

      case 'e':
        putchar(cmd);
        stat = grid_api_do(fr, grid_api_command_edit, dat);
        done = 1;
        break;

      case 'c':
        putchar(cmd);
        stat = grid_api_do(fr, grid_api_command_copy, dat);
        done = 1;
        break;

      case 'p':
        putchar(cmd);
        stat = grid_api_do(fr, grid_api_command_paste, dat);
        done = 1;
        break;

      case '.':
        putchar(cmd);
        stat = grid_api_do(fr, grid_api_command_delete, dat);
        done = 1;
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        putchar(cmd);
        switch (which_num)
        {
          case 1: n1 = add_digit(n1, cmd); break;
          case 2: n2 = add_digit(n2, cmd); break;
          default: break;
        }
        break;

      case ',':
        putchar(cmd);
        which_num = 2;
        break;

      default: break;
    }

    if (done)
    {
      printf("\n");
      grid_api_do(fr, grid_api_command_show, dat);
      done = 0;
      n1 = n2 = 0;
      which_num = 1;
      memset(&dat, 0, sizeof(dat));
      status(stat);
    }
  }

  cooked();

  printf("\n");

  grid_api_destroy(fr);

	return 0;
}

static void help(void)
{
  printf("\n");
  printf("\n");
  printf("? - Help   q - Quit   <ESC> - abort command\n");
  printf("\n");
  printf("[N]k - Move up      K - Goto first row\n");
  printf("[N]j - Move down    J - Goto last row\n");
  printf("[N]h - Move left    H - Goto first column\n");
  printf("[N]l - Move right   L - Goto last column\n");
  printf("N,Ng - Goto cell\n");
  printf("\n");
  printf("[N]ar - Add N rows after     [N]Ar - Insert N rows before\n");
  printf("[N]ac - Add N columns after  [N]Ac - Insert N columns before\n");
  printf("N,Ns - Set sheet size\n");
  printf("\n");
  printf("[N]dr - Delete N rows after     [N]Dr - Delete N rows before\n");
  printf("[N]dc - Delete N columns after  [N]Dc - Delete N columns before\n");
  printf("\n");
  printf("e - Edit\n");
  printf("c - Copy\n");
  printf("p - Paste\n");
  printf(". - Clear\n");
}

static struct termios tios_orig;

void raw(void)
{
  struct termios tios_new;

    // Save current settings
  tcgetattr(0, &tios_orig);

    // Get a copy for new settings
  tcgetattr(0, &tios_new);

  cfmakeraw(&tios_new);
  tios_new.c_oflag |= OPOST;
  //tios_new.c_lflag |= ECHO;

  tcsetattr(0, TCSANOW, &tios_new);
}

void cooked(void)
{
  tcsetattr(0, TCSANOW, &tios_orig);
}

static int add_digit(int n, char digit)
{
  digit -= '0';

  if (n || digit)
    n = (n * 10) + digit;

  return n;
}

static void status(grid_api_status_s *stat)
{
  assert(stat);

  printf("\r%d [%d,%d] (%d,%d): ",
         stat->code,
         stat->rows,
         stat->columns,
         (int)vertex_get_y(stat->location),
         (int)vertex_get_x(stat->location));
}

void free_data(data* const d)
{
  if (!d) return;
  free(d);
}

void* edit(void* const d)
{
  int i;
  int c;
  int done;
  data* dat;

  dat = (data*)malloc(sizeof(data));
  memset(dat, 0, sizeof(data));

  printf("\n");
  if (d && strlen(((data*)d)->value))
    printf("%s\n", ((data*)d)->value);
  else
    printf("<empty>\n");

  for (i = done = 0; (i < 40) && !done; i++)
  {
    c = getchar();

    switch (c)
    {
      case '\b':
      case 0x7f:
        putchar(0x08);
        putchar(' ');
        putchar(0x08);
        i -= 2;
        if (i < 0) i = -1;
        continue;
      case '\n':
      case '\r':
        done = 1;
        break;
      case 0x1b:
        if (!d && dat) free(dat);
        return NULL;
      default:
        putchar(c);
        dat->value[i] = c;
    }
  }

  dat->value[i] = 0;

  return dat;
}

void* show(void* const d)
{
  data* dat;

  dat = (data*)d;

  if (dat)
    printf("%s\n", dat->value);

  return dat;
}

void* delete(void* const d)
{
  return d;
}

void* copy(void* const d)
{
  data* dat;

  if (!d) return NULL;

  dat = (data*)malloc(sizeof(data));
  memcpy(((data*)dat), d, sizeof(data));

  return dat;
}

void* paste(void* const d)
{
  return d;
}

