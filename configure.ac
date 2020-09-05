dnl * Esther Esther'S The Hard EscalatoR
dnl * (c) pico, 2018
dnl *
dnl * Linux Privilege scalation the hardway
dnl *
dnl * Esther is free software: you can redistribute it and/or modify
dnl * it under the terms of the GNU General Public License as published by
dnl * the Free Software Foundation, either version 3 of the License, or
dnl * (at your option) any later version.
dnl *
dnl * Esther is distributed in the hope that it will be useful,
dnl * but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl * GNU General Public License for more details.
dnl *
dnl * You should have received a copy of the GNU General Public License
dnl * along with Esther.  If not, see <http://www.gnu.org/licenses/>.

AC_INIT([esther], 0.0.3, [@0x00pico])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CONFIG_SRCDIR([src/esther.c])
AC_CONFIG_MACRO_DIR([m4])

AM_PROG_AR
AC_PROG_CC
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL

AC_CHECK_HEADERS([pwd.h])

AC_CONFIG_HEADERS([config.h])

CFLAGS="-g -O0 -Wall -std=c99 -D_GNU_SOURCE=1"

AC_CONFIG_FILES([Makefile \
		src/Makefile])
AC_OUTPUT
