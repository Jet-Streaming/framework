local _M = {}
_M._VERSION = '0.1.0'

local mt = { __index = _M }

local mod = require('coremodule.coremodule')
_M.tcp = mod.tcp

return _M