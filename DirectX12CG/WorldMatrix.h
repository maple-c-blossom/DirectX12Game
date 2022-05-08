#pragma once

#include <DirectXMath.h>
#include "View.h"

namespace MCB
{
	class WorldMatrix
	{
	public:
		//ワールド行列
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
		//スケール行列
		DirectX::XMMATRIX matScale = DirectX::XMMatrixIdentity();
		//回転行列
		DirectX::XMMATRIX matRot = DirectX::XMMatrixIdentity();
		//平行移動行列
		DirectX::XMMATRIX matTransform;





		/// <summary>
		/// X回転の設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotX(float angle,bool ConvertToRadians = true);
		/// <summary>
		/// Y回転設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotY(float angle, bool ConvertToRadians = true);
		/// <summary>
		/// Z回転の設定
		/// </summary>
		/// <param name="angle">角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRotZ(float angle, bool ConvertToRadians = true);

		/// <summary>
		/// 回転設定
		/// </summary>
		/// <param name="angleX">Xの角度</param>
		/// <param name="angleY">Yの角度</param>
		/// <param name="angleZ">Zの角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		void SetMatRot(float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

		/// <summary>
		/// 引数で与えられた角度に変換された回転行列を返します
		/// </summary>
		/// <param name="matRot">回転元の行列</param>
		/// <param name="angleX">Xの角度</param>
		/// <param name="angleY">Yの角度</param>
		/// <param name="angleZ">Zの角度</param>
		/// <param name="ConvertToRadians">弧度法に変換するフラグ、angleを弧度法で書いたら"false"を設定してください</param>
		/// <returns></returns>
		DirectX::XMMATRIX ReturnMatRot(DirectX::XMMATRIX matRot,float angleX, float angleY, float angleZ, bool ConvertToRadians = true);

		void SetMatScale(float scaleX, float scaleY, float scaleZ);

		void SetMatTrans(float transX, float transY, float transZ);

		void CreateMatrixWorld(DirectX::XMMATRIX matScale, DirectX::XMMATRIX matRot,DirectX:: XMMATRIX matTransform);

		void UpdataBillBordMatrixWorld(View view);

		void UpdataMatrixWorld();


	};
}

