local ce = jet.plugin.create_element
local spawn = jet.thread.spawn
local tcp = require('tcp')

local _M = {}
_M._VERSION = '0.1.0'

local mt = { __index = _M }

--local connections = {}
print('http.lua in')

local function new(http)
    return {
        element = http
    }
end

--[[
local req_mt = {}
req_mt.__index = req_mt

req_mt.print = function(req, str)
	local http = req.http
	-- sort pipelined reqs
	return http:response(req, str)
end

req_mt.say = function(req, str)
	print('in req_mt.say')
	return req_mt.print(req, str) -- + '\n'
end
]]--

function _M.response(req, body)
	local ok, err = req.http:response(req)
	-- send body in proxy element
end

function _M.createServer(handler)

    local srv = {}
	
    --local tcpsock = tcp.new()
	local tcpsock = ce('ostcp')
	if not tcpsock then
		return nil, 'no such element'
	end
	
    --local ok, err = elm:bind()
    tcpsock:on('connect', function(conn)
		print('http: new connection:', conn)

		local http = ce('http')
		local reqs = {}
		http:on('request', function(req)
			print('http: new request:', req)
			--error('error')
			if (#reqs > 0) then
				print('too many pipelined reqs')
				return jet.EAGAIN
			end
			table.insert(reqs, req)
			local pos = #reqs
			print('http: req pos:', pos)
			
			req.thread = spawn(function ()
				--setmetatable(req, req_mt)
				req.http = http
				req.out_header = {}

--[[
req.print = function(req, str)
	local http = req.http
	-- sort pipelined reqs
	return http:response(req, str)
end

req.say = function(req, str)
	print('in req_mt.say')
	return req.print(req, str) -- + '\n'
end

local ok, err = req:say('body')
print('req:say return:', ok, err)
]]


				--local ok, err = xpcall(handler, 
				--	function() 
				--		print(debug.traceback()) 
				--	end, req)
				
				--local ok, err = pcall(handler, req)
				local ok, err = handler(req)
				--assert(ok, err)
			end)
		end)

		conn:link(http)

		table.insert(srv.clients, conn)
	end)
    
    -- OpenResty: 当客户端过早关闭当前连接（下游）时自动调用
    tcpsock:on('abort', function()
        spawn(function ()
            -- callback(xxx)
        end)
    end)
    
	srv.element = tcpsock
	srv.clients = {}
    return setmetatable(srv, mt)
end

function _M:send()
end

function _M:listen(addr, port, backlog)
	return self.element:listen(addr, port, backlog)
end

function _M.request(url)
end

function _M:set_timeout()
end

function _M.get(url)
    return _M.request('GET', url)
end

-- OpenResty 兼容库
function _M.print(s)
end

function _M.say(s)
	return _M.print(s .. '\n')
end

function _M.exit(status)
end

function _M.on_abort(callback)
end


return _M