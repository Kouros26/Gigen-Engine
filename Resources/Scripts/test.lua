-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
}

-- Update function
function test:Update(dt)

    self.elapsedTime = self.elapsedTime + dt

    rigidBody = self.owner:GetRigidBody()

    if (self.elapsedTime > 1) then
        rigidBody:AddTorque(Vector3.new(0, 1000,0))
        self.elapsedTime = 0
    end
end

-- Returns the usertype so the engine has a reference to it
return test