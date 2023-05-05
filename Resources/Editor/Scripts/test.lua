-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,

}

function bool_to_number(value)
    return value and 1 or 0
  end

function test:Start()
    Debug.Log("Start")

    transform = self.owner:GetTransform()
    transform:SetPosition(Vector3.new(0, 3, 0))


    rigidBody = self.owner:GetRigidBody()
    
    rigidBody:SetGravityEnabled(true)

    rigidBody:SetMass(1)
end


-- Update function
function test:Update(dt)
    self.elapsedTime = self.elapsedTime + dt
    
    rigidBody = self.owner:GetRigidBody()
    move = Vector3.new()
    move.x = bool_to_number(Inputs.GetKey(Keys.J)) - bool_to_number(Inputs.GetKey(Keys.L))
    move.z = bool_to_number(Inputs.GetKey(Keys.I)) - bool_to_number(Inputs.GetKey(Keys.K))
    move = move * 1000 * dt


    if(move ~= Vector3.Zero) then
        rigidBody:AddTorque(Vector3.new(math.deg(move.x), 0, math.deg(move.z)))
        Debug.Log(tostring(move))
        rigidBody:SetLinearVelocity(move)
    end
end

-- Returns the usertype so the engine has a reference to it
return test