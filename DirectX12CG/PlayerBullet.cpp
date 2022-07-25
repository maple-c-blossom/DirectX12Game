#include "PlayerBullet.h"

void MCB::PlayerBullet::Initialize(Model* model, DirectX::XMFLOAT3 position)
{
	if (model == nullptr) return;
	this->model = model;
	this->position = position;
	tex.CreateNoTextureFileIsTexture();
}

void MCB::PlayerBullet::Update()
{

}

void MCB::PlayerBullet::Draw()
{
	Object3d::Draw(tex.incrementNum);
}
