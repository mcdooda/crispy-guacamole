local Money = require 'mods/crispy-guacamole/maps/td/money'

return {
    {
        template = 'tower',
        name = 'Tower',
        description = 'Throws arrows at enemies',
        cost = 1000
    },
    {
        template = 'sleeping_dragon',
        name = 'Sleeping Dragon',
        description = 'Increases income',
        cost = 5000,
        onPlace = function()
            Money:addIncome(100)
        end
    }
}