-- Holds data that are shared between functions of this usertype
local test =
{
    elapsedTime = 0,
    -- array of game objects
    gameObjects = {}
}

function bool_to_number(value)
    return value and 1 or 0
  end

function test:Start()

    Debug.Log(type("Ground")) -- should print "string"
    Debug.Log(type({ "Ground" })) -- should print "table"
    


    rigidBody = self.owner:GetRigidBody()
    rigidBody:SetGravityEnabled(false)
    
    transform = self.owner:GetTransform()
    transform:SetPosition(Vector3.new(0, 4, 0))
    self.owner:SetModel("Engine/Models/MinecraftVillage.fbx")

    transform:SetScale(Vector3.new(0.1))
  
    for i = 1, 100 do
        local obj = GameObjectManager.Instantiate(Vector3.new(0, 0, 0), Vector3.new(0), Vector3.new(0.1))
        obj:SetModel("Engine/Models/MinecraftVillage.fbx")
        obj:SetTexture("Engine/Textures/Icon.png")
        obj:GetTransform():SetPosition(Vector3.new(0, 0, i * 4))
        obj:GetTransform():SetScale(Vector3.new(0.1))
        self.gameObjects[i] = obj
    end

end


-- Update function
function test:Update(dt)

    self.elapsedTime = self.elapsedTime + dt
    if (self.elapsedTime > 10) then
       for i = 1, 100 do
          self.gameObjects[i]:Destroy()
          
        end
    end
  
    transform = self.owner:GetTransform()
    move = Vector3.new()
    move.x = bool_to_number(Inputs.GetKey(Keys.J)) - bool_to_number(Inputs.GetKey(Keys.L))
    move.z = bool_to_number(Inputs.GetKey(Keys.I)) - bool_to_number(Inputs.GetKey(Keys.K))
    move = move * 10 * dt



    if(move ~= Vector3.Zero()) then
        transform:SetPosition(transform:GetPosition() + move)
        transform:LookAt(move, Vector3.Up())
    end
end

-- Returns the usertype so the engine has a reference to it
return test