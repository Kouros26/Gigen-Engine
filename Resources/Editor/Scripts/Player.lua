local Player = 
 { 
    camera = nil,
    transform = nil,
    rigidBody = nil,
    moveSpeed = 10,
    sensitivity = 60,
    jumpForce = 10
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
        self.rigidBody:SetAngularFactor(Vector3.new(0,0,0))
    end
    
    if (self.camera == nil) then
        self.camera = GameObjectManager.CreateCamera()
        self.owner:AddChild(self.camera)
        self.camera:GetTransform():SetPosition(self.transform:GetPosition())
    end


 end 
 
 function Player:Update(deltaTime) 
    Move(deltaTime, self.moveSpeed, self.camera:GetTransform())
   
    Look(deltaTime, self.sensitivity, self.camera:GetTransform())

    Jump(deltaTime, self.transform, self.jumpForce)


    local position = self.camera:GetTransform():GetPosition()
    self.transform:SetPosition(position)

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
    rotation.x = rotation.x - mouseDelta.y * sensitivity * deltaTime


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

function Jump(deltaTime, transform, jumpForce)
    local jumpDir = Vector3.new(0)
    if Inputs.GetKey(Keys.Space) then
        jumpDir = jumpDir + transform:GetUp()
    end
    jumpDir = jumpDir:Normalize()
    jumpDir = jumpDir * jumpForce * deltaTime

    local position = transform:GetPosition()
    position = position + jumpDir

    transform:SetPosition(position)
    Debug.Log("jumpDir: " .. tostring(jumpDir))
    
end








function Player:OnCollisionEnter(otherActor)
    Debug.Log("OnCollisionEnter")
end

function Player:OnCollisionExit(otherActor)
    Debug.Log("OnCollisionExit")
end
 
 return Player