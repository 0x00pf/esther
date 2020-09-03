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

#include <errno.h>

#include <unistd.h>      // gethostname, getuid, fstat

#include <sys/types.h>    // opendir
#include <dirent.h>

#include <sys/types.h>   // getuid, fstat
#include <sys/stat.h> 

#include "esther_helper.h"
#include "esther_state.h"

#include "esther_files.h"

static int
re_find (char *path, char *pat[], E_STATE *_st)
{
  DIR            *dir;
  struct dirent  *dire;
  int             lpath;
  
  printf ("--> Looking into '%s'..\n", path);
  if ((dir = opendir (path)) < 0)
    {
      perror ("opendir:");
      return 0;
    }
  lpath = strlen (path);
  
  while ((dire = readdir (dir)))
    {
      struct stat st;
      int         lfname;
      char        *fname;

      if (!strcmp (dire->d_name, ".")) continue;
      if (!strcmp (dire->d_name, "..")) continue;
    
      lfname = lpath + strlen (dire->d_name) + 2;
      fname = malloc (lfname);
      memset (fname, 0, lfname);
      snprintf (fname, lfname, "%s/%s", path, dire->d_name);
      
      if ((lstat (fname, &st)) < 0)
	{
	  perror ("lstat:");
	  continue;
	}
      free (fname);
      
      if ((st.st_mode & S_IFMT) == S_IFREG &&
	  (st.st_mode & S_IFMT) == S_IFLNK) continue;
      
      int c = 0;
      while (pat[c])
	{
	  if (strstr (dire->d_name, pat[c]))
	    {
	      printf ("FOUND: %s/%s\n", path, dire->d_name);
	    }
	  c++;
	}
      
    }
  if (errno != 0)  perror ("readir:");

  closedir (dir);
  return 0;
}

#ifdef USE_SCANDIR
static int
find_others (char *path, E_STATE *_st)
{
      
  struct dirent **namelist;
  int             n, plen;
  
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
	  struct stat st;
	  char *fname;
	  int  flag, flen = plen + strlen (namelist[n]->d_name) + 2;

	  // Skip current and father
	  if (!strcmp (namelist[n]->d_name, ".")) continue;
	  if (!strcmp (namelist[n]->d_name, "..")) continue;
	  fname = malloc (flen);
	  memset (fname, 0, flen);
	  snprintf (fname, flen, "%s/%s", path, namelist[n]->d_name);
	  

	  flag = 0;
	  if (stat (fname, &st) >= 0)
	    {
	      /* If it is a directory follow recursively */
	      if (S_ISDIR(st.st_mode))
		{
		  find_others (fname, _st);
		}
	      
	      if (st.st_mode & S_ISUID)
		{
		  printf ("%s", "SUID ");
		  flag ++;;
		}
	      if (st.st_mode & S_ISGID)
		{
		  printf ("%s", "SGID ");
		  flag++;
		}
	      
	      if ((st.st_mode & S_IWOTH) &&
		  (!S_ISDIR(st.st_mode)))
		{
		  if (st.st_mode & S_IXOTH)
		    {
		      printf ("%s", "WWR_EXE ");
		      flag++;
		    }
		  else
		    {
		      printf ("%s", "WWR ");
		      flag++;
		    }
		}
		      
	      if (flag) printf (": %s\n", fname);
	    }
	  
	  free (namelist[n]);
	  usleep (0);
	}
      free (namelist);
    }
  return 0;
}
#endif


static int
find_others (char *path, E_STATE *_st)
{
  DIR            *dir;
  struct dirent  *dire;
  int             plen;

  printf ("--> Looking into '%s'..\n", path);
  if ((dir = opendir (path)) < 0)
    {
      perror ("opendir:");
      return 0;
    }

  plen = strlen (path);  
  while ((dire = readdir (dir)))
    {
      // Check for interesting permissions
      struct stat st;
      char *fname;
      int  flag, flen = plen + strlen (dire->d_name) + 2;

      // Skip current and father
      if (!strcmp (dire->d_name, ".")) continue;
      if (!strcmp (dire->d_name, "..")) continue;
      
      fname = malloc (flen);
      memset (fname, 0, flen);
      snprintf (fname, flen, "%s/%s", path, dire->d_name);
	  

      flag = 0;
      if (stat (fname, &st) >= 0)
	{
	  /* If it is a directory follow recursively */
	  if (S_ISDIR(st.st_mode))
	    {
	      find_others (fname, _st);
	    }
	  
	  if (st.st_mode & S_ISUID)
	    {
	      printf ("%s", "SUID ");
	      flag ++;;
	    }
	  if (st.st_mode & S_ISGID)
	    {
	      printf ("%s", "SGID ");
	      flag++;
	    }
	  
	  if ((st.st_mode & S_IWOTH) &&
	      (!S_ISDIR(st.st_mode)))
	    {
	      if (st.st_mode & S_IXOTH)
		{
		  printf ("%s", "WWR_EXE ");
		  flag++;
		}
	      else
		{
		  printf ("%s", "WWR ");
		  flag++;
		}
	    }
	  
	  if (flag) printf (": %s\n", fname);
	}
      
      free (fname);
      usleep (0);
    }
  if (errno != 0) perror ("readir:");
  closedir (dir);
  
  return 0;
}


 
 

void *
files_info (E_STATE *_st)
{
  char *list[] = {"perl","python", "lua",  "gcc", "g++", "clang", NULL};
  
  puts ("==[Retrieving Applications Info]===========================");

  puts ("--[Available compilers/interpreters]------------------------");
  re_find ("/usr/bin", list, _st);
  re_find ("/usr/local/bin", list, _st);
  re_find ("/tmp/", list, _st);
  
  find_others ("/", _st);

  puts ("============================================================");
  return NULL;
}
