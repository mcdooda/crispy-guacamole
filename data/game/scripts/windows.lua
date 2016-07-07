local Menu = require 'data/scripts/menu'

local Windows = {}

function Windows.victory(nextMod)
	Game.over()
	local menu = Menu:new('La mission est un succès : la menace a été endiguée.',
		'Mission suivante', function() Game.load(nextMod) end,
		'Recommencer', Game.reload
	)
	local w, h = Widget.getDimensions(menu.widget)
	local w1, h1 = Widget.getDimensions(menu.textWidget)
	menu:setDimensions{ width = w1 + 20, height = h }
end

function Windows.defeat(message)
	Game.over()
	local menu = Menu:new('La mission a échoué : '..message..'.',
		'Recommencer', Game.reload,
		'Quitter', Game.exit
	)
	local w, h = Widget.getDimensions(menu.widget)
	local w1, h1 = Widget.getDimensions(menu.textWidget)
	menu:setDimensions{ width = w1 + 20, height = h }
end

return Windows
