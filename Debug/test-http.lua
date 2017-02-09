
local cf = 'test-http.lua'
print('start ', cf)

--local nonexist = require('nonexist')
--local tcp = require('coremodule').tcp
--local httpmod = require('http')
--local jcmod = require('jcache')
local NetConnection = require('adobe.NetConnection')

-----------------------------------------------------------


-- Print contents of `tbl`, with indentation.
-- `indent` sets the initial level of indentation.
function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. "<->"
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    --else if type(v) == "userdata" then
    --  print(formatting .. 'userdata')
    else
      print(formatting .. v)
    end
  end
end
---------------------------------------------------------
--[[
function test_http_client()
	-- auth: if this IP acceptable
	local tcp = coremod.tcp.new()
	local rc = tcp:connect('220.181.111.188', 80) -- www.baidu.com
		
	local http = httpmod.http.new()

	--http:link('mux', tcp, 'data')
	tcp:link('data', http, 'mux')
	--http:link(tcp)

local options = {
  method = 'GET',
  path = '/index.html',
  headers = {
    --'Content-Type': 'application/x-www-form-urlencoded',
    --'Content-Length': Buffer.byteLength(postData),
    User_Agent = 'jetsrv/0.1.0',
    Host = 'www.baidu.com',
    Accept = '*/*',
    --X_jk = 'hahaha',
  },
}

	print('request: {')
	tprint(options)
	print('request: }')

	local res = http:request(options)

	print('response: {')
	tprint(res)
	print('response: }')

	if res.status ~= 200 then
		--c:send('')
		--c:close()
		--return
		error('error response')
	end

	local jc = jcmod.jcache.new()
	jc.open('mycache')
	jc.set_key('http://www.baidu.com/')
	--jc.write(res) // http header
	http:link('body', jc)
end


function test_server()
	-- auth: if this IP acceptable
	local tcp = coremod.tcp.new()
	tcp:listen('0.0.0.0', 8080, 10)
	local client = tcp:accept()
	print('new client')
	--client:receive()

	--local http = httpmod.http.new()
	--http:link(tcp)

end
]]

function test_rtmp_client()

	local nc = NetConnection.new()
	nc:connect('rtmp://live.hkstv.hk.lxdns.com/live')

	local ns1 = NetStream.new(nc)
	ns1:play('hks')


--[[
	rc = rtmp:call(0, 'createStream')
	msid = rc[4]
	rc = rtmp:call(msid, 'play', 'hks')
	
	rc = rtmp:call(msid, "myFunction", "hello")


	local onStatus = {
		'onStatus',
		0,
		rtmpmod.amf.null,
		-- information
	}
	rtmp.on_call = function (msid, callobj)
		-- body
		local result = {
			'_result', -- 'error'
			callobj[2],
			-- properties
			-- information
		}
		rtmp:send(msid, result, result[2])
	end
]]
end

--tprint(coremod)
--test_http_client()
--test_server()
test_rtmp_client()

--gc.collect()
-----------------------------------------------------------

--assert(nil)



--[[
local function on_request(req)
		print('on_request: req {')
		tprint(req)
		print('on_request: req }')
		
		-- process
		req.status = 200
		req.out_header = {
			Content_type = "text/html",
			Via = "hahaha",
		}

		print('on_request: req {')
		tprint(req)
		print('on_request: req }')
		req:print('hello jet')		
end

local function test1(c)
	local myhttp = create_element("http")
	print('new http:', myhttp)
	print('metatable =', getmetatable(myhttp))
	--c:dummy()
	--c:link(myhttp)
	--c.nonfunc = function() print('in new link') end
	--c.dummy = 0
	--aa = c.dummy
	
	--local req = myhttp:request(myhttp.GET, "baidu.com/abc?k=v")
	--req.status
	
	myhttp.on_request = function(req)
		--print(req.version)

		local reqs = myhttp:reqs()
		if #reqs > 7 then
			myhttp:read_stop()
		end
		
		--local co = jet.thread.spawn(on_request, req)
		local co = coroutine.create(on_request)
		coroutine.resume(co, req)		
		
	end
	c:link(myhttp) --:link(myhttp)
end
-----------------------------------------------------------

srv = create_element("tcp") -- must be global, otherwise will be GC
print('tcp server:', srv)
print('metatable =', getmetatable(srv))
--srv.read_start = 10
--srv.listen = 10

--print(srv.on_connect)
--srv.on_connect()
	

-- AMS: application.clients: Read-only; an Array object containing a list of all the clients connected to an application.
--srv.connections = {}
connections = {}
srv.on_connect = function (c)
		-- AMS: application.acceptConnection(): Accepts a connection call from a client to the server.
		--coroutine.yield()
		print('new tcp:', c)
		print('metatable =', getmetatable(c))
		test1(c)
		table.insert(connections, c)
	end
--print(srv.on_connect)
--srv.on_connect()
srv:listen('0.0.0.0', 8080, 10)
--print('after listen')
print('tcp server:', srv)
--]]

--[[
local function upstream(downstream_elm)
	local tcp1 = create_element('tcp')
	tcp1:connect('upstream.com')
	local http1 = create_element('http')
	http1:link(tcp1)
	local resp = http1:get('upstream.com/d1')
	assert(resp.status == 200)
	http1:link(downstream_elm)
end

local function HuiYuan()
	local tcp1 = create_element('tcp')
	tcp1:connect('upstream.com')
	local http1 = create_element('http')
	http1:link(tcp1)
	local resp = http1:get('upstream.com/d1')
	assert(resp.status == 200)
	local lc1 = create_element('levelcache')
	lc1:create_key('http://upstream.com/d1')
	http1:link(lc1)
end
--]]

--[[
local http = require('http')

srv = http.createServer(function (req)

	assert('table' == type(req))
	print('new request: req {')
	dump_table(req)
	print('new request: req }')

	local oheaders = req.out_header
	local body = 'Hello world\n'
	
	oheaders['Server'] = 'Jet Streaming Server/0.1.0'
	oheaders['Date'] = 'Thu, 07 Jan 2016 05:50:24 GMT'
	oheaders["Content-Type"] = 'text/plain'
	oheaders['Content-Length'] = #body
	oheaders['Content-Length'] = 0
	oheaders['Last-Modified'] = 'Mon, 21 Sep 2015 02:27:54 GMT'
	oheaders['Connection'] = 'keep-alive'
	oheaders['ETag'] = '"55ff6b2a-37c"'
	oheaders['Access-Control-Allow-Origin'] = '*'
	oheaders['Accept-Ranges'] = 'bytes'
	
	req.status = 200
	--req.reason = 'OK'
	--req:say(body)
	http.response(req, body)
	print('response sent')
	
	--res:finish(body)
	
end)
srv:listen('0.0.0.0', 8080, 10)
print('listening on port 8080')
--]]

--[[
local rtmp = require('rtmp')

srv = rtmp.createServer(function (req)
end)
srv:listen('0.0.0.0', 19350, 10)
print('listening on port 19350')
]]

print('end ', cf)