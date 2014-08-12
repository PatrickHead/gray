#include <string.h>
#include <assert.h>

#include "sieve.h"

static sieve_process_mode_t _g_process_mode = sieve_process_mode_type_passthru;

void sieve_set_process_mode(sieve_process_mode_t m)
{
  _g_process_mode = m;
}

sieve_process_mode_t sieve_get_process_mode(void)
{
  return _g_process_mode;
}

sieve_process_mode_t str2sieve_process_mode_type(char *s)
{
  assert(s);

  if (!strcmp(s, "passthru")) return sieve_process_mode_type_passthru;
  if (!strcmp(s, "edit")) return sieve_process_mode_type_edit;

  return sieve_process_mode_type_passthru;
}

char *sieve_process_mode_type2str(sieve_process_mode_t m)
{
  switch (m)
  {
    case sieve_process_mode_type_passthru: return "passthru";
    case sieve_process_mode_type_edit: return "edit";
  }

  return "passthru";
}

