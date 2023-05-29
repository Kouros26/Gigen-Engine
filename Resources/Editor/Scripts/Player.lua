local Player = 
 { 
    camera = nil,
    transform = nil,
    rigidBody = nil,
    moveSpeed = 10,
    sensitivity = 100,
    jumpForce = 10,
    isGrounded = false,
    jumpTimer = 0,
 } 
 
 function Player:Awake() 
    self.transform = self.owner:GetTransform()
    self.transform:SetPosition(Vector3.new(0, 10, 0))
    self.transform:SetRotation(Vector3.new(0))
    self.transform:SetScale(Vector3.new(1))
        
     self.owner:CreateSphereRigidBody(5, Vector3.new(1), 1)
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
        self.camera:GetTransform():SetPosition(Vector3.new(0, 0, 0))
    end


 end 
 
 function Player:Update(deltaTime) 
    local hit = HitResult.new()
    Physics.RayCast(self.transform:GetPosition(), -self.transform:GetUp() * 10, hit)
    if(Vector3.Distance(self.transform:GetPosition(),hit.hitPoint) < 3) then
        self.isGrounded = true
    end


    Move(deltaTime, self.moveSpeed, self.transform, self.camera, hit.hitPoint, self.isGrounded)
   
    Look(deltaTime, self.sensitivity, self.camera:GetTransform())

    Jump(deltaTime, self.transform, self.jumpForce, self.isGrounded, self.jumpTimer)

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
 end

function CalculateFront(camera)
    local matrix = camera:GetTransform():GetMatrix()
   
    local inverse = Matrix4.Inverse(matrix)
    local front = Vector3.new(inverse[2].x, inverse[2].y, inverse[2].z)
    return front:Normalize()

end

function CalculateUp(camera)
    local matrix = camera:GetTransform():GetMatrix()
   
    local inverse = Matrix4.Inverse(matrix)
    local up = Vector3.new(inverse[1].x , inverse[1].y, inverse[1].z)
    return up:Normalize()
    
end

function CalculateRight(camera)
    local matrix = camera:GetTransform():GetMatrix()
   
    local inverse = Matrix4.Inverse(matrix)
    local right = Vector3.new(inverse[0].x, inverse[0].y, inverse[0].z)
    return right:Normalize()    
end



 function Move(deltaTime, moveSpeed, transform, camera, groundPos, isGrounded)
    


    local moveDir = Vector3.new(0)
    if Inputs.GetKey(Keys.W) then
        moveDir = moveDir + CalculateFront(camera)
    elseif Inputs.GetKey(Keys.S) then
        moveDir = moveDir - CalculateFront(camera)
    end
    if Inputs.GetKey(Keys.A) then
        moveDir = moveDir + CalculateRight(camera)
    elseif Inputs.GetKey(Keys.D) then
        moveDir = moveDir - CalculateRight(camera)
    end
    moveDir = moveDir:Normalize()
    moveDir = moveDir * moveSpeed * deltaTime

    local position = transform:GetPosition()
    position = position + moveDir

    if (groundPos ~= nil and isGrounded) then
        position.y = groundPos.y + 5
    end
    

    transform:SetPosition(position)

end

function Jump(deltaTime, transform, jumpForce, isGrounded, jumpTimer)
    if (isGrounded == false and jumpTimer > 2) then
        return
    end

    if (isGrounded == false) then
        jumpTimer = jumpTimer + deltaTime
    else
        jumpTimer = 0
    end
    
    local jumpDir = Vector3.new(0)
    if Inputs.GetKey(Keys.Space) then
        isGrounded = false
        jumpDir = jumpDir + transform:GetUp()
    end
    jumpDir = jumpDir:Normalize()
    jumpDir = jumpDir * jumpForce * deltaTime

    local position = transform:GetPosition()
    position = position + jumpDir

    transform:SetPosition(position)
    
end








function OnCollisionEnter(otherActor)
    Debug.Log("OnCollisionEnter")
end

function OnCollisionExit(otherActor)
    Debug.Log("OnCollisionExit")
end
 
 return Player