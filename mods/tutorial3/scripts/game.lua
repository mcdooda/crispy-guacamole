local MrMoustache = require 'data/game/scripts/mrmoustache'
local DefenseTower = unpack(require 'data/game/scripts/defensetower')
local HarvesterTower = require 'data/game/scripts/harvestertower'
local TumorCreep = unpack(require 'data/game/scripts/tumorcreep')
local Money = require 'data/game/scripts/money'
local Windows = require 'data/game/scripts/windows'
local buildings = require 'data/game/scripts/buildings'

DefenseTower.upgrades = {
	DefenseTower.upgrades[1]
}
HarvesterTower.description = 'Extrait automatiquement l\'énergie de la poussière de météorite.'

dofile 'data/game/scripts/setup.lua'
dofile 'data/game/scripts/pausemenu.lua'
dofile 'data/game/scripts/interface.lua'
dofile 'data/game/scripts/rules.lua'

function checkVictory()
	if TumorCreep.allStoped() then
		Windows.victory('mods/tutorial4')
	end
end

function checkDefeat()
	
end

local creep = TumorCreep:new(2, 8, 0.6, 5)

MrMoustache.speech({
	
	'Général MacTyer au rapport...',
	'Les creeptologues ont analysé la poussière répandue par la météorite pendant l\'entrée dans l\'atmosphère.',
	'Celle-ci commence à se déposer au sol et serait inoffensive.',
	'Des fabricants de carburants ont trouvé de quoi l\'exploiter à très longue durée.',
	'Nous pourrions leur en fournir pour une forte somme en les récoltants grâce à un extracteur.',
	'Des plus gros morceaux tombent parfois, les troupes envoyées sur place s\'en chargeront.',
	'A vous de jouer !'
	
}, function()
	dofile 'data/game/scripts/loot.lua'
	Money.setAmount(600)
	creep:start()
end)
