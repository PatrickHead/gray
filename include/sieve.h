#ifndef SIEVE_H
#define SIEVE_H

typedef enum
{
  sieve_process_mode_type_passthru,
  sieve_process_mode_type_edit
} sieve_process_mode_t;

void sieve_set_process_mode(sieve_process_mode_t m);
sieve_process_mode_t sieve_get_process_mode(void);

sieve_process_mode_t str2sieve_process_mode_type(char *s);
char *sieve_process_mode_type2str(sieve_process_mode_t m);

#endif // SIEVE_H
