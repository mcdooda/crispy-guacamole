return {
    -- attack
    'components/attackcomponent',
    'entity/getattacktarget',

    -- behavior
    'components/behaviorcomponent',

    -- collision
    'components/collisioncomponent',

    -- detection
    'components/detectioncomponent',

    -- faction
    'components/factioncomponent',

    -- interaction
    'components/interactioncomponent',

    -- life
    'components/lifecomponent',
    'entity/dealdamage',

    -- movement
    'components/movementcomponent',
    'entity/clearpath',
    'entity/setmoveanimation',
    'entity/setspeed',
    'entity/getspeed',
    'entity/moveto',
    'entity/setstrafing',
    'entity/getstrafing',

    -- player controller
    'components/playercontrollercomponent',

    -- projectile
    'components/projectilecomponent',
    'entity/fireprojectile',
    'entity/setprojectilespeed',
    'entity/getprojectilespeed',

    -- prop
    'components/propcomponent',

    -- selection
    'components/selectioncomponent',

    -- sprite
    'components/spriteanimation',
    'components/spriteattachpoint',
    'components/spritecomponent',
    'entity/playanimation',
    'entity/flipspritex',

    -- entity
    'entity/gettemplatename',

    'entity/spawnentity',
    'entity/despawnentity',

    'entity/lookatentity',
    'entity/jump',

    'entity/getheading',
    'entity/setheading',
    'entity/headingchanged',
    'entity/getelevation',
    'entity/setelevation',
    'entity/getposition',
    'entity/setposition',

    'entity/isvalidentity',
    'entity/iscomponentenabled',

    -- flow
    'flow/eachframe',
    'flow/cooldown',

    -- game
    'game/gamedt',
    'game/gametime',

    -- controller
    'controller/buttonpressed',
    'controller/buttonjustpressed',
    'controller/leftstickdirection',
    'controller/rightstickdirection',
    'controller/axisvalue',
}