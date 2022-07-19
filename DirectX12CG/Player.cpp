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

	playerObj.position.x += move.vec.x * speed;
	playerObj.position.y += move.vec.y * speed;
	playerObj.position.z += move.vec.z * speed;

	//const Float3 moveLimit = {DxWindow::GetInstance()->window_width / 2 - 20.0f,DxWindow::GetInstance()->window_height / 2 - 20.0f, 0};
	//playerObj.position.x = max(playerObj.position.x,moveLimit.x);
	//playerObj.position.x = min(playerObj.position.x, -moveLimit.x);
	//playerObj.position.y = max(playerObj.position.y, moveLimit.y);
	//playerObj.position.y = min(playerObj.position.y, -moveLimit.x);


}

void MCB::Player::Initialize(Model* model)
{
	playerObj.Init();
	playerObj.model = model;
	playerObj.scale = { 3,3,3 };
}

void MCB::Player::Update()
{
	Move();
}

void MCB::Player::Draw()
{
	playerObj.Draw();
}
