/* Esther Esther'S The Hard EscalatoR
 * (c) pico, 2018
 *
 * Linux Privilege scalation the hardway
 *
 * Esther is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Esther is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Esther.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <unistd.h>      // gethostname, getuid, fstat


#include "esther_helper.h"
#include "esther_state.h"

#include "esther_system.h"
#include "esther_user.h"
#include "esther_files.h"
#include "esther_net.h"
#include "esther_fs.h"
#include "esther_proc.h"

/* Add your information extraction function */
static FUNC func[] = {
  system_info,
  net_info,
  user_info,
  fs_info,
  proc_info,
  files_info,
  NULL
};


int
main (int argc, char *argv[], char *env[])
{
  FUNC    *ptr = func;
  E_STATE _state;
  char    **penv = env;
  
  puts ("ESTHER. Linux Privilege Escalation");
  puts ("--------------------------------------------");
  
  puts ("--[Environment...]--------------------------");
  while (*penv) puts (*(penv++));
  puts ("--------------------------------------------");

  // Run information extraction functions
  while (*ptr) (*ptr++) (&_state);
  
  return 0;
}
