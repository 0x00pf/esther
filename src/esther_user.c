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

#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include "config.h"



#include <time.h>
#include <utmp.h>

#include "esther_helper.h"
#include "esther_state.h"


#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#define LASTLOG_FILE "/var/log/lastlog"

#ifndef UTMP_FILE
#define UTMP_FILE "/var/run/utmp"
#endif

#ifndef WTMP_FILE
#define WTMP_FILE "/var/log/wtmp"
#endif

#define LASTLOG_BLOCK 0x124

typedef struct latlost_ent {
  int           unk;
  char          tty[32];
  char          hostname[256];
  unsigned char pad[4891];
} LASTLOGENT;


#ifndef HAVE_PWD_H
#error "pwd.h not found.... getpwuid not yet implemented"
// Add alternative implementation of getpwuid here?
// XXX: Find out availability of the function. 
#endif

int
plast (E_STATE *_st)
{
  LASTLOGENT ent;
  char       uname[1024];
  FILE       *f;
  long       off;
  int        id;
  
  puts ("--[Last log users (lastlog)]------------------------\n");
  if ((f = fopen (LASTLOG_FILE, "rb")) == NULL)
    {
      fprintf (stderr, "- Cannot open file %s\n", LASTLOG_FILE);
      puts ("--[ERROR]--------------------");
      return -1;
      
    }
  
  id = 0;
  while (!feof (f))
    {
      off = id * LASTLOG_BLOCK;
      fseek (f, off, SEEK_SET);
      if ((fread (&ent, LASTLOG_BLOCK, 1,f)) <=0 )  break;
      
      time_t v = (time_t)ent.unk;
      
      struct passwd *r;
      if ((r = getpwuid (id)) == NULL) 	snprintf (uname, 1024, "%d", id);
      else strncpy (uname, r->pw_name, 1024);
      
    
      if (ent.unk != 0)
	printf ("%s\t\t%8s\t%s\t%s",
		uname, ent.tty, ent.hostname[0] ? ent.hostname : "\t", ctime(&v));
      id++;
      
    }
  fclose (f);
  
  return 0;
}

int logged (E_STATE *_st)
{
  FILE *f;
  struct utmp ut;
  
  // Process uTMP
  puts ("--[Current Users (w)]------------------------\n");
  if ((f = fopen (UTMP_FILE, "rb")) == NULL)
    {
      fprintf (stderr, "- Cannot open file %s\n", LASTLOG_FILE);
      puts ("--[ERROR]--------------------");
      return -1;
    }
  
  while (!feof (f))
    {
      if ((fread (&ut, sizeof (struct utmp), 1, f)) <= 0) break;

      int   l = ut.ut_host[0] ? strlen (ut.ut_host): 8; // Poor's man formatting

      /* For current users we will like to know IDLE time as provided by w */
      char   t[1024],_pty[1024];
      time_t t1 = ut.ut_tv.tv_sec;
      
      ctime_r (&t1,t);   /* Generate string for time field */
      t[strlen(t) - 1 ] =0;

      /* Get stat from associated tty... that is the last time the terminal was used */
      snprintf (_pty, 1024, "/dev/%s", ut.ut_line);
      
      struct stat st;
      int         old,idle_t = 0;
      /* Helper structures to human readable time differences*/
      char        *t_unit[] = {"sec", "min", "hour", "day", "month", "year"};
      int         t_conv[] = {60, 60, 24, 30, 12};
      int         order = 0;
      
      if (stat (_pty, &st) >= 0) 
	{
	  idle_t = time(NULL) - st.st_atime;
	  while (idle_t > 0)
	    {
	      old = idle_t;
	      idle_t /= t_conv[order];

	      order++;
	    }
	}

      if (order > 0) order--;
      
      /* For each available entry....*/
      if (ut.ut_user[0])
	{
	  if (old != 0) /* Only show time when there is something to show */
	    printf ("%s\t%s%s\t%s%s\t(%7d)\t(%d %s)\n",
		    ut.ut_user,
		    strlen(ut.ut_user) > 7 ? "":"\t",
		    ut.ut_line,
		    ut.ut_host[0] ? ut.ut_host: "\t",
		    l > 14 ? "" : ((l > 7) ? "\t" : "\t\t"),
		    ut.ut_pid, old, t_unit[order]);
	  else
	    printf ("%s\t%s%s\t%s%s\t\n",
		    ut.ut_user,
		    strlen(ut.ut_user) > 7 ? "":"\t",
		    ut.ut_line,
		    ut.ut_host[0] ? ut.ut_host: "\t",
		    l > 14 ? "" : ((l > 7) ? "\t" : "\t\t"));
	}
    }
  fclose (f);
  
  // Process WTMP
  puts ("--------------------------");
  puts ("--[Last logged in users (last)]------------------------\n");
  if ((f = fopen (WTMP_FILE, "rb")) == NULL)
    {
      fprintf (stderr, "- Cannot open file %s\n", LASTLOG_FILE);
      puts ("--[ERROR]--------------------");
      return -1;
    }
  while (!feof (f))
    {
      if ((fread (&ut, sizeof (struct utmp), 1, f)) <= 0) break;
      char t[1024];
      time_t t1 = ut.ut_tv.tv_sec;
      ctime_r (&t1,t);
      t[strlen(t) - 1 ] =0;
      if (ut.ut_user[0])
	printf ("%s\t\t%s\t%s\t(%7d)\t%s\n",
		ut.ut_user, ut.ut_line, ut.ut_host, ut.ut_pid,t);
      else
	printf (">LOGOUT\t\t%s\t\t\t\t\t%s\n", ut.ut_line, t);
      
    }
  fclose (f);
  puts ("--------------------------");
  return 0;
}

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

      // Shall we use getpwent?. This probably will work on all cases
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
      if (!strcmp (name, "admin")) p+= sprintf (p, "ADM (%s)", group);
      if (!strcmp (name, "sudo")) p+= sprintf (p, "SUDO (%s)", group);
      if (!strcmp (name, "root")) p+= sprintf (p, "ROOT (%s)", group);
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
  // Current information
  
  
  // Update state
  _st->uid = uid;
  _st->gid = gid;

  // Process system files
  plast (_st);
  logged (_st);

  ppass (_st);
  pgrp (_st);
  
  puts ("============================================================");

  return NULL;
}
