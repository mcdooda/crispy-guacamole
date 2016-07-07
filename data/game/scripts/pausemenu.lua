local Menu = require 'data/scripts/menu'
local Game = Game

local menu
Input.onKeyDown(Input.ESCAPE, function()
	if not Game.isPaused() then
		Game.pause()
		menu = Menu:new('Pause',
			'Reprendre', Game.resume,
			'Recommencer', Game.reload,
			'Retour a l\'accueil', function() Game.load('mods/menu') end,
			'Quitter', Game.exit
		)
	else
		Game.resume()
		menu:destroy()
	end
end)
