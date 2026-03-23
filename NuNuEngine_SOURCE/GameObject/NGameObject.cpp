#include "GameObject/NGameObject.h"
#include "Helpers/NInput.h"
#include "Helpers/NTime.h"
#include "High Level Interface/NApplication.h"
#include "Component/Transform/NTransform.h"

namespace NuNu::object
{
	void Destroy(GameObject* gameObject)
	{
		if (gameObject != nullptr)
			gameObject->death();
	}
}

namespace NuNu
{
#pragma region GameObject - Player
	GameObject::GameObject()
		: mState(eState::Active)
		, mLayerType(eLayerType::None)
	{
		mComponents.resize((UINT)enums::eComponentType::End);
		initializeTransform();
	}

	GameObject::~GameObject()
	{
		for (const Component* comp : mComponents)
		{
			if (comp == nullptr) continue;

			SAFE_DELETE(comp);
		}
	}

	void GameObject::Initialize()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr) continue;
			comp->Initialize();
		}
	}

	void GameObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr) continue;
			comp->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr) continue;
			comp->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr) continue;
			comp->Render();
		}
	}


	void GameObject::initializeTransform()
	{
		AddComponent<Transform>();
	}
#pragma endregion
}
