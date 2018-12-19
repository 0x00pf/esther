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

#include <mntent.h>

#include "esther_fs.h"

void *
fs_info (E_STATE *_st)
{
  struct mntent *m;
  FILE          *f;
  
  puts ("==[Retrieving File Systems Information]=====================");

  // Get mounted file systems
  if ((f = setmntent("/etc/mtab", "r")) == NULL)
    {
      fprintf (stderr, "Cannot list available filesystems...\n");
      return NULL;
    }

  while ((m = getmntent(f)))
    {
      // Mark interesting filesystems
      if (strstr (m->mnt_opts, "rw") && !strstr (m->mnt_opts,"noexec"))
	printf ("%s", "!! ");
      else
	printf ("%s", "   ");
      printf ("[%10s] %15s -> %s (%s)\n",
	      m->mnt_type, m->mnt_fsname, m->mnt_dir, m->mnt_opts);
      
    }
  
  endmntent(f);
  puts ("============================================================");
  return NULL;
}

