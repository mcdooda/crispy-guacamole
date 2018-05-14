local Path = require 'data/scripts/path'

local cocreate = coroutine.create
local coresume = coroutine.resume
local costatus = coroutine.status
local coyield = coroutine.yield

local getTime = game.getTime

local LevelTasks = {
    tasks = {}
}

function LevelTasks:executeTasks()
    local tasks = self.tasks
    local taskCount = #tasks
    for i = taskCount, 1, -1 do
        local task = tasks[i]
        assert(coresume(task))
        if costatus(task) == 'dead' then
            tasks[i] = tasks[taskCount]
            tasks[taskCount] = nil
            taskCount = taskCount - 1
        end
    end
end

function LevelTasks:loop()
    while true do
        self:executeTasks()
        coyield()
    end
end

function LevelTasks:addTask(func)
    local tasks = self.tasks
    local task = cocreate(func)
    tasks[#tasks + 1] = task
    return task
end

function LevelTasks:removeTask(task)
    local tasks = self.tasks
    for i = 1, #tasks do
        if tasks[i] == task then
            tasks[i] = tasks[#tasks]
            tasks[#tasks] = nil
            return true
        end
    end
    return false
end

function LevelTasks:run(...)
    for i = 1, select('#', ...) do
        local file = select(i, ...)
        local task = assert(loadfile(Path.getMapFilePath(file .. '.lua')))
        self:addTask(task)
    end
    self:loop()
end

function LevelTasks:sleep(duration)
    local endTime = getTime() + duration
    while getTime() < endTime do
        coyield()
    end
end

return LevelTasks