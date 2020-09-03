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
#include <sys/utsname.h> // uname

#include "esther_helper.h"
#include "esther_state.h"

#include "esther_system.h"

void *
system_info (E_STATE *_st)
{
  struct utsname un;
  char   name[HOST_NAME_MAX];
  char   *tmp;
  
  puts ("==[Retrieving System Information]=======================");
  
  /*
   * Kernel version... uname -a
   */
  uname (&un);
  printf ("Operating System : %s\n", PON(un.sysname));
  printf ("Host/Node name   : %s\n", PON(un.nodename));
  printf ("Release          : %s\n", PON(un.release));
  printf ("Machine          : %s\n", PON(un.machine));
  printf ("Version          : %s\n", PON(un.version));
#ifdef _GNU_SOURCE
  printf ("NIS/YP Domain    : %s\n", PON(un.domainname));
#endif
  
  /* From uname man page the hostname reported by uname may be
   * different of the hostname set with sethostname
   */
  gethostname (name, HOST_NAME_MAX);
  printf ("GetHostname name : %s\n", name);
  /* read /proc/version... compiler version and others */
  tmp = rpo ("/proc/version");
  printf ("/proc/version    : %s\n", PON(tmp));
  free (tmp);
  

  re_cat ("/etc/", "release");
  re_cat ("/etc/", "issue");

  
  /* strace hostname -> is actually printing uname.nodename */
  puts ("============================================================");
  return NULL;
}

