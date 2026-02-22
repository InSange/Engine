#pragma once
#include "CommonInclude.h"

namespace NuNu
{
	enum class eKeyState
	{
		Down = 0,
		Pressed,
		Up,
		None,
	};

	enum class eKeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M, 
		Left, Right, Down, Up,
		LButton, MButton, RButton,
		End,
	};

	class Input
	{
	public:
		struct Key
		{
			eKeyCode keyCode;
			eKeyState state;
			bool bPressed;
		};

		static void Initailize();
		static void Update();

		static bool GetKeyDown(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Down; }
		static bool GetKeyUp(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Up; }
		static bool GetKey(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Pressed; }

	private:
		eKeyState mState = eKeyState::Down;
		static std::vector<Key> mKeys;
	};
}

