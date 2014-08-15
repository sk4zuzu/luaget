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

#include <stdio.h>
#include <lua.h>

#include "luabonus.h"


// 1:
static int lb_keys_of_iter(lua_State *L) {
    UP(table);
    UP(index)-1;
    lua_pushvalue(L, index);
    if (zero == lua_next(L, table)) {
        lua_pop(L, 1); // pop table
        lua_pushnil(L);
        return 1;
    }
    lua_pop(L, 1); // pop value
    lua_pushvalue(L, -1);
    lua_replace(L, index);
    return 1;
}


// 1:tab
int lb_keys_of(lua_State *L) {
    lua_pushnil(L); // index (lua_next)
    lua_pushcclosure(L, lb_keys_of_iter, 2);
    return 1;
}


// 1:
static int lb_values_of_iter(lua_State *L) {
    UP(table);
    UP(index)-1;
    lua_pushvalue(L, index);
    if (zero == lua_next(L, table)) {
        lua_pop(L, 1); // pop table
        lua_pushnil(L);
        return 1;
    }
    lua_insert(L, -2);
    lua_replace(L, index);
    return 1;
}


// 1:tab
int lb_values_of(lua_State *L) {
    lua_pushnil(L); // index (lua_next)
    lua_pushcclosure(L, lb_values_of_iter, 2);
    return 1;
}


lb_func_t lb_reg_iter[] = {
    {   "keys_of",lb_keys_of   },
    { "values_of",lb_values_of },
    {         nil,nil          }
};

// vim:ts=4:sw=4:et:
