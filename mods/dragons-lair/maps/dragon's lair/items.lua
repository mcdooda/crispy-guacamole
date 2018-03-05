
local function add(value, add)
	return value + add
end

local function multiply(value, add)
	return value * add
end

return {
    {
        name = 'Iron ring',
        cost = 20,
        effect = add,
        factor = 1,
        description = 'Add one damage'
    },
    {
        name = 'Gold ring',
        cost = 60,
        effect = add,
        factor = 2,
        description = 'Add two damage'
    },
    {
        name = 'Enhanced ring',
        cost = 200,
        effect = add,
        factor = 5,
        description = 'Add five damage'
    },
    {
        name = 'Magic ring',
        cost = 200,
        effect = multiply,
        factor = 1.2,
        description = 'Multiply your damages by 1.2'
    }
}