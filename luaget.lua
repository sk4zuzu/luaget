#!/usr/bin/env lua
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


--[[
setmetatable(table,getmetatable(table) or {})
getmetatable(table)
['instance_mt'] = {
    ['__index'] = function(_,k)
                   return table[k]
                  end
}
getmetatable(table)
['__call'] = function(_,t)
              setmetatable(t,getmetatable(table)['instance_mt'])
              return t
             end
]]


local errstr = {
    [1] = 'Could not open partfile',
    [2] = 'Unknown option',
    [3] = 'Unable to get content length',
    [4] = 'File \'%s\' already exists. Use overwrite.'
}


local function show_version()
    print('LUAGET 1.0 20130808, sk4zuzu@gmail.com')
end


local function show_help()
    show_version()
    print()
    print('Usage: luaget [OPTIONS] <URL>'            )
    print('OPTIONS:'                                 )
    print(' -v               - show version info'    )
    print(' -h               - show this help screen')
    print(' -x               - overwrite output file')
    print(' -n <INTEGER>     - set number of threads')
    print(' -b <COOKIEFILE>  - use curl cookiefile'  )
    print()
end


local lb = require'lb' -- 'luabonus'
local lc = require'luacurl'


local function get_content_length(url,cookiefile)
    local result = {}
    local c = lc.new()
    c:setopt(lc.OPT_WRITEDATA, result)
    c:setopt(lc.OPT_WRITEFUNCTION,
        function(result,line)
            local x = line:match('Content[-]Length: (%d+)')
            if x then
                result.length = x
            end
            return line:len()
        end
    )
    if cookiefile
     then c:setopt(lc.OPT_COOKIEFILE, cookiefile) end
    c:setopt(lc.OPT_FOLLOWLOCATION, true)
    c:setopt(lc.OPT_NOPROGRESS, true)
    c:setopt(lc.OPT_HEADER, true)
    c:setopt(lc.OPT_NOBODY, true)
    c:setopt(lc.OPT_URL, url)
    assert( c:perform() ~= 0 )
    c:close()
    return assert( result.length, errstr[3] )
end


local function get_content(url,cookiefile,range,file,opts)
    local c = lc.new()
    c:setopt(lc.OPT_WRITEDATA, file)
    c:setopt(lc.OPT_WRITEFUNCTION,
        function(stream,buffer)
            stream:write(buffer)
            return buffer:len()
        end
    )
    c:setopt(lc.OPT_PROGRESSDATA, opts)
    c:setopt(lc.OPT_PROGRESSFUNCTION,
        function(opts,dltotal,dlnow)
            if dltotal == 0
             then return end
            local now = lb.time()
            if now-opts.before <= 0
             then return end
            local msg = string.format(
                '%d:%d%%:%dK/s', opts.n,
                math.ceil((dlnow+opts.seek)/
                    (dltotal+opts.seek)*100),
                math.ceil((dlnow-opts.dlbefore)/
                        (now-opts.before)/1024)
            )
            lb.write(opts.pipes[opts.n].w, msg)
              opts.before = now
            opts.dlbefore = dlnow
        end
    )
    if cookiefile
     then c:setopt(lc.OPT_COOKIEFILE, cookiefile) end
    c:setopt(lc.OPT_FOLLOWLOCATION, true)
    c:setopt(lc.OPT_NOPROGRESS, false)
    c:setopt(lc.OPT_RANGE, range)
    c:setopt(lc.OPT_URL, url)
    assert( c:perform() ~= 0 )
    c:close()
end


local function close_all_other_pipes(pipes,max,n)
    for k = n+1,max do
        lb.close(pipes[k].r)
        lb.close(pipes[k].w)
    end
    lb.close(pipes[n].r)
end


local function close_specific_pipe(pipes,n)
    lb.close(pipes[n].w)
end


local function partfile_name(root_name,part,parts)
    return string.format('.%s.lg_%dof%d', root_name,
                                          part,
                                          parts)
end


local function partfile_info(n,parts,length)
    local count = math.floor(length/parts)
    local start = count*(n-1)
    if n == parts
     then count = length-start end
    local stop = start+count-1
    return count, start, stop
end


local function worker(opts)
    close_all_other_pipes(opts.pipes,
                          opts.parts,
                          opts.n)
    local name = partfile_name(lb.basename(opts.url),
                                           opts.n,
                                           opts.parts)
    local count, start, stop = partfile_info(opts.n,
                                             opts.parts,
                                             opts.length)
    local file = assert( io.open(name,'r+') or
                         io.open(name,'w+'), errstr[1] )
    local seek = file:seek('end', 0);
    if start + seek < stop then
                start = start + seek
            opts.seek = seek
          opts.before = lb.time()
        opts.dlbefore = 0
        get_content(opts.url,
                    opts.cookiefile,
                    tostring(start)..'-'..stop,
                    file,
                    opts)
        lb.write(opts.pipes[opts.n].w, tostring(opts.n)..':done')
        lb.close(opts.pipes[opts.n].w)
    end
    file:close()
end


local function monitor(pipes)
    local pg_hash = {}
    local rfds = table({})
    for _,p in ipairs(pipes)
     do rfds:insert(p.r) end
    while true do
        local ready = lb.select(rfds)
        if not ready
         then break end
        for _,i in ipairs(ready) do
            local str = lb.read(rfds[i])
            if not str
             then lb.mark(rfds,i)
             else pg_hash[ rfds[i] ] = str end
        end
        if #lb.clean(rfds) == 0
         then break end
        for _,pg in pairs(pg_hash)
         do lb.write(1, ' '..pg ) end
        lb.write(1, (' '):rep(8)..'\r' )
    end
    lb.write(1, '\n' )
end


local function final_concat(parts,final_name)
    local cleanup = function(f)
        for _,x in ipairs(f)
         do x:close() end
    end
    local name = partfile_name(final_name, 1, parts)
    local f = {}
    f[1] = assert( io.open(name,'r+'), errstr[1] )
    if not f[1]
     then return end
    f[1]:seek('end', 0)
    for n = 2,parts do
        name = partfile_name(final_name, n, parts)
        local x = assert( io.open(name,'r'), errstr[1] )
        if not x
         then cleanup(f) return end
        f[n] = x 
    end
    for n = 2,parts do
        while true do
            local data = f[n]:read(1048576)
            if not data
             then break end
            f[1]:write(data)
        end
    end
    cleanup(f)
end


local function final_cleanup(parts,final_name)
    local name = partfile_name(final_name, 1, parts)
    lb.rename(name, final_name)
    for n = 2,parts do
        name = partfile_name(final_name, n, parts)
        lb.unlink(name)
    end
end


local function parse_cmdopts(local_arg)
    local opt = nil
    local cmdopts = { overwrite = false, parts = 4 } -- defaults
    while true do
        opt = lb.getopt(local_arg, 'vhxn:b:')
            if opt.ret == -1
             then break
        elseif opt.ret == string.byte('v')
             then show_version() os.exit(0)
        elseif opt.ret == string.byte('h')
             then show_help()    os.exit(0)
        elseif opt.ret == string.byte('x')
             then cmdopts.overwrite = true
        elseif opt.ret == string.byte('n')
             then cmdopts.parts = tonumber(opt.arg)
        elseif opt.ret == string.byte('b')
             then cmdopts.cookiefile = opt.arg
        elseif opt.ret == string.byte('?')
             then assert(nil, errstr[2] )
           end
    end
    if not local_arg[opt.ind]
     then show_help() os.exit(1) end
    cmdopts.url = local_arg[opt.ind]
    return cmdopts
end


do
    local    cmdopts = parse_cmdopts(arg)
    local     length = get_content_length(cmdopts.url, cmdopts.cookiefile)
    local      pipes = table({})
    local final_name = lb.basename(cmdopts.url)
    if not cmdopts.overwrite and lb.exists(final_name)
     then assert(nil, string.format(errstr[4], final_name)) end
    print('Content-Length: '..length..' bytes')
    print(' Parts/Threads: '..cmdopts.parts)
    print('    Cookiefile: '..(cmdopts.cookiefile or 'no'))
    print('      Filename: '..final_name)
    for n = 1,cmdopts.parts do
        local r,w = lb.pipe()
        pipes:insert({ r = r, w = w })
    end
    for n = 1,cmdopts.parts do
        lb.spawn({ n = n,
                 url = cmdopts.url,
               parts = cmdopts.parts,
               pipes = pipes,
              length = length,
          cookiefile = cmdopts.cookiefile }, worker)
        close_specific_pipe(pipes, n)
    end
    monitor(pipes)
    for n = 1,cmdopts.parts
     do lb.wait() end
    final_concat (cmdopts.parts, final_name)
    final_cleanup(cmdopts.parts, final_name)
end

-- vim:ts=4:sw=4:et:
