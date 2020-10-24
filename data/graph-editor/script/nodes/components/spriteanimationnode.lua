local NodeEditor = flat.require 'graph-editor/nodeeditor'
local ScriptRuntime = flat.require 'graph/script/scriptruntime'
local SpriteComponentNode = require 'data/graph/script/nodes/components/spritecomponentnode'
local Preview = require 'data/scripts/preview'

local SpriteAnimationNode = NodeEditor:inherit()

function SpriteAnimationNode:build(spriteAnimationNode, nodeWidget, pinsWidget)
    local previewContainer = Widget.makeColumnFlow()
    pinsWidget:addChild(previewContainer)
    nodeWidget.previewContainer = previewContainer
    return true
end

local function getSpriteComponentNode(script)
    local nodes = script:getNodes()
    for i = 1, #nodes do
        local node = nodes[i]
        if getmetatable(node) == SpriteComponentNode then
            return node
        end
    end
end

function SpriteAnimationNode:update(spriteAnimationNode, nodeWidget, pinsWidget)
    local mainWindow = nodeWidget.mainWindow
    local spriteAnimationNodeIndex = assert(mainWindow:getCurrentGraph():findNodeIndex(spriteAnimationNode))
    nodeWidget.previewContainer:removeAllChildren()

    -- do not crash if an error occured while running the graph
    local ok, err = pcall(function()
        -- we must use a clone of the graph as we need to resolve the compounds
        local scriptClone = mainWindow:getCurrentGraph():clone()
        scriptClone:resolveCompoundsAndReroutes()
        local scriptRuntime = ScriptRuntime:new(scriptClone)
        local spriteComponentNode = getSpriteComponentNode(scriptClone)
        if spriteComponentNode then
            local spriteComponentNodeRuntime = scriptRuntime:getNodeRuntime(spriteComponentNode)
            local spriteComponentTemplate = spriteComponentNodeRuntime:tryReadFromOutputPin(spriteComponentNode.componentOutPin)
            if spriteComponentTemplate then
                -- this can be inaccurate when used with compounds
                -- the ideal solution would be to look for the clone node by animation name
                local spriteAnimationNodeClone = scriptClone.nodeInstances[spriteAnimationNodeIndex]
                local spriteAnimationNodeRuntime = scriptRuntime:getNodeRuntime(spriteAnimationNodeClone)
                local spriteAnimationName = spriteAnimationNodeRuntime:readOptionalPin(spriteAnimationNodeClone.nameInPin)
                if spriteAnimationName then
                    local entityTemplatePath = mainWindow.metadata.entityTemplatePath
                    local animationPreview = Preview.sprite(entityTemplatePath, spriteComponentTemplate, spriteAnimationName, true)
                    -- we did it!
                    nodeWidget.previewContainer:addChild(animationPreview)
                end
            end
        end
    end)
    return true
end

return SpriteAnimationNode