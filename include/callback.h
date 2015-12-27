/*!
    @file callback.h

    @brief Header file for callback system.

    @timestamp Fri, 09 Oct 2015 05:37:44 +0000

    @author Patrick Head  mailto:patrickhead@gmail.com

    @copyright Copyright (C) 2013  Patrick Head

    @license
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.\n
    \n
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.\n
    \n
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CALLBACK_H
#define CALLBACK_H

  /*!

    @file callback.h

    This is the header file for a callback function mechanism.

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
    addition, this initiating function then will trigger callbacks for
    entry into the initiating function, and at each location in the initiating
    function were a successful, or failed return will occur.
  */

  /*!
    @brief Callback table entry
  */

typedef struct
{
    /*! @brief unique name of callback entry */
  char *tag;
    /*! @brief user registered entry callback function */
  int (*enter)(void *data);
    /*! @brief user registered success callback function */
  int (*succeed)(void *data);
    /*! @brief user registered failure callback function */
  int (*fail)(void *data);
} callback_entry;

  /*!
    @brief Callback table
  */

typedef struct
{
    /*! @brief number of entries */
  int size;
    /*! @brief array of callback entries */
  callback_entry *callbacks;
} callback_table;

  /*!
    @brief Types of callback entries
  */

typedef enum
{
    /*! @brief function entry callback */
  entry = 0,
    /*! @brief function exit on success callback */
  success,
    /*! @brief function exit on failure callback */
  fail
} callback_type;

callback_table *callback_initialize(void);

int callback_create(callback_table *cbt,
                    char *tag);

int callback_register(callback_table *cbt,
                      char *tag,
                      callback_type type,
                      int (*function)(void *data));

int callback_by_index(callback_table *cbt,
                      int index,
                      callback_type type,
                      void *data);

int callback_by_tag(callback_table *cbt,
                    char *tag,
                    callback_type type,
                    void *data);

#endif // CALLBACK_H

