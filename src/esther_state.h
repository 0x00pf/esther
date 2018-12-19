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

#ifndef ESTHER_STATE_H
#define ESTHER_STATE_H

#include <sys/types.h>
#include <unistd.h>

  /*
   * Let's store some state retrieved from the different tests
   * and pass it in to the different functions just in case
   */
  typedef struct e_state_t
  {
    char     *uname;
    uid_t    uid;
    gid_t    gid;
    int      n_glist;
    gid_t    *glist;
  } E_STATE;
  

  /* Kindof Poor-man's Plug-ins*/
  typedef void *(*FUNC)(E_STATE *);



#endif
