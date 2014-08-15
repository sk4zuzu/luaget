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

#include <sys/select.h>
#include <lua.h>

#include "luabonus.h"


#define BUF_MAX (1500)

typedef int fd_t;
typedef fd_t maxfd_t;


// 1:tab 2:secs
int lb_select(lua_State *L) {
    int i;
    int sel;
    fd_t fd;
    int t = 1;
    fd_set rfds;
    maxfd_t maxfd = -1;
    struct timeval tout, *p_tout;

    if (t == lua_gettop(L)) {
        p_tout = nil;
    }
    else if (lua_isnil(L, t+1)) {
        p_tout = nil;
        lua_pop(L, 1);
    }
    else {
        tout.tv_sec  = lua_tointeger(L, t+1);
        tout.tv_usec = 0;
        p_tout = &tout;
        lua_pop(L, 1);
    }
    FD_ZERO(&rfds);
    lua_foreach(L, t) {
        fd = lua_tointeger(L, -1);
        if (fd < 0) {
            continue;
        }
        FD_SET(fd, &rfds);
        if (fd > maxfd) {
            maxfd = fd;
        }
    }
    sel = select(maxfd+1, &rfds, nil, nil, p_tout);
    if (sel < 0) { // error
        lua_pushnil(L);
        return 1;
    }
    lua_newtable(L);
    if (sel == 0) { // timeout
        return 1;
    }
    i = 1;
    lua_rawforeach(L, t) {
        fd = lua_tointeger(L, -1);
        if (FD_ISSET(fd, &rfds)) {
            lua_pop(L, 1);
            lua_pushvalue(L, -1);
            lua_rawseti(L, t+1, i++);
            continue;
        }
        lua_pop(L, 1);
    }
    return 1;
}


// 1:tab
int lb_clean(lua_State *L) {
    fd_t f_fd;
    fd_t b_fd;
    int f = 1;
    int b = lua_rawlen(L, 1); // lua_objlen ?
    for (; f <= b ;) {
        for (;; b--) {
            if (f > b) {
                goto done;
            }
            lua_rawgeti(L, 1, b);
            b_fd = lua_tointeger(L, -1);
            lua_pop(L, 1); 
            if (b_fd < 0) {
                lua_pushnil(L);
                lua_rawseti(L, 1, b);
                continue;
            }
            break;
        }
        for (;; f++) {
            if (f >= b) {
                goto done;
            }
            lua_rawgeti(L, 1, f);
            f_fd = lua_tointeger(L, -1);
            lua_pop(L, 1); 
            if (f_fd < 0) {
                lua_pushinteger(L, b_fd);
                lua_rawseti(L, 1, f);
                lua_pushinteger(L, -1);
                lua_rawseti(L, 1, b);
                break;
            }
            continue;
        }
    }
done:
    return 1;
}


// 1:fds 2:idx
int lb_mark(lua_State *L) {
    lua_pushinteger(L, -1);
    lua_settable(L, 1); 
    return 1;
}


lb_func_t lb_reg_select[] = {
    { "select",lb_select },
    {  "clean",lb_clean  },
    {   "mark",lb_mark   },
    {      nil,nil       }
};

// vim:ts=4:sw=4:et:
