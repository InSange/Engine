#include "Scenes/NToolScene.h"
#include "Object/NObject.h"
#include "Contents/NTile.h"
#include "Component/TileMapRenderer/NTilemapRenderer.h"
#include "Resource/NResources.h"
#include "Resource/Texture/NTexture.h"
#include "Component/Camera/NCamera.h"
#include "Renderer/NRenderer.h"
#include "Helpers/NInput.h"
#include "High Level Interface/NApplication.h"
#include "Scripts/NCameraScript.h"

extern NuNu::Application application;

namespace NuNu
{
	ToolScene::ToolScene()
	{
	}

	ToolScene::~ToolScene()
	{
	}

	void ToolScene::Initialize()
	{
		Scene::Initialize();

		GameObject* camera = object::Instantiate<GameObject>(enums::eLayerType::None, Vector2(812.0f, 470.0f));
		Camera* cameraComp = camera->AddComponent<Camera>();
		camera->AddComponent<CameraScript>();

		renderer::mainCamera = cameraComp;

		Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
		TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();

		tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));

	}

	void ToolScene::Update()
	{
		Scene::Update();
	}

	void ToolScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(eKeyCode::LButton))
		{
			createTileObject();
		}

		if (Input::GetKeyDown(eKeyCode::S))
		{
			Save();
		}
		if (Input::GetKeyDown(eKeyCode::L))
		{
			Load();
		}
	}

	void ToolScene::Render()
	{
		Scene::Render();

		renderGrid(hdc);
	}

	void ToolScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void ToolScene::OnExit()
	{
		Scene::OnExit();
	}

	void ToolScene::Save()
	{
		std::filesystem::create_directories("../Resources/Tile");

		OPENFILENAME ofn = {};

		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = L"..\\Resources\\Tile";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (false == GetSaveFileName(&ofn))
			return;

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, szFilePath, L"wb");

		for (Tile* tile : mTiles)
		{
			TilemapRenderer* tmr = tile->GetComponent<TilemapRenderer>();
			Transform* tr = tile->GetComponent<Transform>();

			Vector2 sourceIndex = tmr->GetIndex();
			Vector2 position = tr->GetPosition();

			int x = (int)sourceIndex.x;
			int y = (int)sourceIndex.y;

			if (pFile == nullptr)
				break;

			fwrite(&x, sizeof(int), 1, pFile);
			fwrite(&y, sizeof(int), 1, pFile);

			x = (int)position.x;
			y = (int)position.y;
			fwrite(&x, sizeof(int), 1, pFile);
			fwrite(&y, sizeof(int), 1, pFile);
		}

		if(pFile)
			fclose(pFile);
	}

	void ToolScene::Load()
	{
		OPENFILENAME ofn = {};
		wchar_t szFilePath[256] = {};

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = szFilePath;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = 256;
		ofn.lpstrFilter = L"Tile\0*.tile\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = L"..\\Resources\\Tile";
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (false == GetOpenFileName(&ofn))
			return;

		FILE* pFile = nullptr;
		_wfopen_s(&pFile, szFilePath, L"rb");

		while (true)
		{
			int idxX = 0;
			int idxY = 0;

			int posX = 0;
			int posY = 0;

			if (pFile == nullptr)
				break;

			if (fread(&idxX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&idxY, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posY, sizeof(int), 1, pFile) == NULL)
				break;

			Tile* tile = object::Instantiate<Tile>(eLayerType::Tile, Vector2((float)posX, (float)posY));
			TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();
			tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));
			tmr->SetIndex(Vector2((float)idxX, (float)idxY));

			//tile->SetIndexPosition(posX, posY);
			mTiles.push_back(tile);
		}
		
		if(pFile)
			fclose(pFile);
	}

	void ToolScene::renderGrid(HDC hdc)
	{
		
	}

	void ToolScene::createTileObject()
	{
		Vector2 pos = Input::GetMousePosition();
		pos = renderer::mainCamera->CalculateTilePosition(pos);

		if (pos.x >= 0.0f || pos.y >= 0.0f)
		{
			int idxX = (int)(pos.x / TilemapRenderer::TileSize.x);
			int idxY = (int)(pos.y / TilemapRenderer::TileSize.y);

			Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
			TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();

			tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));
			tmr->SetIndex(TilemapRenderer::SelectedIndex);

			tile->SetIndexPosition(idxX, idxY);
			mTiles.push_back(tile);
		}

	}
}

LRESULT CALLBACK WndTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		POINT mousePos = {};
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);

		NuNu::math::Vector2 mousePosition;
		mousePosition.x = (float)mousePos.x;
		mousePosition.y = (float)mousePos.y;

		int idxX = (int)(mousePosition.x / NuNu::TilemapRenderer::OriginTileSize.x);
		int idxY = (int)(mousePos.y / NuNu::TilemapRenderer::OriginTileSize.y);

		NuNu::TilemapRenderer::SelectedIndex = Vector2((float)idxX, (float)idxY);
	}
	break;
	case WM_KEYDOWN:
	{

	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


