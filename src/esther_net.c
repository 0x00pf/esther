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

#include "esther_net.h"

/* Let's use NETLINK to get network information (man 7 netlink)*/
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>


void *
net_info (E_STATE *_st)
{
  int  s;
  struct nlmsghdr nlmsg;
  
  puts ("==[Retrieving Network Information]==========================");

  if ((s = socket (AF_NETLINK, SOCK_RAW|SOCK_CLOEXEC, NETLINK_ROUTE)) < 0)
    {
      perror ("socket(NETLINK):");
      return NULL;
    }
  close (s);
  puts ("** Not yet implemented");
  puts ("============================================================");
  return NULL;
}

