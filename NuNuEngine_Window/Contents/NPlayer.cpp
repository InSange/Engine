#include "Contents/NPlayer.h"
#include <Helpers/NInput.h>
#include <Component/Transform/NTransform.h>
#include <Helpers/NTime.h>

namespace NuNu
{
	void Player::Initialize()
	{
		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();
	}

	void Player::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void Player::Render(const Matrix& view, const Matrix& projection)
	{
		GameObject::Render(view, projection);
	}
}


