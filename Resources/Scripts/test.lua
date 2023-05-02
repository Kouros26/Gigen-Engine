-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
}

-- Update function
function test:Update(dt)
    Debug.Log(tostring(self.elapsedTime))

    self.elapsedTime = self.elapsedTime + dt

    transform = self.owner:GetTransform()

    transform:SetPosition(Vector3.new(0, math.sin(self.elapsedTime), 0))
end

-- Returns the usertype so the engine has a reference to it
return test