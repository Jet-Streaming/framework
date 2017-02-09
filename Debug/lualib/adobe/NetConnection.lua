
local tcp = require('coremodule').tcp

-- load native modules
--local pathbak = package.path
--package.path = ''
local rtmp = require('adobe.rtmp')
local rtmcsp = require('adobe.rtmcsp')
--package.path = pathbak

local _M = { _VERSION = '0.1.0' }

local mt = { __index = _M }

--[[
local function resp(rtmp, obj)
	local rc = {
		'_result', -- '_error', 'verifyClient', 'contactExternalServer'
		obj[2], -- Transaction ID
		-- Properties(Object) :
		-- Name-value pairs that describe the properties(fmsver etc.) of the connection.
		{ fmsVer = '"FMS/3,0,1,123"', capabilities = 31, },
		-- Information(Object) :
		-- Name-value pairs that describe the response from|the server. ‘code’,‘level’, ‘description’ are names of few among such information.
		{ level = 'status', code = 'NetConnection.Connect.Success', description = 'Connection succeeded.', objectEncoding = 0 },
		--rtmp.null,
		--rtmp.undefined, 
	}

	print('response AMF object:')
	tprint(rc)
	rtmp:send(rc)
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
		print('rtmp: new connection:', conn)

		local rtmcsp = assert(ce('rtmcsp'))
		local rtmp = assert(ce('rtmp'))

		conn:link(rtmcsp, 'cs');
		rtmcsp:link('msg', rtmp)
		
		rtmp:on('amf', function(obj)
			print('received AMF object:')
			tprint(obj)
			--dump_table(obj)
			resp(rtmp, obj)
		end)

		table.insert(srv.clients, conn)
	end)
        
	srv.element = tcpsock
	srv.clients = {}
    return setmetatable(srv, mt)
end
]]

function _M.new()
	-- body
	return setmetatable({ _rtmpelm = nil }, mt)
end

function _M:connect(url)

	-- TODO resolve url
	local addr = '119.90.20.13' -- live.hkstv.hk.lxdns.com
		--'222.74.236.41',
	local port = 1935

	local tcpelm = tcp.new()
	local rc = tcpelm:connect(addr, port) 

	--tprint(rtmpmod)
	local cselm = rtmcsp.rtmcsp.new()
	tcpelm:link('data', cselm, 'mux')

	rc = cselm:handshake()
	print('handshake done.')

	rc = cselm:set_chunk_size(1400) -- should be less than MTU
	rc = cselm:set_ack_size(1024)
	--rc = cselm:set_peer_bandwidth(1024, rtmpmod.bandwidth.hard) -- soft, dynamic
	print('set_chunk_size done.')

	local rtmpelm = rtmp.rtmp.new()
	rc = cselm:link('message', rtmpelm, 'message')
	assert(0 == rc)

	rc = rtmpelm:set_chunk_size(cselm:chunk_size())

	local connectobj = {
		'connect',
		1, -- Transaction ID
		-- Command Object
		{
			app = 'live',
			flashVer = 'JET/0.1',
			--flashVer = 'FMLE/3.0 (compatible; FMSc/1.0)',
			swfUrl = '',
			tcUrl = 'rtmp://live.hkstv.hk.lxdns.com/live',
			type = 'nonprivate',
			fpad = false,
			audioCodecs = 1,
			videoCodecs = 1,
			pageUrl = '',
			objectEncoding = 1,
		},
		--rtmpmod.amf.undefined,
	}
	--tprint(connectobj)
	rc = rtmpelm:call(0, connectobj[2], connectobj) -- TODO connectobj必须是最后一个参数
	print('connect done.')
	tprint(rc)
	return true
end

function _M:listen(addr, port, backlog)
end

function _M:set_chunk_size(cs)
end

function _M:set_ack_size(as)
end



return _M