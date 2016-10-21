local function dump(t, i)
	i = i or ''
	if type(t) == 'table' then
		for k, v in pairs(t) do
			if type(v) == 'table' then
				print(i, k)
				dump(v, i..'\t')
			else
				print(i, k, v)
			end
		end
	else
		print(t)
	end
end

return dump
