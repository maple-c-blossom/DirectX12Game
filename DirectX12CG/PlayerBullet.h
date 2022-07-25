#pragma once
#include "Object3d.h"


namespace MCB
{
	class PlayerBullet:public Object3d
	{
		Texture tex;
	public:
		void Initialize(Model* model,DirectX::XMFLOAT3 position);
		void Update();
		void Draw();

	};

}



