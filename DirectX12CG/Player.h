#pragma once
#include "Object3d.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>
namespace MCB
{
	class Player
	{

		Input* input = Input::GetInstance();

		float speed = 1.5f;
		void Move();
		Texture bulletTex;
		std::list<std::unique_ptr<PlayerBullet>>bullets;
	public:
		Object3d obj;
		void Initialize(Model* model);
		void Update();
		void MatUpdate(View view,Projection proj);
		void Draw();
		void Rotate();
		void Attack();
	};

}

