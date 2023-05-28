local Player = 
 { 
    camera = nil,
    transform = nil,
    rigidBody = nil,
    moveSpeed = 10,
    sensitivity = 0.5,
 } 
 
 function Player:Awake() 

    -- self.camera = GameObjectManager.CreateCamera()
    -- self.owner:AddChild(self.camera)
    -- self.camera:SetPosition(0, 10, 0)

    self.transform = self.owner:GetTransform()
    self.transform:SetPosition(Vector3.new(0, 10, 0))
    self.transform:SetRotation(Vector3.new(0))
    self.transform:SetScale(Vector3.new(1))
    
     self.owner:CreateCapsuleRigidBody(1, 1, Vector3.new(1), 1)
    --self.owner:CreateBoxRigidBody(Vector3.new(0.5),Vector3.new(1), 1)
    self.rigidBody = self.owner:GetRigidBody()
    self.rigidBody:SetAngularFactor(Vector3.new(0,1,0))
end


function Player:Start() 
    Physics.Delegate("OnCollisionEnter")
    Physics.Delegate("OnCollisionExit")
    if (self.transform == nil) then
        self.transform = self.owner:GetTransform()
    end

    if (self.rigidBody == nil) then
        self.rigidBody = self.owner:GetRigidBody()
    end

 end 
 
 function Player:Update(deltaTime) 
    Move(deltaTime, self.moveSpeed, self.transform)
    Look(deltaTime, self.sensitivity, self.transform)
 end
 

 function Look(deltaTime, sensitivity, transform)
    local mouseDelta = Inputs.GetMousePos()
    local rotation = transform:GetRotation()
    if (mouseDelta.x == 0 and mouseDelta.y == 0 ) then
        return
    end
    if(mouseDelta.x > 0) then
        mouseDelta.x = 1
    elseif(mouseDelta.x < 0) then
        mouseDelta.x = -1
    end

    if(mouseDelta.y > 0) then
        mouseDelta.y = 1
    elseif(mouseDelta.y < 0) then
        mouseDelta.y = -1
    end
    rotation.x = rotation.x + mouseDelta.y * sensitivity
    rotation.y = rotation.y + mouseDelta.x * sensitivity
    transform:SetRotation(rotation)
    Debug.Log("rotation: " .. tostring(rotation))
 end


 function Move(deltaTime, moveSpeed, transform)
    local moveDir = Vector3.new(0)
    if Inputs.GetKey(Keys.W) then
        moveDir.z = 1
    elseif Inputs.GetKey(Keys.S) then
        moveDir.z = -1
    end
    if Inputs.GetKey(Keys.A) then
        moveDir.x = -1
    elseif Inputs.GetKey(Keys.D) then
        moveDir.x = 1
    end
    moveDir = moveDir:Normalize()
    moveDir = moveDir * moveSpeed * deltaTime

    local position = transform:GetPosition()
    position = position + moveDir

    transform:SetPosition(position)

    Debug.Log("moveDir: " .. tostring(moveDir))
end

function Player:OnCollisionEnter(otherActor)
    Debug.Log("OnCollisionEnter")
end

function Player:OnCollisionExit(otherActor)
    Debug.Log("OnCollisionExit")
end
 
 return Player