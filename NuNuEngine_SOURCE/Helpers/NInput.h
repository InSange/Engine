#pragma once
#include "Common/CommonInclude.h"

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
		SpaceBar,
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


		__forceinline static bool GetKeyDown(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Down; }
		__forceinline static bool GetKeyUp(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Up; }
		__forceinline static bool GetKey(eKeyCode key) { return mKeys[(UINT)key].state == eKeyState::Pressed; }
		__forceinline static math::Vector2 GetMousePosition() { return mMousePosition; }

	private:
		static void createKeys();
		static void updateKeys();
		static void updateKey(Input::Key& key);
		static bool isKeyDown(eKeyCode code);
		static void updateKeyDown(Input::Key& key);
		static void updateKeyUp(Input::Key& key);
		static void getMousePositionByWindow();
		static void clearKeys();

	private:
		static std::vector<Key> mKeys;
		static math::Vector2 mMousePosition;
	};
}


