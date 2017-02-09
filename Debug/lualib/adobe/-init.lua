local _M = {}
_M._VERSION = '0.1.0'

local mt = { __index = _M }

_M.rtmp = require('adobe.rtmp')
--_M.rtmfp = require('adobe.rtmfp')

return _M