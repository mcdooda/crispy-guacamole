local function splitLine(line, max)
	local words = {}
	for word in line:gmatch('[^ ]+') do
		words[#words + 1] = word
	end
	local lines = {}
	local numWords = #words
	local i = 1
	while i <= numWords do
		local j = i
		local n = 0
		while n < max and i <= numWords do
			n = n + #words[i]
			i = i + 1
		end
		if n > max then
			i = i - 1
		end
		if i > numWords then
			i = numWords
		end
		lines[#lines + 1] = table.concat(words, ' ', j, i)
		i = i + 1
	end
	return lines
end

return splitLine
