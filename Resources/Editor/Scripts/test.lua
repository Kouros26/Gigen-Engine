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

    
    rigidBody = self.owner:GetRigidBody()
    rigidBody:SetGravityEnabled(true)
    

    transform = self.owner:GetTransform()
    transform:SetPosition(Vector3.new(0, 4, 0))
    self.owner:SetModel("Engine/Models/MinecraftVillage.fbx")
    transform:SetScale(Vector3.new(0.1))
  
end


-- Update function
function test:Update(dt)

    transform = self.owner:GetTransform()
    move = Vector3.new()
    move.x = bool_to_number(Inputs.GetKey(Keys.J)) - bool_to_number(Inputs.GetKey(Keys.L))
    move.z = bool_to_number(Inputs.GetKey(Keys.I)) - bool_to_number(Inputs.GetKey(Keys.K))
    move = move * 10 * dt

    Debug.Log(tostring(move))

    if(move ~= Vector3.Zero()) then
        transform:SetPosition(transform:GetPosition() + move)
        transform:LookAt(move, Vector3.Up())
    end
end

-- Returns the usertype so the engine has a reference to it
return test