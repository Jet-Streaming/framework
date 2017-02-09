--[[
Hello, and welcome to hacking Jet!

 This file is invoked by node::LoadEnvironment in src/node.cc, and is
 responsible for bootstrapping the node.js core. As special caution is given
 to the performance of the startup process, many dependencies are invoked
 lazily.
]]

--coroutine.yield()

print(package.path)
--package.path = package.path .. ';./node_modules/?.lua';
--print(package.path)

local function tprint (tbl, indent)
  if not indent then indent = 0 end
  for k, v in pairs(tbl) do
    formatting = string.rep("  ", indent) .. k .. " <-> "
    if type(v) == "table" then
      print(formatting)
      tprint(v, indent+1)
    else
      print(formatting .. v)
    end
  end
end

print('start Jet with interpreter:', _VERSION)

-- parse arguments
print('args {')
tprint(jet.args)
print('args }')

-- debug hook

-- init functions

-- test coroutine
--local co = coroutine.create(function() end)
--coroutine.resume(co)



-- spawn main thread
local thread = jet.thread
--assert(not jet.entry)
--jet.entry = 
thread.spawn(function (a, b, c)
    print(a,b,c)
        
    local file = jet.args[1]
    assert(file, 'startup Lua file expected')

        -- run a file from disk
		local f, err = loadfile(file)
    if not f then
      print(err)
      return
    end
    f()
		--print(file, "excuted")
        
        -- download a file and run
		--local code = http.get(file)
		--loadstring(code)()
		
        -- read from stdin and run
		--print('input some Lua code and Ctrl+Z to execute it:')
		--loadfile()()
        
	end, 2, 4, 6)
--assert("thread" == type(jet.entry))

--coroutine.resume(jet.entry)

