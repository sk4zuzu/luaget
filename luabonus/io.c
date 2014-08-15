//
// LUAGET 1.0 20130808 copyright sk4zuzu@gmail.com 2013
//
// This file is part of LUAGET.
//
// LUAGET is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// LUAGET is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LUAGET.  If not, see <http://www.gnu.org/licenses/>.
//

#include <unistd.h>
#include <lua.h>

#include "luabonus.h"


#define BUF_MAX (1500)

typedef int fd_t;


// 1:fd 2:str
int lb_write(lua_State *L) {
    size_t len;
    fd_t fd = lua_tointeger(L, -2);
    char *str = (char*) lua_tolstring(L, -1, &len);
    if (fd < 0 || str == nil) {
        lua_pop(L, 2); // cleanup
        lua_pushinteger(L, -1);
        return 1;
    }

    int tmp = write(fd, str, len); 

    lua_pop(L, 2); // cleanup
    lua_pushinteger(L, tmp);
    return 1;
}


// 1:fd
int lb_read(lua_State *L) {
    int len;
    char buf[BUF_MAX+1];
    fd_t fd = lua_tointeger(L, 1);
    len = read(fd, buf, BUF_MAX);
    if (len <= 0) {
        lua_pop(L, 1); // cleanup
        lua_pushnil(L);
        return 1;
    }
    buf[len] = 0;
    lua_pop(L, 1); // cleanup
    lua_pushstring(L, buf);
    return 1;
}


lb_func_t lb_reg_io[] = {
    { "write",lb_write },
    {  "read",lb_read  },
    {    nil,nil       }
};

// vim:ts=4:sw=4:et:
