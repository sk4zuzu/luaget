--
-- LUAGET 1.0 20130808 copyright sk4zuzu@gmail.com 2013
--
-- This file is part of LUAGET.
--
-- LUAGET is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- LUAGET is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with LUAGET.  If not, see <http://www.gnu.org/licenses/>.
--

local lb = require'luabonus'


function print_r(source,indent)
    indent = indent or ''
    if 'table' == type(source) then
        io.write(indent)
        io.write'{\n'
        for k,v in pairs(source)
         do io.write(k..': ') print_r(v,indent..'\t') end
        io.write(indent)
        io.write'}\n'
    elseif 'boolean' == type(source) then
        io.write(indent)
        io.write'"'
        if source
         then io.write('true')
         else io.write('false') end
        io.write'"\n'
    elseif 'function' == type(source) then
        io.write(indent)
        print(source)
    else
        io.write(indent)
        io.write'"'
        io.write(source or 'nil')
        io.write'"\n'
    end
end


do
     lb.table()
     
     string.basename = lb.basename
        string.split = lb.split
             keys_of = lb.keys_of
           values_of = lb.values_of
end


return lb

-- vim:ts=4:sw=4:et:
