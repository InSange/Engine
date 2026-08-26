#include "Helpers/NInput.h"
#include "High Level Interface/NApplication.h"

extern NuNu::Application application;

namespace NuNu
{
	std::vector<Input::Key> Input::Keys = {};
	Vector2 Input::mMousePosition = Vector2::One;
	bool Input::mBlocked = false;
/*
	int ASCII[static_cast<UINT>(eKeyCode::End)] = {
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,
		VK_LBUTTON, VK_MBUTTON, VK_RBUTTON,
		VK_LCONTROL,
		VK_SPACE,
	};*/

	void Input::Initialize()
	{
		createKeys();
	}

	void Input::Update()
	{
		if (mBlocked == true)
			return;

		updateKeys();
	}

	void Input::createKeys()
	{
		for (int vk = 0; vk <= 0xFF; ++vk)
		{
			eKeyCode keyCode = static_cast<eKeyCode>(vk);

			Key key = {};
			key.bPressed = false;
			key.State = eKeyState::None;
			key.KeyCode = keyCode;
			key.VK_KeyCode = vk;

			Keys.push_back(key);
		}
	}

	void Input::updateKeys()
	{
		std::ranges::for_each(Keys,
			[](Key& key) -> void
			{
				updateKey(key);
			});
	}

	void Input::updateKey(Key& key)
	{
		if (!GetFocus())
		{
			clearKeys();
			return;
		}

		if (isKeyDown(key.KeyCode))
			updateKeyDown(key);
		else
			updateKeyUp(key);

		getMousePositionByWindow();
	}

	bool Input::isKeyDown(eKeyCode code)
	{
		return GetAsyncKeyState(static_cast<int>(code)) & 0x8000;
	}

	void Input::updateKeyDown(Key& key)
	{
		if (key.bPressed == true)
			key.State = eKeyState::Pressed;
		else
			key.State = eKeyState::Down;

		key.bPressed = true;
	}

	void Input::updateKeyUp(Key& key)
	{
		if (key.bPressed == true)
			key.State = eKeyState::Up;
		else
			key.State = eKeyState::None;

		key.bPressed = false;
	}

	void Input::getMousePositionByWindow()
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		ScreenToClient(application.GetWindow().GetHwnd(), &mousePos);

		UINT width = application.GetWindow().GetWidth();
		UINT height = application.GetWindow().GetHeight();

		mMousePosition.x = -1.0f;
		mMousePosition.y = -1.0f;

		if (CAST_UINT(mousePos.x) > 0 && CAST_UINT(mousePos.x) < width)
			mMousePosition.x = CAST_FLOAT(mousePos.x);
		if (CAST_UINT(mousePos.y) > 0 && CAST_UINT(mousePos.y) < height)
			mMousePosition.y = CAST_FLOAT(mousePos.y);
	}

	void Input::clearKeys()
	{
		for (Key& key : Keys)
		{
			if (key.State == eKeyState::Down || key.State == eKeyState::Pressed)
			{
				key.State = eKeyState::Up;
			}
			else if (key.State == eKeyState::Up)
			{
				key.State = eKeyState::None;
			}

			key.bPressed = false;
		}
	}
}
