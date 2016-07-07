local MrMoustache = require 'data/game/scripts/mrmoustache'
local HarvesterTower = require 'data/game/scripts/harvestertower'
local DefenseTower = unpack(require 'data/game/scripts/defensetower')
local TumorCreep = unpack(require 'data/game/scripts/tumorcreep')
local Money = require 'data/game/scripts/money'
local Windows = require 'data/game/scripts/windows'
local buildings = require 'data/game/scripts/buildings'

HarvesterTower.cost = -1
DefenseTower.description = 'Empêche la propagation du virus.'
DefenseTower.upgrades = {
	DefenseTower.upgrades[1]
}

dofile 'data/game/scripts/setup.lua'
dofile 'data/game/scripts/pausemenu.lua'
dofile 'data/game/scripts/interface.lua'
dofile 'data/game/scripts/rules.lua'

function checkVictory()
	if TumorCreep.allStoped() then
		Windows.victory('mods/tutorial3')
	end
end

function checkDefeat()
	
end

local creep = TumorCreep:new(6, 6, 0.6, 5)

MrMoustache.speech({
	
	'Général MacTyer au rapport...',
	'Nos scientifiques ont découvert un moyen de détecter là où le fléau va agir.',
	'Nous avons déjà installé le réseau d\'énergie en prévision mais la propagation va commencer sous peu.',
	'Votre mission sera d\'installer les tours de défense radio pour empêcher la menace d\'atteindre le QG.',
	'A vous de jouer !'
	
}, function()
	Money.setAmount(3000)
	creep:start()
end)
