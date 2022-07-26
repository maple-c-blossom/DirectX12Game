#include "Player.h"
#define IN input->

using namespace MCB;
using namespace DirectX;
using namespace std;
void MCB::Player::Move()
{
	Vector3D move = { 0,0,0 };
	if (input->IsKeyDown(DIK_W))
	{
		move.vec.y += 1;
	}

	if (input->IsKeyDown(DIK_S))
	{
		move.vec.y -= 1;
	}

	if (input->IsKeyDown(DIK_D))
	{
		move.vec.x += 1;
	}

	if (input->IsKeyDown(DIK_A))
	{
		move.vec.x -= 1;
	}

	move.V3Norm();

	obj.position.x += move.vec.x * speed;
	obj.position.y += move.vec.y * speed;
	obj.position.z += move.vec.z * speed;

	const Float3 moveLimit = {64.0f,34.0f, 0};
	obj.position.x = max(obj.position.x, -moveLimit.x);
	obj.position.x = min(obj.position.x, moveLimit.x);
	obj.position.y = max(obj.position.y, -moveLimit.y);
	obj.position.y = min(obj.position.y, moveLimit.y);


}

void MCB::Player::Initialize(Model* model)
{
	obj.Init();
	obj.model = model;
	obj.scale = { 3,3,3 };
	bulletTex.CreateNoTextureFileIsTexture();
}

void MCB::Player::Update()
{
	Move();
	Rotate();
	Attack();
	for (unique_ptr<PlayerBullet>& bullet : bullets) bullet->Update();
	
}

void MCB::Player::MatUpdate(View view, Projection proj)
{
	obj.MatrixUpdata(view, proj);
	for (unique_ptr<PlayerBullet>& bullet : bullets) bullet.get()->obj.MatrixUpdata(view, proj);
}

void MCB::Player::Draw()
{
	obj.Draw();
	for (unique_ptr<PlayerBullet>& bullet : bullets) bullet->Draw();
}

void MCB::Player::Rotate()
{
	if (input->IsKeyDown(DIK_Q))
	{
		obj.rotasion.y -= 0.04f;
	}

	if (input->IsKeyDown(DIK_E))
	{
		obj.rotasion.y += 0.04f;
	}
}

void MCB::Player::Attack()
{
	if (IN IsKeyTrigger(DIK_SPACE))
	{
		Float3 Bposition;
		Bposition.x = obj.position.x, Bposition.y = obj.position.y, Bposition.z = obj.position.z;
		std::unique_ptr<PlayerBullet> bullet = make_unique<PlayerBullet>();
		bullet->Initialize(obj.model, &bulletTex, Bposition);
		bullets.push_back(move(bullet));

	}
}
