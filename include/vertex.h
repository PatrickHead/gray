#ifndef VERTEX_H
#define VERTEX_H

typedef struct
{
  char *tag;
  double x;
  double y;
  double z;
} vertex_s;

vertex_s *vertex_create(void);
void vertex_destroy_void(void *v);
void vertex_destroy(vertex_s *v);
vertex_s *vertex_copy(vertex_s *v);

void vertex_set(vertex_s *v, char *tag, double x, double y, double z);
void vertex_get(vertex_s *v, char **tag, double *x, double *y, double *z);

void vertex_set_tag(vertex_s *v, char *tag);
char *vertex_get_tag(vertex_s *v);
void vertex_set_x(vertex_s *v, double x);
double vertex_get_x(vertex_s *v);
void vertex_set_y(vertex_s *v, double y);
double vertex_get_y(vertex_s *v);
void vertex_set_z(vertex_s *v, double z);
double vertex_get_z(vertex_s *v);

vertex_s *str2vertex(char *s);
char *vertex2str(vertex_s *v);

#endif // VERTEX

