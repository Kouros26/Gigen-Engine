#include "UIImage.h"
#include "ResourceManager.h"

UIImage::UIImage() : UIElement("Image")
{
	model = ResourceManager::Get<Model>(g_defaultPlanePath);
	texture = ResourceManager::Get<Texture>(g_defaultTexturePath);
}

UIImage::~UIImage()
{
}

void UIImage::SetTexture(const std::string& path)
{
	texture = ResourceManager::Get<Texture>(path);
}

void UIImage::Draw()
{
	model->Draw(texture);
}