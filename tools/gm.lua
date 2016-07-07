#!/usr/bin/env lua5.1

local commands = {
	newmap = 'create a new map'
}

local function runCommand(command)
	require 'tools/titlize'
	require 'tools/mkfile'
	
	print('Running ' .. command .. '...')
	dofile('tools/scripts/' .. command .. '.lua')
end

local command = ...

if not command or not commands[command] then
	if command then
		print('Unknown command ' .. command .. '!')
	end
	
	print 'Available commands:'
	for command, description in pairs(commands) do
		print('   ' .. command, description)
	end
	
	if not command then
		print 'Command:'
		command = io.read '*l'
		if command and commands[command] then
			runCommand(command)
		end
	end
else
	runCommand(command)
end
