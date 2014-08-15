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
#include <stdlib.h>
#include <stdio.h>
#include <lua.h>

#include "luabonus.h"

typedef int fd_t;


// 1:secs
int lb_sleep(lua_State *L) {
    sleep( lua_tointeger(L, -1) );
    lua_pop(L, 1); // cleanup (pop secs)
    return 0;
}


// 1:
int lb_time(lua_State *L) {
    time_t t; time(&t);
    lua_pushinteger(L, t);
    return 1;
}


// 1:
int lb_pipe(lua_State *L) {
    fd_t fd[2];
    if (pipe(fd) < 0) {
        lua_pushnil(L);
        lua_pushnil(L);
        return 2;
    }
    lua_pushinteger(L, fd[0]);
    lua_pushinteger(L, fd[1]);
    return 2;
}


// 1:fd
int lb_close(lua_State *L) {
    fd_t fd = lua_tointeger(L, -1);
    if (fd) {
        close(fd);
    } 
    lua_pop(L, 1); // cleanup (pop fd)
    return 0;
}


// 1:opts 2:proc
int lb_spawn(lua_State *L) {
    pid_t pid = fork(); 
    if (pid == -1) {
        printf("%s\n", "unable to fork()");
        lua_pop(L, 2); // cleanup
        lua_pushnil(L);
        return 1;
    }
    if (pid == 0) {
        lua_insert(L, -2); // xchg opts and proc
        lua_call(L, 1, 1);
        exit( lua_tointeger(L, -1) ); 
        lua_pushnil(L);
        return 1;
    }
    lua_pop(L, 2); // cleanup
    lua_pushinteger(L, pid);
    return 1;
}


// 1:
int lb_wait(lua_State *L) {
    int status; wait(&status);  
    return 0;
}


lb_func_t lb_reg_os[] = {
    { "sleep",lb_sleep },
    {  "time",lb_time  },
    {  "pipe",lb_pipe  },
    { "close",lb_close },
    { "spawn",lb_spawn },
    {  "wait",lb_wait  },
    {     nil,nil      }
};

// vim:ts=4:sw=4:et:
