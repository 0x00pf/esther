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

#ifndef ESTHER_HELPER_H
#define ESTHER_HELPER_H

#define PON(v)(v?v:NULL)

#ifdef __cplusplus
extern "C" {
#endif
  
  char *rpo (char *path);
  int  cat (char *path);
  int  re_cat (char *path, char *pat);
  int  dump_fields (char *path, char *pat[], char *res[]);
#ifdef __cpluslus
}
#endif

#endif
