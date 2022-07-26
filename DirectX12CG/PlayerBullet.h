#pragma once
#include "Object3d.h"


namespace MCB
{
	class PlayerBullet
	{
		Texture* tex;
	public:
		Object3d obj;
		void Initialize(Model* model,Texture* tex,Float3 position);
		void Update();
		void Draw();

	};

}



