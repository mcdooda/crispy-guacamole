local Effects = require 'mods/dragons-lair/maps/dragons-lair/effects'

return {
    {
        name = 'Iron ring',
        cost = 20,
        effect = Effects.add,
        factor = 1,
        description = 'Add one damage'
    },
    {
        name = 'Gold ring',
        cost = 60,
        effect = Effects.add,
        factor = 2,
        description = 'Add two damage'
    },
    {
        name = 'Enhanced ring',
        cost = 200,
        effect = Effects.add,
        factor = 5,
        description = 'Add five damage'
    },
    {
        name = 'Magic ring',
        cost = 200,
        effect = Effects.multiply,
        factor = 1.2,
        description = 'Multiply your damages by 1.2'
    }
}