#pragma once
#include "Object3d.h"
#include "Input.h"
namespace MCB
{
	class Player
	{

		Input* input = Input::GetInstance();

		float speed = 1.5f;
		void Move();
	public:
		Object3d playerObj;
		void Initialize(Model* model);
		void Update();
		void Draw();
	};

}

