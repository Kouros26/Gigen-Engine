local Player = 
 { 
    camera = nil,
    transform = nil,
    rigidBody = nil,
    moveSpeed = 10,
    sensitivity = 0.1,
 } 
 
 function Player:Awake() 

    -- self.camera = GameObjectManager.CreateCamera()
    -- self.owner:AddChild(self.camera)
    -- self.camera:SetPosition(0, 10, 0)

    self.transform = self.owner:GetTransform()
    self.transform:SetPosition(0, 10, 0)
    self.transform:SetRotation(0, 0, 0)
    self.transform:SetScale(1, 1, 1)
    
    self.owner:CreateCapsuleRigidBody(1, 1, Vector3.new(1), 1)
    self.rigidBody = self.owner:GetRigidBody()
    Physics.Delegate("OnCollisionEnter")
    Physics.Delegate("OnCollisionExit")

 end

 
 function Player:Start() 
    if (self.transform == nil) then
        self.transform = self.owner:GetTransform()
    end

    if (self.rigidBody == nil) then
        self.rigidBody = self.owner:GetRigidBody()
    end

 end 
 
 function Player:Update(deltaTime) 
    Move(deltaTime)
    Look(deltaTime)
 end
 

 function Look(deltaTime)
    local mouseDelta = Input.GetMousePos()
    local rotation = self.transform:GetRotation()
    rotation.y = rotation.y + mouseDelta.x * self.sensitivity
    rotation.x = rotation.x + mouseDelta.y * self.sensitivity
    self.transform:SetRotation(rotation)
 end


 function Move(deltaTime)
    local moveDir = Vector3.new(0, 0, 0)
    if Input.GetKey(KeyCode.W) then
        moveDir.z = 1
    elseif Input.GetKey(KeyCode.S) then
        moveDir.z = -1
    end
    if Input.GetKey(KeyCode.A) then
        moveDir.x = -1
    elseif Input.GetKey(KeyCode.D) then
        moveDir.x = 1
    end
    moveDir = moveDir:Normalize()
    moveDir = moveDir * self.moveSpeed * deltaTime
    self.transform:AddPosition(moveDir)
end

function Player:OnCollisionEnter(otherActor)
    Debug.Log("OnCollisionEnter")
end

function Player:OnCollisionExit(otherActor)
    Debug.Log("OnCollisionExit")
end
 
 return Player