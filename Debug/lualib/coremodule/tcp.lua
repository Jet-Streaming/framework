local _M = {}
_M._VERSION = '0.1.0'

local mt = { __index = _M }

local oldpath = package.path
local mod_path = (...):match("(.-)[^%.]+$")
mod_path = 'coremodule' -- TODO
--print(mod_path)
package.path = package.path .. ';' .. mod_path .. '.bin'
print(package.path)
local mod = require('coremodule')
package.path = oldpath

function _M.new()
end

function _M.createServer(handler)
end

function _M.new()
	return mod.tcp.new()
end

function _M:connect(ip, port)
	return self:connect(ip, port)
end


return _M