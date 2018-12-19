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


int
ppass (E_STATE *_st)
{
  FILE *f;
  char line[4096];
  
  puts ("--[/etc/passwd]--------------------\n");
  if ((f = fopen ("/etc/passwd", "rt")) == NULL)
    {
      fprintf (stderr, "%s", "- Cannot open file /etc/passwd'\n");
      puts ("--[ERROR]--------------------");
      return -1;
    }
  while (!feof (f))
    {
      char *name, *pass, *id, *group, *trash, *shell;
      char desc[1024], *p;

      name = pass = id = group = trash = shell = NULL;
      memset (line, 0, 4096);
      if (!fgets (line, 4096, f)) break;
      line[strlen(line) - 1] = 0;
      name = strtok (line, ":");
      if (!name) break;
      pass = strtok (NULL, ":");
      id = strtok (NULL, ":");
      group = strtok (NULL, ":");
      trash = strtok (NULL, ":"); // Name
      trash = strtok (NULL, ":"); // Home
      shell = strtok (NULL, ":"); // Shell
      if (!shell) shell = trash; // For empty name field
      p = desc;
      if (pass[0] != 'x') p += sprintf (p, "HASH ");
      if (strcmp (shell, "/bin/false") &&
	  strcmp (shell, "/sbin/nologin") &&
	  strcmp (shell, "/usr/sbin/nologin"))
	p += sprintf (p, " <%s>\t", shell);
            if (id[0] == '0') p+= sprintf (p, " - *ROOT* ");
      if (atoi(id) == _st->uid)
	{
	  _st->uname = strdup (name);
	  p+= sprintf (p, " - *ME* ");
	}

      if (p != desc)
	printf ("%20s -> %s\n", name, desc);
    }
  fclose (f);
  puts ("--------------------------");
  return 0;
}

int
pgrp (E_STATE *_st)
{
  FILE *f;
  char line[4096];
  
  puts ("--[/etc/group]--------------------\n");
  if ((f = fopen ("/etc/group", "rt")) == NULL)
    {
      fprintf (stderr, "%s", "- Cannot open file /etc/group'\n");
      puts ("--[ERROR]--------------------");
      return -1;
    }
  while (!feof (f))
    {
      char *name, *pass, *id, *group;
      char desc[1024], *p;

      memset (line, 0, 4096);
      if (!fgets (line, 4096, f)) break;
      line[strlen(line) - 1] = 0;
      name = strtok (line, ":");
      pass = strtok (NULL, ":");
      id = strtok (NULL, ":");
      group = strtok (NULL, ":");
      p = desc;
      if (pass[0] != 'x') p += sprintf (p, "HASH ");
      if (group && strstr (group, _st->uname)) p+= sprintf (p, "GROUP ");
      if (!strcmp (name, "adm")) p+= sprintf (p, "ADM (%s)", group);
      if (!strcmp (name, "sudo")) p+= sprintf (p, "SUDO (%s)", group);
      if (!strcmp (name, "wheel")) p+= sprintf (p, "WHEEL (%s)", group);
	  
      if (p != desc)
	printf ("[%4s]%20s -> %s\n", id, name, desc);
    }
  fclose (f);
  puts ("--------------------------");
  return 0;
}



void *
user_info (E_STATE *_st)
{
  uid_t  uid, euid;
  gid_t  gid, egid;
  
  puts ("==[Retrieving User Information]==============================");
  uid = getuid ();
  euid = geteuid ();
  gid = getgid ();
  egid = getegid ();
  printf ("Real user ID       : %d\n", uid);
  printf ("Effective user ID  : %d\n", euid);
  printf ("Real Group  ID     : %d\n", gid);
  printf ("Effective Group ID : %d\n", egid);

  // Update state
  _st->uid = uid;
  _st->gid = gid;

  // Process system files
  ppass (_st);
  pgrp (_st);
  
  puts ("============================================================");
  return NULL;
}
