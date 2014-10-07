/*!
    @file callback.c

    @brief Source code for callback system.

    @timestamp Tue, 07 Oct 2014 04:22:50 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

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

    @file callback.c

    This is the source code file for a callback function mechanism.

    This callback mechanism is intended to be as generic as possible.  Any
    software that may want to provide callback hooks for external user code
    can use this mechanism.

    This mechanism provides a callback table, that in turn contains callback
    record entries.  Each callback entry has a tag (name), and three function
    pointers available for entry, success and failure callbacks.

    In turn, for any callback entry provided by the initiating software
    function, user software can then register its own callback functions, which
    provide the actual actions that are triggered by the callback initiating
    software.

    Generally, an individual software function may create a callback entry into
    the callback table.  The only requirement for the entry is a name.  In
    addition, this initiating function then will trigger callbacks at
    entry into the initiating function, and at each location in the initiating
    function where a successful, or failed return will occur.
  */

  // Required system headers

#include <stdlib.h>
#include <string.h>

  // Project related headers

#include "callback.h"

  // Module function prototypes

static int find_entry_by_tag(callback_table *cbt, char *tag);

  /*!

     @brief Initialize the callback mechanism.
    
     This function creates a callback table, initializes it, and returns
     a pointer to the new callback table.
    
     @retval "callback_table *" success
     @retval NULL               failure

  */

callback_table *callback_initialize(void)
{
  callback_table *cbt;

    // Allocate the callback table, return on failure

  cbt = (callback_table *)malloc(sizeof(callback_table));
  if (!cbt)
    return NULL;

    // Initialize the table to nothingness

  memset(cbt, 0, sizeof(callback_table));

  return cbt;
}

  /*!

     @brief Create a new callback entry in callback table.
    
     This function appends a new callback entry to the callback_table.
     The callback_table is dynamically resized to accomodate the new
     callback_entry.
     The callback_table's size is incremented.
    
     @param cbt callback_table *
     @param tag string containing name of new callback_entry
    
     @retval  0 success
     @retval -1 failure

  */

int callback_create(callback_table *cbt,
                    char *tag)
{
  callback_entry *cbe;

    // Sanity check parameters

  if (!cbt)
    return -1;

  if (!tag)
    return -1;

    // Lookup the callback entry in the table by tag name,
    // if the entry already exists, return with failure.

  if ((find_entry_by_tag(cbt, tag)) >= 0)
    return -1;

    // Allocate space for the new callback entry in the callback table array.

  if (!cbt->size)
    cbt->callbacks = malloc(sizeof(callback_entry));
  else
    cbt->callbacks = realloc(cbt->callbacks,
                             sizeof(callback_entry) * (cbt->size + 1));
  if (!cbt->callbacks)
    return -1;

    // Establish and initialize the new callback entry at end of array.

  cbe  = &cbt->callbacks[cbt->size];

  memset(cbe, 0, sizeof(callback_entry));

  if (tag)
    cbt->callbacks[cbt->size].tag = strdup(tag);

    // Record new callback entry in table

  ++cbt->size;

    // Return with success

  return 0;
}

  /*!

     @brief Registers a callback function.
    
     This function registers a callback function to a specific callback_entry
     in the callback_table for a specific callback_type.
    
     @param cbt      callback_table *
     @param tag      string containing name of new callback_entry
     @param type     callback_type
     @param function pointer to function with signature of:@n
                     int (*function)(void *data)
    
     @retval  0 success
     @retval -1 failure

  */

int callback_register(callback_table *cbt,
                      char *tag,
                      callback_type type,
                      int (*function)(void *data))
{
  callback_entry *cbe;
  int i;

    // Sanity check parameters

  if (!cbt)
    return -1;

  if (!tag)
    return -1;

    // Lookup callback entry in callback table by tag name, return on failure

  i = find_entry_by_tag(cbt, tag);
  if (i < 0)
    return -1;

     // Get convenience pointer to found callback entry.

  cbe  = &cbt->callbacks[i];

    // Set appropriate callback function, based on callback type

  switch (type)
  {
    case entry:
      cbe->enter = function;
      break;
    case success:
      cbe->succeed = function;
      break;
    case fail:
      cbe->fail = function;
      break;
  }

    // Return with success

  return 0;
}

  /*!

     @brief Makes a callback by index into callback_table.
    
     This function will call the callback registered for the callback_entry
     located at index postion into callback_table for the desired
     callback_type.@n
     If no callback function is registered for the specific callback_type,
     then an error is returned.
     If the index is out of range, then an error is returned.
    
     @param cbt callback_table *
     @param index integer offset position of callback_entry in
                  callback_table
     @param type callback_type
     @param data pointer to user supplied data of any type.
    
     @retval  0 success
     @retval -1 failure

  */

int callback_by_index(callback_table *cbt,
                      int index,
                      callback_type type,
                      void *data)
{
  int (*func)(void *data);

    // Sanity check parameters

  if (!cbt)
    return -1;

  if (index < 0)
    return -1;

  if (index >= cbt->size)
    return -1;

    // Establish the function pointer for callback, based on type

  switch (type)
  {
    case entry:
      func = cbt->callbacks[index].enter;
      break;
    case success:
      func = cbt->callbacks[index].succeed;
      break;
    case fail:
      func = cbt->callbacks[index].fail;
      break;
  }

    // If callback type does not have a registered function, then return with
    // failure

  if (!func)
    return -1;

    // Call the callback function, and return its status

  return func(data);
}

  /*!

     @brief Makes a callback by tag name of callback_entry in
            callback_table.
    
     This function will call the callback registered for the callback_entry
     with an exact tag name in the callback_table for the desired
     callback_type.@n
     If no callback function is registered for the specific callback_type,
     then an error is returned.
     If no callback_entry exists with the tag name then an error is returned.
    
     @param cbt callback_table *
     @param tag string containing name of callback_entry in the
                callback_table
     @param type callback_type
     @param data pointer to user supplied data of any type.
    
     @retval  0 success
     @retval -1 failure

  */

int callback_by_tag(callback_table *cbt,
                    char *tag,
                    callback_type type,
                    void *data)
{
  int i;

    // Sanity check parameters

  if (!cbt)
    return -1;

  if (!tag)
    return -1;

    // Locate callback by tag name, return on failure

  i = find_entry_by_tag(cbt, tag);
  if (i < 0)
    return -1;

    // Make callback, using index number of found callback

  return callback_by_index(cbt, i, type, data);
}

  /*!

     @brief Locate a callback_entry in the callback_table by tag name.
    
     This function will lookup the callback_entry in the callback_table
     by tag name.  If the callback_entry exists, then the index of the
     matching callback_entry is returned.  Otherwise, an error is returned.
    
     @param cbt callback_table *
     @param tag string containing name of callback_entry in the
                  callback_table
    
     @retval int index of matching callback_entry
     @retval -1  failure

  */

static int find_entry_by_tag(callback_table *cbt, char *tag)
{
  int i;

    // Sanity check parameters

  if (!cbt)
    return -1;

  if (!tag)
    return -1;

    // Simple sequential search of callback entries in callback table.
    // Return on successful search match

  for (i = 0; i < cbt->size; i++)
    if (!strcmp(cbt->callbacks[i].tag, tag))
      return i;

    // Return with failure on NO MATCH

  return -1;
}

