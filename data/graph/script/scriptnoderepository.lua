return {
    -- attack
    'components/attackcomponent',
    'entity/getattacktarget',

    -- behavior
    'components/behaviorcomponent',
    'entity/enterstate',

    -- collision
    'components/collisioncomponent',

    -- detection
    'components/detectioncomponent',

    -- faction
    'components/factioncomponent',

    -- fog vision
    'components/fogvisioncomponent',

    -- audio
    'components/samplecomponent',
    'entity/playsample',
    'entity/playsound',
    'entity/playbark',

    -- interaction
    'components/interactioncomponent',

    -- life
    'components/lifecomponent',
    'entity/dealdamage',

    -- movement
    'components/movementcomponent',
    'entity/clearpath',
    'entity/setspeed',
    'entity/getspeed',
    'entity/moveto',
    'entity/setstrafing',
    'entity/isstrafing',
    'entity/setmidairacceleration',
    'entity/getmidairacceleration',
    'entity/isfollowingpath',
    'entity/ismidair',
    'entity/movementstarted',
    'entity/movementstopped',

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
    'entity/setcycleanimation',
    'entity/clearcycleanimation',
    'entity/setcycleanimated',
    'entity/resetcycleanimation',
    'entity/playanimation',
    'entity/flipspritex',
    'entity/setspriterotation',
    'entity/directionalsprite',
    'entity/getattachpoint',

    -- entity
    'entity/gettemplatepath',

    'entity/spawnentity',
    'entity/despawnentity',

    'entity/lookatentity',
    'entity/jump',

    'entity/eachentityinrange',

    'entity/getheading',
    'entity/setheading',
    'entity/headingchanged',
    'entity/getelevation',
    'entity/setelevation',
    'entity/getposition',
    'entity/setposition',
    'entity/setposition2d',
    'entity/getforward',

    'entity/setinstigator',
    'entity/getinstigator',

    'entity/setextradatafield',
    'entity/getextradatafield',

    'entity/isvalidentity',
    'entity/iscomponentenabled',
    'entity/disablecomponent',

    -- gameplay
    'gameplay/makecircularwave',
    'gameplay/makelinearwave',

    -- flow
    'flow/eachframe',
    'flow/cooldown',

    -- game
    'game/gamedt',
    'game/gametime',
    'game/sleep',
    'game/timer',

    -- controller
    'controller/buttonpressed',
    'controller/buttonjustpressed',
    'controller/buttonjustreleased',
    'controller/leftstickdirection',
    'controller/rightstickdirection',
    'controller/axisvalue',

    -- map
    'map/settiletemplate',
    'map/navigationraycast',
    'map/navigabilitymask',
}