#include "NInput.h"

namespace NuNu
{
	std::vector<Input::Key> Input::mKeys = {};

	int ASCII[(UINT)eKeyCode::End] = {
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,
		MK_LBUTTON, MK_MBUTTON, MK_RBUTTON,
	};

	void Input::Initailize()
	{
		//mKeys.resize((UINT)eKeyCode::End);

		for (size_t i = 0; i < (UINT)eKeyCode::End; i++)
		{
			Key key = {};
			key.bPressed = false;
			key.state = eKeyState::None;
			key.keyCode = (eKeyCode)i;

			mKeys.push_back(key);
		}
	}

	void Input::Update()
	{
		for (size_t i = 0; i < mKeys.size(); i++)
		{
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{	// 키 누름
				if (mKeys[i].bPressed == true) // 꾹 누른 상태
				{
					mKeys[i].state = eKeyState::Pressed;
				}
				else // 이전에 안누르고 지금 누름
				{
					mKeys[i].state = eKeyState::Down;
				}

				mKeys[i].bPressed = true;
			}
			else
			{	// 키 안누름
				if (mKeys[i].bPressed == true) // 누르고 있는 상태에서 키 뗌
				{
					mKeys[i].state = eKeyState::Up;
				}
				else // 이전에 안누르고 지금도 안누름
				{
					mKeys[i].state = eKeyState::None;
				}

				mKeys[i].bPressed = false;
			}
		}
	}
}