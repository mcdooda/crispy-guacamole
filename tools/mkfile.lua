function mkfile(name, content, ...)
	local f = io.open(name, 'w')
	f:write(string.format(content, ...))
	f:close()
	print('created file ' .. name)
end

function mkdir(name)
	os.execute('mkdir ' .. name)
	print('created directory ' .. name)
end
