function titlize(title)
	local eq = ''
	for i = 1, #title + 4 do
		eq = eq .. '='
	end
	
	print(eq)
	print('= ' .. title .. ' =')
	print(eq)
	print ''
end
