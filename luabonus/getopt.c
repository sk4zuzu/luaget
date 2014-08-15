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


// 1:tab 2:str
int lb_getopt(lua_State *L) {
    int optret;
    int argc = 0;
    char *argv[1024];
    TOP(o); // optstring
    TOP(t)-1; // table
    char *optstring = (char*) lua_tostring(L, o);

    lua_rawgeti(L, t, 0);
    argv[argc++] = (char*) lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_dropkey(L, t,  0);
    lua_dropkey(L, t, -1);
    lua_foreach(L, t) {
        argv[argc++] = (char*) lua_tostring(L, -1);
    }
    opterr = 0;
    optret = getopt(argc, argv, optstring);
   
    lua_pop(L, 2); // cleanup (pop table and optstring)
   
    lua_newtable(L);
    lua_pushkey(L, -1, "ret", lua_pushinteger(L, optret) );
    lua_pushkey(L, -1, "arg", lua_pushstring (L, optarg) );
    lua_pushkey(L, -1, "ind", lua_pushinteger(L, optind) );
    // lua_pushkey(L, -1, "err", lua_pushinteger(L, opterr) );
    lua_pushkey(L, -1, "opt", lua_pushinteger(L, optopt) );

    return 1;
}


lb_func_t lb_reg_getopt[] = {
    { "getopt",lb_getopt },
    {      nil,nil       }
};

// vim:ts=4:sw=4:et:
