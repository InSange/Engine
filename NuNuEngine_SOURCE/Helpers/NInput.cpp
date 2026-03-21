#include "Helpers/NInput.h"
#include "High Level Interface/NApplication.h"

extern NuNu::Application application;

namespace NuNu
{
	std::vector<Input::Key> Input::mKeys = {};
	math::Vector2 Input::mMousePosition = math::Vector2::One;

	int ASCII[(UINT)eKeyCode::End] = {
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,
		VK_LBUTTON, VK_MBUTTON, VK_RBUTTON,
		VK_SPACE,
	};

	void Input::Initailize()
	{
		createKeys();
	}

	void Input::Update()
	{
		updateKeys();
	}

	void Input::createKeys()
	{
		for (size_t i = 0; i < (UINT)eKeyCode::End; i++)
		{
			Key key = {};
			key.bPressed = false;
			key.state = eKeyState::None;
			key.keyCode = (eKeyCode)i;

			mKeys.push_back(key);
		}
	}

	void Input::updateKeys()
	{
		std::for_each(mKeys.begin(), mKeys.end(),
			[](Key& key) -> void
			{
				updateKey(key);
			});
	}

	void Input::updateKey(Input::Key& key)
	{
		if (GetFocus())
		{
			if (isKeyDown(key.keyCode))
			{
				updateKeyDown(key);
			}
			else
			{
				updateKeyUp(key);
			}

			getMousePositionByWindow();
		}
		else
		{
			clearKeys();
		}
	}

	bool Input::isKeyDown(eKeyCode code)
	{
		return GetAsyncKeyState(ASCII[(UINT)code]) & 0x8000;
	}

	void Input::updateKeyDown(Input::Key& key)
	{
		if (key.bPressed == true)
			key.state = eKeyState::Pressed;
		else
			key.state = eKeyState::Down;

		key.bPressed = true;
	}

	void Input::updateKeyUp(Input::Key& key)
	{
		if (key.bPressed == true)
			key.state = eKeyState::Up;
		else
			key.state = eKeyState::None;

		key.bPressed = false;
	}

	void Input::getMousePositionByWindow()
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		ScreenToClient(application.GetHwnd(), &mousePos);

		UINT width = application.GetWidth();
		UINT height = application.GetHeight();

		mMousePosition.x = -1.0f;
		mMousePosition.y = -1.0f;

		if (CAST_UINT(mousePos.x) > 0 && CAST_UINT(mousePos.x) < width)
			mMousePosition.x = CAST_FLOAT(mousePos.x);
		if (CAST_UINT(mousePos.y) > 0 && CAST_UINT(mousePos.y) < height)
			mMousePosition.y = CAST_FLOAT(mousePos.y);
	}

	void Input::clearKeys()
	{
		for (Key& key : mKeys)
		{
			if (key.state == eKeyState::Down || key.state == eKeyState::Pressed)
			{
				key.state = eKeyState::Up;
			}
			else if (key.state == eKeyState::Up)
			{
				key.state = eKeyState::None;
			}

			key.bPressed = false;
		}
	}
}
