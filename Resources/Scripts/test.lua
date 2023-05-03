-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
}

-- Update function
function test:Update(dt)
    Debug.Log(tostring(self.elapsedTime))

    self.elapsedTime = self.elapsedTime + dt

    Debug.Log(tostring(self.owner))
    transform = self.owner:GetTransform()

    transform:SetPosition(Vector3.new(transform:GetPosition().x, math.sin(self.elapsedTime), transform:GetPosition().z))
end

-- Returns the usertype so the engine has a reference to it
return test