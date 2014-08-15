#
# LUAGET 1.0 20130808 copyright sk4zuzu@gmail.com 2013
#
# This file is part of LUAGET.
#
# LUAGET is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LUAGET is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LUAGET.  If not, see <http://www.gnu.org/licenses/>.
#

    CC = gcc
   OUT = luabonus.so
  OBJS = $(patsubst %.c,%.o, $(wildcard *.c))
CFLAGS += -fpic
all: $(OUT)
$(OUT): $(OBJS)
	$(CC) -fpic -shared -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean:
	-rm $(OUT) $(OBJS)

