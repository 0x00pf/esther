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
  int             n, k, plen, n_proc = 0;
  char            *path = "/proc/";
  char           *fields[3] = {"Name:", "Uid:", NULL};
  char           *res[3] = {NULL, NULL, NULL};
  
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
	  char *fname;
	  char *cmd_line = NULL;
	  int  flen = plen + strlen (namelist[n]->d_name) + 4 + strlen ("cmdline");

	  // Skip current and father
	  if (!strcmp (namelist[n]->d_name, ".")) continue;
	  if (!strcmp (namelist[n]->d_name, "..")) continue;
	  if (!isdigit(namelist[n]->d_name[0])) continue;
	  fname = malloc (flen);
	  memset (fname, 0, flen);
	  // FIXME: use status file to retrieve process name and user
	  snprintf (fname, flen, "%s/%s/cmdline", path, namelist[n]->d_name);
	  cmd_line = rpo (fname);
	  // Get Name and UID
	  snprintf (fname, flen, "%s/%s/status", path, namelist[n]->d_name);
	  if ((k = dump_fields (fname, fields, res)) < 0)
	    {
	      if (cmd_line)
		printf ("[%5s] %s\n", namelist[n]->d_name, cmd_line);
	      else
		printf ("[%5s] N/A\n", namelist[n]->d_name);
	    }
	  else
	    {
	      int uid[4];
	      char mine[16];
	      
	      sscanf (res[1], "%d %d %d %d",
		      &uid[0], &uid[1], &uid[2], &uid[3] );
	      if (uid[0] == _st->uid)
		strcpy (mine, "<MINE> ");
	      else if (uid[0] == 0)
		strcpy (mine, "<ROOT> ");
	      else
		strcpy (mine, "<OTHER>");
	      // TODO: Check for socket files under /proc/PID/fd
	      //       Identify active connections
	      //       List of sockets on /proc/net/tcp|udp
	      //       Use inode field from /proc/net + stat to map to process???
	      printf ("[%5s] %s %20s (UID: R:%5d E:%5d SS:%5d FS:%5d) (%s)\n",
		      namelist[n]->d_name, mine, res[0],
		      uid[0], uid[1], uid[2], uid[3],
		      cmd_line ? cmd_line : "N/A" );
	    }
	  for (;k>=0; k--) if (res[k]) free (res[k]);
	  free (cmd_line);
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

