#include "Player.h"

using namespace MCB;
using namespace DirectX;
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

	position.x += move.vec.x * speed;
	position.y += move.vec.y * speed;
	position.z += move.vec.z * speed;

	const Float3 moveLimit = {64.0f,34.0f, 0};
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, moveLimit.y);


}

void MCB::Player::Initialize(Model* model)
{
	Init();
	model = model;
	scale = { 3,3,3 };
}

void MCB::Player::Update()
{
	Move();
	Rotate();
}

void MCB::Player::MatUpdate(View view, Projection proj)
{
	MatrixUpdata(view, proj);
}

void MCB::Player::Draw()
{
	Draw();
}

void MCB::Player::Rotate()
{
	if (input->IsKeyDown(DIK_Q))
	{
		rotasion.y -= 0.04f;
	}

	if (input->IsKeyDown(DIK_E))
	{
		rotasion.y += 0.04f;
	}
}
