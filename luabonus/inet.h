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

#ifndef __LB__INET__H
#define __LB__INET__H

int lb_socket(lua_State *L);
int lb_bind(lua_State *L);
int lb_listen(lua_State *L);
int lb_accept(lua_State *L);

extern  lb_func_t lb_reg_inet[];
extern lb_const_t lb_creg_inet[];

#endif
// vim:ts=4:sw=4:et:
