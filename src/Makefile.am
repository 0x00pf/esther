# * Esther Esther'S The Hard EscalatoR
# * (c) pico, 2018
# *
# * Linux Privilege scalation the hardway
# *
# * Esther is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * Esther is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with Esther.  If not, see <http://www.gnu.org/licenses/>.

bin_PROGRAMS=esther

estherincludedir  = $(includedir)/esther
estherinclude_HEADERS=esther_system.h esther_state.h esther_user.h esther_net.h esther_files.h \
		      esther_proc.h esther_fs.h esther_helper.h
EXTRA_DIST=esther_system.h esther_state.h esther_user.h esther_net.h esther_files.h \
		      esther_proc.h esther_fs.c esther_helper.h

esther_SOURCES=esther.c esther_system.c esther_user.c esther_files.c esther_net.c \
	       esther_proc.c esther_fs.c esther_helper.c
