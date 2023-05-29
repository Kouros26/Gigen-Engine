local Player = 
 { 
    camera = nil,
    transform = nil,
    rigidBody = nil,
    moveSpeed = 10,
    sensitivity = 10,
 } 
 
 function Player:Awake() 
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
        self.rigidBody:SetAngularFactor(Vector3.new(0,1,0))
    end

    if (self.camera == nil) then
        self.camera = GameObjectManager.CreateCamera()
        self.owner:AddChild(self.camera)
        self.camera:GetTransform():SetPosition(Vector3.new(0, 0, 0))
    end

 end 
 
 function Player:Update(deltaTime) 
    Move(deltaTime, self.moveSpeed, self.transform)
   
    Look(deltaTime, self.sensitivity, self.camera:GetTransform())

    local rotation = self.transform:GetRotation()
    rotation.y = self.camera:GetTransform():GetRotation().y
    self.transform:SetRotation(rotation)

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
    
    rotation.y = rotation.y + mouseDelta.x * sensitivity * deltaTime
    rotation.x = rotation.x + mouseDelta.y * sensitivity * deltaTime


    transform:SetRotation(rotation)
    Debug.Log("rotation: " .. tostring(rotation))
 end


 function Move(deltaTime, moveSpeed, transform)
    local moveDir = Vector3.new(0)
    if Inputs.GetKey(Keys.W) then
        moveDir = moveDir + transform:GetFront()
    elseif Inputs.GetKey(Keys.S) then
        moveDir = moveDir - transform:GetFront()
    end
    if Inputs.GetKey(Keys.A) then
        moveDir = moveDir + transform:GetRight()
    elseif Inputs.GetKey(Keys.D) then
        moveDir = moveDir - transform:GetRight()
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