#ifndef GRID_SIZE_H
#define GRID_SIZE_H

typedef struct
{
  int width;
  int height;
} grid_size_s;

grid_size_s *grid_size_create(void);
void grid_size_destroy_void(void *v);
void grid_size_destroy(grid_size_s *gs);
grid_size_s *grid_size_copy(grid_size_s *gs);

void grid_size_set(grid_size_s *gs, int width, int height);
void grid_size_get(grid_size_s *gs, int *width, int *height);

int grid_size_get_width(grid_size_s *gs);
void grid_size_set_width(grid_size_s *gs, int width);

int grid_size_get_height(grid_size_s *gs);
void grid_size_set_height(grid_size_s *gs, int height);

char *grid_size2str(grid_size_s *gs);
grid_size_s *str2grid_size(char *s);

#endif // GRID_SIZE_H
