#pragma once
#include "Object3d.h"
#include "Input.h"
namespace MCB
{
	class Player:public Object3d
	{

		Input* input = Input::GetInstance();

		float speed = 1.5f;
		void Move();
	public:
		void Initialize(Model* model);
		void Update();
		void MatUpdate(View view,Projection proj);
		void Draw();
		void Rotate();
	};

}

