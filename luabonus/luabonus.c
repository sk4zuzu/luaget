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
#include "table.h"
#include "iter.h"
#include "os.h"
#include "fs.h"
#include "io.h"
#include "inet.h"
#include "select.h"
#include "string.h"
#include "getopt.h"


lb_func_t *lb_regs[] = {
    lb_reg_table,
    lb_reg_iter,
    lb_reg_os,
    lb_reg_fs,
    lb_reg_io,
    lb_reg_inet,
    lb_reg_select,
    lb_reg_string,
    lb_reg_getopt,
    nil
};


static void init_regs(lua_State *L,int nt) {
    int r;
    lb_func_t *reg;
    for (r = 0; (reg = lb_regs[r]); r++) {
        int f;
        for (f = 0; reg[f].name; f++) {
               lua_pushstring(L, reg[f].name);
            lua_pushcfunction(L, reg[f].func);
            lua_settable(L, nt);
        }
    }
}


lb_const_t *lb_cregs[] = {
    lb_creg_inet,
    nil
};


static void init_cregs(lua_State *L,int nt) {
    int r;
    lb_const_t *creg;
    for (r = 0; (creg = lb_cregs[r]); r++) {
        int c;
        for (c = 0; creg[c].name; c++) {
             lua_pushstring(L, creg[c].name);
            lua_pushinteger(L, creg[c].value);
            lua_settable(L, nt);
        }
    }
}


LUALIB_API int luaopen_luabonus(lua_State *L) {
    lua_newtable(L); TOP(nt);
    init_regs(L,nt);
    init_cregs(L,nt);
    return 1;
}

// vim:ts=4:sw=4:et:
