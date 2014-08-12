#ifndef GRID_API_H
#define GRID_API_H

#include "grid.h"

typedef enum
{
  grid_api_command_nop = 0,
  grid_api_command_up,
  grid_api_command_down,
  grid_api_command_left,
  grid_api_command_right,
  grid_api_command_home,
  grid_api_command_end,
  grid_api_command_goto,
  grid_api_command_edit,
  grid_api_command_show,
  grid_api_command_delete,
  grid_api_command_copy,
  grid_api_command_paste,
  grid_api_command_new_row,
  grid_api_command_del_row,
  grid_api_command_new_column,
  grid_api_command_del_column,
  grid_api_command_get_size,
  grid_api_command_set_size,
  grid_api_command_get_data,
  grid_api_command_set_data
} grid_api_command_t;

typedef struct grid_api_status
{
  int code;
  int rows;
  int columns;
  vertex_s *location;
  void *data;
} grid_api_status_s;

typedef union
{
  int repeat;
  vertex_s *location;
  void *data;
} grid_api_data_u;

typedef struct grid_api
{
  void *_internals;
} grid_api_s;

typedef void *(*grid_api_data_func)(void *data);
typedef void (*grid_api_free_func)(void *data);

grid_api_s *grid_api_create(void);
void grid_api_destroy(grid_api_s *ga);
void grid_api_set_free(grid_api_s *ga, grid_api_free_func func);

void grid_api_set_edit(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_show(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_delete(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_copy(grid_api_s *ga, grid_api_data_func func);
void grid_api_set_paste(grid_api_s *ga, grid_api_data_func func);

grid_api_status_s *grid_api_do(grid_api_s *ga,
                               grid_api_command_t command,
                               grid_api_data_u data);

#endif // GRID_API_H
