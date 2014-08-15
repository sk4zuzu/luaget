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
#include <dirent.h>
#include <lua.h>

#include "luabonus.h"


// 1:path
int lb_exists(lua_State *L) {
    char *path = (char*) lua_tostring(L, -1);

    int tmp = access(path, F_OK) == zero;

    lua_pop(L, 1); // cleanup
    lua_pushboolean(L, tmp);
    return 1;
}


// 1:path 2:path
int lb_rename(lua_State *L) {
    char *old_path = (char*) lua_tostring(L, -2);
    char *new_path = (char*) lua_tostring(L, -1);

    int tmp = rename(old_path,new_path);

    lua_pop(L, 2); // cleanup
    lua_pushinteger(L, tmp);
    return 1;
}


// 1:str
int lb_unlink(lua_State *L) {
    char *path = (char*) lua_tostring(L, -1);

    int tmp = unlink(path);

    lua_pop(L, 1); // cleanup
    lua_pushinteger(L, tmp);
    return 1;
}


// 0:
int lb_dir_iter(lua_State *L) {
    DIR *dir = (DIR*) lua_touserdata(L, lua_upvalueindex(1));
    struct dirent *ent = readdir(dir);
    if (ent) {
        lua_pushstring(L, ent->d_name);
        return 1;
    }
    closedir(dir);
    lua_pushnil(L);
    return 1;
}
// 1:path
int lb_dir(lua_State *L) {
    TOP(d);
    char *path = (char*) lua_tostring(L, d);
    DIR *dir = opendir(path);
    lua_pop(L, 1); // cleanup
    if (nil == dir) {
        lua_pushnil(L);
        return 1;
    }
    lua_pushlightuserdata(L, dir);
    lua_pushcclosure(L, lb_dir_iter, 1);
    return 1;
}


lb_func_t lb_reg_fs[] = {
    { "exists",lb_exists },
    { "rename",lb_rename },
    { "unlink",lb_unlink },
    {    "dir",lb_dir    },
    {      nil,nil       }
};

// vim:ts=4:sw=4:et:
