/*!
    @file input.c

    @brief Source code for tracked input source mechanism.

    @timestamp Tue, 07 Oct 2014 07:32:14 +0000

    @author Patrick Head   mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2013  Patrick Head

    @license
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.@n
    @n
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.@n
    @n
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

  /*!

    @file input.c

    This is the source code file for a tracked input source module. 

  */

  // Required system headers

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

  // Module specific headers

#include "input.h"

  // Module specific macros

#define CHUNK 512
#define FALSE 0
#define TRUE 1

  /*!
    @brief Definintion of source types
  */

typedef enum
{
    /*! @brief UNKNOWN source type */
  source_type_none = 0,
    /*! @brief Regular file source type */
  source_type_file,
    /*! @brief STDIN source type */
  source_type_stdin,
    /*! @brief User supplied string buffer source type */
  source_type_buffer
} source_type;

  /*!
    @brief Definition of global source tracking data
  */

static struct
{
    /*! @brief Input source type */
  source_type type;
    /*! @brief Transient file pointer for file source type */
  FILE *file;
    /*! @brief Input buffer, either dynamic or user supplied */
  unsigned char *buffer;
    /*! @brief Current input tracking position */
  long  position;
    /*! @brief Total size of input source in bytes */
  long  size;
    /*! @brief Input EOF flag */
  unsigned char eof;
} _source;

  /*
    Globals
  */

static long *_line_map = NULL;
static long _n_lines = 0L;

  /*
    Function prototypes
  */

static void _clear_source(void);
static void _map_lines(void);
static long _find_line_number(long position);
static long _search_line_number(long position, long ilow, long ihigh);

  /*!

     @brief Initialize an input source.
    
     This function initializes an input source.
     - If the @b source parameter is NULL, then @e stdin will be read into a
       dynamically allocated buffer.
     - If the @b source parameter is a string containing no new-line characters,
       then the string is considered a file pathname, and that file will be
       read into a dynamically allocated buffer.
     - If the @b source parameter is a string containing at least one new-line
       characer, then this string is used as the input buffer in place.
    
     @note The input tracking position is initially set to 0
    
     @note All input source lines are mapped for later line/char tracking.
    
     @note This function is not re-entrant.  Only one input source can be
     managed at a time with this function.
    
     @todo If deemed necessary, implement a multiple input source mechanism.
    
     @param source NULL@n
                   pathname@n
                   string with at least one new-line@n
    
     @retval TRUE  success
     @retval FALSE failure

  */

unsigned char input_initialize(char *source)
{
  static unsigned char first_time = TRUE;
  unsigned char *b = NULL;
  unsigned char *p = NULL;
  int n_read = 0;
  int n_chunks = 0;
  struct stat st;

    // Clear source tracking data if being called for the first time

  if (first_time)
  {
    _clear_source();
    first_time = FALSE;
  }

    // If any source data exists, then we can not initialize, return with
    // failure

  if (_source.file || _source.buffer)
    return FALSE;

    // If the source parameter is NULL, then read from STDIN

  if (!source)
  {
    _source.type = source_type_stdin;
    
    b = p = NULL;
    n_chunks = 0;

    while (TRUE)
    {
      b = (unsigned char *)realloc(b, ((n_chunks + 1) * CHUNK));
      if (!p)
        p = b;
      memset(p, 0, CHUNK);
      n_read = fread(p, sizeof(unsigned char), CHUNK, stdin);
      ++n_chunks;
      p += n_read;
      *p = 0;
      if (n_read < CHUNK)
        break;
    }

    _source.buffer = b;

    _source.size = strlen((char *)_source.buffer);

    _map_lines();

    return TRUE;
  }

    // If source string contains a new-line character, then this is a
    // caller supplied buffer

  else if (strchr(source, '\n'))
  {
    _source.type = source_type_buffer;
    _source.buffer = (unsigned char *)source;
    _source.size = strlen((char *)_source.buffer);

    _map_lines();

    return TRUE;
  }

    // source parameter is the name of a regular file

  else
  {
    if (stat(source, &st))
      return FALSE;
    _source.file = fopen(source, "r");
    if (!_source.file)
      return FALSE;
    _source.type = source_type_file;
    _source.size = st.st_size;

    b = (unsigned char *)malloc(_source.size + 1);
    memset(b, 0, _source.size + 1);

    n_read = fread(b, sizeof(unsigned char), _source.size, _source.file);

    fclose(_source.file);

    _source.file = NULL;

    _source.buffer = b;

    _map_lines();

    return TRUE;
  }

    // Anything else is wrong, return with failure

  return FALSE;
}

  /*!

     @brief Destroy and cleanup an input source.
    
     This function destroys any allocated buffer associated with an input
     source.  Also, the input tracking position is reset to 0, and the line
     mapping is destroyed.
    
  */

void input_cleanup(void)
{
    // Free dynamic buffer, if appropriate

  switch (_source.type)
  {
    case source_type_file:
    case source_type_stdin:
      free(_source.buffer);
      break;
    case source_type_buffer:
      break;
    case source_type_none:
    default:
      return;
  }

    // Clean up the line map

  if (_line_map)
    free(_line_map);

    // Clean up the source tracking data

  _clear_source();

  return;
}

  /*!

     @brief Fetch and return the next byte from an input source.
    
     This function reads the next byte from an input source and returns that
     byte.  If the input source is at its end, then the EOF condition is
     marked.  Also, the input tracking position is updated.
    
     @retval byte value of byte that was read
     @retval 0    on any condition where byte can not be read.

  */

unsigned char input_byte(void)
{
  unsigned char b;

  switch (_source.type)
  {
    case source_type_file:
    case source_type_stdin:
    case source_type_buffer:
      b = _source.buffer[_source.position];
      _source.position++;
      if (_source.position == _source.size)
        _source.eof = TRUE;
      return b;
    default:
     return 0;
  }

  return 0;
}

  /*!

     @brief Returns current input tracking position.
    
     @retval long position

  */

long input_get_position(void)
{
  return _source.position;
}

  /*!

     @brief Set input tracking position.
    
     @param pos

     @retval TRUE  success
     @retval FALSE failure

  */

unsigned char input_set_position(long pos)
{
    // Sanity check parameter

  if (pos < 0)
    return FALSE;

  if (pos >= _source.size)
    return FALSE;

  switch (_source.type)
  {
    case source_type_file:
    case source_type_stdin:
    case source_type_buffer:
      if (pos >= _source.size)
        return FALSE;
      _source.position = pos;
      _source.eof = FALSE;
      return TRUE;
    default:
      break;
  }

  return FALSE;
}

  /*!

     @brief Return input source EOF condition.
    
     @retval TRUE  EOF
     @retval FALSE otherwise

  */

unsigned char input_eof(void)
{
  return _source.eof;
}

  /*!

     @brief Retrieve current input location information.
    
     This function calculates the current input line and character location
     and fills in a user supplied @e input_location struct.
    
     @param loc pointer to @e input_location struct

     @retval TRUE  success
     @retval FALSE failure

  */

unsigned char input_get_location(input_location *loc)
{
  long ln;
  long cp;

    // Sanity check parameter

  if (!loc)
    return FALSE;

    // If line map doesn't exist, it is not possible to determine location

  if (!_line_map)
    return FALSE;

  cp = input_get_position();

  ln = _find_line_number(cp);

  loc->line_number = ln + 1L;
  loc->character_offset = cp - _line_map[ln] + 1L;

  return TRUE;
}

  /*!

     @brief Resets _source structure to all default values
    
  */

static void _clear_source(void)
{
  memset(&_source, 0, sizeof(_source));
  return;
}

  /*!

     @brief Creates a line number mapping array.
    
     This function creates a global line mapping array for detailed input
     source location tracking.  Each array element corresponds to a line
     in the input source, and the value of each element is the byte offset
     from the beginning of the input source of the first byte in the
     corresponding line.

     @note Currently, a line is defined by a string of bytes that end with
           the literal new-line ASCII character.

     @see @e _line_map
    
  */

static void _map_lines(void)
{
  long curr_pos;
  unsigned char b;

  curr_pos = input_get_position();

  _line_map = (long *)malloc(sizeof(long));
  if (!_line_map)
    return;

  _line_map[0] = 0L;

  while (!input_eof())
  {
    b = input_byte();
    if (b == '\n')
    {
      ++_n_lines;
      _line_map = (long *)realloc(_line_map, sizeof(long) * (_n_lines + 1));
      if (!_line_map)
        break;
      _line_map[_n_lines] = input_get_position();
    }
  }

  input_set_position(curr_pos);

  return;
}

  /*!

     @brief Find a line number based on input tracking position.
    
     This function returns the mapped line number based on the input tracking
     @e position.

     @note This function simply serves as the head call to the recursive
           function @e _search_line_number.

     @see @e _search_line_number
    
     @param position

     @retval long line number

  */

static long _find_line_number(long position)
{
  return _search_line_number(position, 0L, _n_lines - 1);
}

  /*!

     @brief Perform binary search for input line number.
    
     This function returns the mapped line number based on the input tracking
     @e position.

     @note This function is recursive.  Normally should be called from
           function @e _find_line_number.

     @see @e _find_line_number
    
     @param position   target position in input source
     @param ilow       low end line number for search
     @param ihigh      high end line number for search

     @retval long      line number of target position in input source

  */

static long _search_line_number(long position, long ilow, long ihigh)
{
  long imid;
  long p1, p2;

  if (position <= 0L)
    return 0L;

  if ((ihigh - ilow) <= 1L)
    return ilow;

  imid = (ilow + ihigh) / 2L;

  if (imid >= ihigh)
    return ihigh;

  p1 = _line_map[imid];
  p2 = _line_map[imid+1];

  if ((p1 <= position) && (p2 > position))
    return imid;

  if (p1 > position)
    ihigh = imid;
  else
    ilow = imid;

  return _search_line_number(position, ilow, ihigh);
}

