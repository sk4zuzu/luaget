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

#ifndef __LUABONUS__H
#define __LUABONUS__H

#define invalid (-1)
#define zero (0)
#define nil ((void*)zero)


#define lua_foreach(L,t) \
    for(lua_pushnil(L);lua_next(L,t);lua_pop(L,1))

#define lua_rawforeach(L,t) \
    for(lua_pushnil(L);lua_next(L,t);)


#define lua_dropkey(L,t,i) \
    lua_pushnil(L);lua_rawseti(L,t,i)

#define lua_pushkey(L,t,k,__code__) \
    lua_pushstring(L,k);__code__;lua_rawset(L,t-2)


#define TOP(name) \
    int name = lua_gettop(L)

#define UP(name) \
    int name = lua_upvalueindex(1)


typedef struct {
    char *name;
    int (*func)(lua_State*);
} lb_func_t;

typedef struct {
    char *name;
    int value;
} lb_const_t;

#endif
// vim:ts=4:sw=4:et:
