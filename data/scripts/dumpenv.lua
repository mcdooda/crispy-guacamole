local dump = require 'data/scripts/dump'

local function dumpenv()
	for k, v in pairs(_G) do
		local c = k:sub(1, 1)
		if 'A' <= c and c <= 'Z' then
			print '====================='
			print(k)
			dump(v)
		end
	end
	print '====================='
end

return dumpenv
