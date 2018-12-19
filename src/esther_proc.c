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
#include <ctype.h>

#include <errno.h>

#include <unistd.h>      // gethostname, getuid, fstat

#include <dirent.h>      // scandir
#include <sys/types.h>   // getuid, fstat
#include <sys/stat.h> 
//#include <sys/utsname.h> // uname

#include "esther_helper.h"

#include "esther_state.h"
#include "esther_proc.h"


static int
scan_proc (E_STATE *_st)
{
  struct dirent **namelist;
  int             n, plen, n_proc = 0;
  char            *path = "/proc/";
  
  if ((n = scandir (path, &namelist, NULL, alphasort)) < 0)
    {
      // Do not show Permission denied errors
      if (errno != EACCES) perror ("scandir:");
      return 0;
    }
  else
    {
      plen = strlen (path);
      while (n--)
	{
	  // Check for interesting permissions
	  char *fname, *proc_name = NULL;
	  int  flen = plen + strlen (namelist[n]->d_name) + 4 + strlen ("cmdline");

	  // Skip current and father
	  if (!strcmp (namelist[n]->d_name, ".")) continue;
	  if (!strcmp (namelist[n]->d_name, "..")) continue;
	  if (!isdigit(namelist[n]->d_name[0])) continue;
	  fname = malloc (flen);
	  memset (fname, 0, flen);
	  // FIXME: use status file to retrieve process name and user
	  snprintf (fname, flen, "%s/%s/cmdline", path, namelist[n]->d_name);
	  proc_name = rpo (fname);
	  if (proc_name)
	    printf ("[%5s] %s\n", namelist[n]->d_name, proc_name);
	  else
	    printf ("[%5s] N/A\n", namelist[n]->d_name);

	  free (proc_name);
	  free (namelist[n]);
	  n_proc++;
	  usleep (0);
	}
      free (namelist);
    }
  printf ("%d processes scanned\n", n_proc);
  return 0;
}

void *
proc_info (E_STATE *_st)
{
  puts ("==[Retrieving Processes Information]========================");
  // Identify system architecture (parse reference bianry /bin/sh???)
  //   - PIE
  //   - Stack canaries
  //   - 32/64 bits
  // Mark up processes with:
  //   Network connections
  //
  scan_proc (_st);
  puts ("============================================================");
  return NULL;
}

