#include "UIImage.h"
#include "ResourceManager.h"

UIImage::UIImage()
{
	model = ResourceManager::Get<Model>(g_defaultPlanePath);
	texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
}

UIImage::~UIImage()
{
}

void UIImage::Draw()
{
	model->Draw(texture);
}
