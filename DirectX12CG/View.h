#pragma once
#include <DirectXMath.h>


namespace MCB
{
    class View
    {
        public:
        DirectX::XMMATRIX mat = DirectX::XMMatrixIdentity();//行列
        DirectX::XMFLOAT3 eye{ 0,0,0 }; //視点座標(カメラ自身の座標)
        DirectX::XMFLOAT3 target{ 0,0,1 };//注視点座標(カメラが見ている座標、XMMatrixLookAtLHはこの座標から視点座標へのベクトルを求めることで向きを決定している)
        DirectX::XMFLOAT3 up{ 0,1,0 };//上方向ベクトル(カメラの上側がどこかを決定する座標。)
        DirectX::XMMATRIX billMat = DirectX::XMMatrixIdentity();


        /// <summary>
        /// 引数の値をクラスのそれぞれの変数に代入した上でビュー変換行列を作成します。
        /// </summary>
        /// <param name="eye">視点座標</param>
        /// <param name="target">注視点座標</param>
        /// <param name="up">上方向ベクトル</param>
        void CreateMatrixView(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 up);

        /// <summary>
        /// すでにある変数を利用して行列を作成します
        /// </summary>
        void UpDateMatrixView();
    };

}

