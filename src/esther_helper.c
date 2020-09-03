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
#include <stdlib.h>
#include <string.h>

#include <dirent.h>      // scandir
#include <sys/types.h>   // getuid, fstat
#include <sys/stat.h> 


#include "esther_helper.h"

/*
 * Helper functions
 */
char *
rpo (char *path)
{
  FILE *f;
  char *str;
  
  if ((f = fopen (path, "rt")) == NULL)
    {
      fprintf (stderr, "- Cannot open file '%s'\n", path);
      return NULL;
    }
  if ((str = malloc (8192)) == NULL)
    {
      fprintf (stderr, "- Cannot allocate internal buffer\n");
      goto rpo_exit;
    }
  if (fread (str, 1, 8192, f) == 0)
    {
      //fprintf (stderr, "- Cannot read data from file '%s'\n", path);
      free (str);
      str = NULL;
    }
  
 rpo_exit:
  fclose (f);
  return str;
}

int
cat (char *path)
{
  FILE *f;
  char line[4096];
  
  printf ("--[%s]--------------------\n", path);
  if ((f = fopen (path, "rt")) == NULL)
    {
      fprintf (stderr, "- Cannot open file '%s'\n", path);
      puts ("--[ERROR]--------------------");
      return -1;
    }
  while (!feof (f))
    {
      memset (line, 0, 4096);
      fgets (line, 4096, f);
      printf ("%s", line);
    }
  fclose (f);
  puts ("--------------------------");
  return 0;
}

int
re_cat (char *path, char *pat)
{
  struct dirent **namelist;
  int             n, plen;


  if ((n = scandir (path, &namelist, NULL, alphasort)) < 0)
    {
      perror ("scandir:");
      return 0;
    }
  else
    {
      plen = strlen (path);
      while (n--)
	{
	  // Only dump Regular files and links
	  if (namelist[n]->d_type != DT_REG &&
	      namelist[n]->d_type != DT_LNK) continue;
	  if (strstr (namelist[n]->d_name, pat))
	    {
	      char *fname;
	      int  flen = plen + strlen (namelist[n]->d_name) + 2;
	      fname = malloc (flen);
	      memset (fname, 0, flen);
	      snprintf (fname, flen, "%s/%s", path, namelist[n]->d_name);
	      cat (fname);
	      free (fname);
	      free (namelist[n]);
	    }
	}
      free (namelist);
    }
  return 0;
}

char *
e_trim (char *c)
{
  while (*(c++) == ' ');
  return c;
}

// Dump multiple fields from a file
// res array should be initialised
int
dump_fields (char *path, char *pat[], char *res[])
{
  FILE *f;
  char line[4096], *aux;
  int  i, cnt = 0;

  if (!res) return -1;
  if ((f = fopen (path, "rt")) == NULL)
    {
      fprintf (stderr, "- Cannot open file '%s'\n", path);
      puts ("--[ERROR]--------------------");
      return -1;
    }
  while (!feof (f))
    {
      memset (line, 0, 4096);
      fgets (line, 4096, f);
      line[strlen(line) - 1] = 0;
      // Check for patterns
      for (i = 0; pat[i]; i++)
	if ((aux = strstr(line, pat[i])))
	  res[cnt++] = strdup (e_trim (aux + strlen(pat[i])));
    }
  fclose (f);
  return cnt;
}
