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
#include <stdio.h>
#include <lua.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "luabonus.h"
#include "os.h"


typedef int fd_t;


// 1:int 2:int
int lb_socket(lua_State *L) {
    TOP(t);   // type
    TOP(d)-1; // domain
    int domain = lua_tointeger(L, d);
    int   type = lua_tointeger(L, t);
    lua_pop(L, 2); // cleanup

    fd_t sock = socket(domain,type,zero);
    if(sock==invalid) lua_pushnil(L);
                 else lua_pushinteger(L, sock);
    return 1;
}


// 1:int 2:int
int lb_bind(lua_State *L) {
    TOP(p);   // port
    TOP(s)-1; // sock
    fd_t sock = lua_tointeger(L, s); 
     int port = lua_tointeger(L, p);

    lua_pop(L, 2); // cleanup

    struct sockaddr_in addr = {
        .sin_family=AF_INET,
        .sin_port=htons(port),
        .sin_addr={ .s_addr=INADDR_ANY }
    };

    int tmp =
     bind(sock,(struct sockaddr *)&addr,sizeof(addr));
    if(tmp==invalid) lua_pushnil(L);
                else lua_pushinteger(L, sock);
    return 1;
}


// 1:int 2:int
int lb_listen(lua_State *L) {
    TOP(b);   // blog
    TOP(s)-1; // sock
    fd_t sock = lua_tointeger(L, s); 
     int blog = lua_tointeger(L, b);

    lua_pop(L, 2); // cleanup

    int tmp = listen(sock,blog);
    if(tmp==invalid) lua_pushnil(L);
                else lua_pushinteger(L, sock);
    return 1;
}


// 1:int
int lb_accept(lua_State *L) {
    TOP(s);
    fd_t sock = lua_tointeger(L, s);

    lua_pop(L, 1); // cleanup

    struct sockaddr_in addr;
             socklen_t len=sizeof(addr);
    fd_t tmp =
     accept(sock,(struct sockaddr *)&addr,&len);
    if(tmp==invalid) {
        lua_pushnil(L);
        return 1;
    }
    else {
        lua_pushinteger(L, tmp);
        lua_pushfstring(L,"%s:%d",inet_ntoa(addr.sin_addr),
                                      ntohs(addr.sin_port));
        return 2;
    }
}


// 1:port 1:blog
int lb_server(lua_State *L) {
    lua_insert(L, -2); // xchg port and blog

    lua_pushinteger(L, AF_INET);
    lua_pushinteger(L, SOCK_STREAM);
    lb_socket(L);

    if(lua_isnil(L, -1)) {
        lua_insert(L, -3); // xchg blog and nil
        lua_pop(L, 2); // cleanup (pop port and blog)
        return 1;
    }

    lua_pushvalue(L, -1); // duplicate sock
    lua_insert(L, -3); // xchg port and 2nd sock
    lua_insert(L, -2); // xchg port and 1st sock
    lb_bind(L);

    if(lua_isnil(L, -1)) {
        lua_insert(L, -2); // xchg nil and sock
        lb_close(L);
        lua_remove(L, -3); // cleanup (remove blog)
        return 1; // return nil
    }

    lua_insert(L, -3); // xchg blog and 2nd sock
    lua_insert(L, -2); // xchg blog and 1st sock
    lb_listen(L);

    if(lua_isnil(L, -1)) {
        lua_insert(L, -2); // xchg nil and sock
        lb_close(L);
        return 1; // return nil
    }

    lua_pop(L, 1); // cleanup (pop 2nd sock)
    return 1; // return sock
}


lb_func_t lb_reg_inet[] = {
    { "socket",lb_socket },
    {   "bind",lb_bind   },
    { "listen",lb_listen },
    { "accept",lb_accept },
    { "server",lb_server },
    {      nil,nil       }
};


lb_const_t lb_creg_inet[] = {
    {     "AF_UNIX",AF_UNIX     }, 
    {    "AF_LOCAL",AF_LOCAL    }, 
    {     "AF_INET",AF_INET     }, 
    {    "AF_INET6",AF_INET6    }, 
    { "SOCK_STREAM",SOCK_STREAM },
    {  "SOCK_DGRAM",SOCK_DGRAM  },
    {           nil,zero        }
};

// vim:ts=4:sw=4:et:
