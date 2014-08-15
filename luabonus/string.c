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
#include <string.h>
#include <stdio.h>
#include <lua.h>

#include "luabonus.h"
#include "table.h"


// 1:str
int lb_basename(lua_State *L) {
    TOP(p);
    char *path = (char*) lua_tostring(L, p);
    char *find = rindex(path, '/');
    lua_pop(L, 1); // cleanup
    if (find) {
        lua_pushstring(L, find+1 );
    }
    return 1;
}


// 1:str 2:str
int lb_split(lua_State *L) {
    int k = 1;
    char *find;
    size_t ptrn_len;
    TOP(p);
    TOP(s)-1;
     char *str = (char*)  lua_tostring(L, s);
    char *ptrn = (char*) lua_tolstring(L, p, &ptrn_len);

    lua_pushnil(L); // lb_table_call
    lua_newtable(L); TOP(nt);
    while ((find = strstr(str, ptrn))) {
        lua_pushlstring(L, str, find - str);
        lua_rawseti(L, nt, k++);
        str = find + ptrn_len;
    }
    lua_pushstring(L, str);
    lua_rawseti(L, nt, k);
    lb_table_call(L);

    lua_remove(L, -2); // cleanup (remove ptrn)
    lua_remove(L, -2); // cleanup (remove str)
    return 1; // return new table
}


lb_func_t lb_reg_string[] = {
    { "basename",lb_basename },
    {    "split",lb_split    },
    {        nil,nil         }
};

// vim:ts=4:sw=4:et:
