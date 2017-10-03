--[[
return {
	origin = flat.Vector2(10.5, 17),
	size = flat.Vector2(10, 4),
	animations = {
		spawn = {
			line = 1,
			numFrames = 10,
			frameDuration = 0.12
		},
		despawn = {
			line = 4,
			numFrames = 6,
			frameDuration = 0.12
		},
		move = {
			line = 2,
			numFrames = 4,
			frameDuration = 0.12
		},
		attack = {
			line = 3,
			numFrames = 6,
			frameDuration = 0.12
		}
	}
}
]]

return flat.graph.script.run 'mods/crispy-guacamole/entities/zombie/sprite'
