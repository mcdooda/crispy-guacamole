local Component = assert(Component)

local function sortedByName()
    local componentsSorted = {}
    for componentName, componentFlag in pairs(Component) do
        componentsSorted[#componentsSorted + 1] = componentName
    end
    table.sort(componentsSorted)
    return componentsSorted
end

return {
    sortedByName = sortedByName
}