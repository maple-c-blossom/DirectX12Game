#include "PlayerBullet.h"

void MCB::PlayerBullet::Initialize(Model* model, Texture* tex, Float3 position)
{
	if (model == nullptr) return;
	obj.Init();
	obj.model = model;
	obj.position.x = position.x, obj.position.y = position.y, obj.position.z = position.z;
	this->tex = tex;
}

void MCB::PlayerBullet::Update()
{

}

void MCB::PlayerBullet::Draw()
{
	obj.Draw(tex->incrementNum);
}
