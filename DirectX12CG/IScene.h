#pragma once
namespace MCB
{
	class IScene
	{
	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void LoadModel() = 0;
		virtual void LoadTexture() = 0;
		virtual void LoadSound() = 0;
		virtual void Object3DInit() = 0;
		virtual void SpriteInit() = 0;
		virtual void DrawSprite() = 0;
		virtual void Object3DDraw() = 0;
	};

}

