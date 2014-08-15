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

#include <lua.h>

#include "luabonus.h"


// 1: 2:
static int lb_table_index(lua_State *L) {
    TOP(_)-1; lua_remove(L, _);
    lua_getglobal(L, "table");
    lua_insert(L, -2);
    TOP(k);
    TOP(t)-1;
    lua_rawget(L, t);
    lua_remove(L, t);
    return 1; // table[k]
}


// 1: 2:
int lb_table_call(lua_State *L) {
    TOP(_)-1; lua_remove(L, _);
    TOP(tgt);
    lua_getglobal(L, "table"); TOP(t);
    lua_getmetatable(L, t); TOP(mt);
    lua_pushstring(L, "instance_mt");
    lua_rawget(L, mt); TOP(i_mt);
    lua_setmetatable(L, tgt);
    lua_pop(L, 2); // t+mt 
    return 1; // tgt
}


// 1:
int lb_table(lua_State *L) {
    lua_getglobal(L, "table"); TOP(t); TOP(mt)+1;
    if (zero == lua_getmetatable(L, t)) {
        lua_newtable(L);
        lua_pushvalue(L, mt);
        lua_setmetatable(L, t);
    }

    lua_pushstring(L, "instance_mt");
    lua_newtable(L); TOP(i_mt);
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, lb_table_index);
    lua_rawset(L, i_mt);
    lua_rawset(L, mt);

    lua_pushstring(L, "__call");
    lua_pushcfunction(L, lb_table_call);
    lua_rawset(L, mt);

    lua_pop(L, 2); // t+mt
    return 0;
}


lb_func_t lb_reg_table[] = {
    { "table",lb_table },
    {     nil,nil      }
};

// vim:ts=4:sw=4:et:
