#include "NToolScene.h"
#include "NObject.h"
#include "NTile.h"
#include "NTilemapRenderer.h"
#include "NResources.h"
#include "NTexture.h"
#include "NCamera.h"
#include "NRenderer.h"
#include "NInput.h"
#include "NApplication.h"
#include "NCameraScript.h"

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
			Vector2 pos = Input::GetMousePosition();
			pos = renderer::mainCamera->CalculateTilePosition(pos);

			if (pos.x >= 0.0f || pos.y >= 0.0f)
			{
				int idxX = pos.x / TilemapRenderer::TileSize.x;
				int idxY = pos.y / TilemapRenderer::TileSize.y;

				Tile* tile = object::Instantiate<Tile>(eLayerType::Tile);
				TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();

				tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));
				tmr->SetIndex(TilemapRenderer::SelectedIndex);

				tile->SetIndexPosition(idxX, idxY);
				mTiles.push_back(tile);
			}
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

	void ToolScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

		UINT screenWidth = application.GetWidth();
		UINT screenHeight = application.GetHeight();

		int xCount = screenWidth / TilemapRenderer::TileSize.x;
		int yCount = screenHeight / TilemapRenderer::TileSize.y;

		for (size_t i = 0; i <= xCount; i++)
		{
			Vector2 pos = renderer::mainCamera->CalculatePosition(Vector2(TilemapRenderer::TileSize.x * i, 0.0f));
			
			MoveToEx(hdc, pos.x, 0, NULL);
			LineTo(hdc, pos.x, screenHeight);
		}

		for (size_t i = 0; i <= yCount; i++)
		{
			Vector2 pos = renderer::mainCamera->CalculatePosition(Vector2(0.0f, TilemapRenderer::TileSize.y * i));

			MoveToEx(hdc, 0, pos.y, NULL);
			LineTo(hdc, screenWidth, pos.y);
		}
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

			int x = sourceIndex.x;
			fwrite(&x, sizeof(int), 1, pFile);
			int y = sourceIndex.y;
			fwrite(&y, sizeof(int), 1, pFile);

			x = position.x;
			fwrite(&x, sizeof(int), 1, pFile);
			y = position.y;
			fwrite(&y, sizeof(int), 1, pFile);
		}

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

			if (fread(&idxX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&idxY, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posX, sizeof(int), 1, pFile) == NULL)
				break;
			if (fread(&posY, sizeof(int), 1, pFile) == NULL)
				break;

			Tile* tile = object::Instantiate<Tile>(eLayerType::Tile, Vector2(posX, posY));
			TilemapRenderer* tmr = tile->AddComponent<TilemapRenderer>();
			tmr->SetTexture(Resources::Find<graphics::Texture>(L"Overworld"));
			tmr->SetIndex(Vector2(idxX, idxY));

			//tile->SetIndexPosition(posX, posY);
			mTiles.push_back(tile);
		}

		fclose(pFile);
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
		mousePosition.x = mousePos.x;
		mousePosition.y = mousePos.y;

		int idxX = mousePosition.x / NuNu::TilemapRenderer::OriginTileSize.x;
		int idxY = mousePos.y / NuNu::TilemapRenderer::OriginTileSize.y;

		NuNu::TilemapRenderer::SelectedIndex = Vector2(idxX, idxY);
	}
	break;
	case WM_KEYDOWN:
	{

	}
	case WM_PAINT:
	{
		// 여기는 계속 반복적으로 그려냄
		// DC 화면 출력에 필요한 모든 정보를 가지는 데이터 구조체
		// GDI모듈에 의해서 관리
		// 폰트, 선 굵기, 색상 등
		// 화면 출력에 필요한 모든 경우는 WINAPI에서는 DC를 통해서 작업을 진행

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		NuNu::graphics::Texture* texture = NuNu::Resources::Find<NuNu::graphics::Texture>(L"Overworld");

		Gdiplus::Graphics graphics(hdc);

		Gdiplus::Rect destRect(
			static_cast<INT>(0),
			static_cast<INT>(0),
			static_cast<INT>(texture->GetWidth()),
			static_cast<INT>(texture->GetHeight())
		);

		graphics.DrawImage(texture->GetImage(),
			destRect,
			0, 0,
			static_cast<INT>(texture->GetWidth()),
			static_cast<INT>(texture->GetHeight()),
			Gdiplus::UnitPixel, nullptr);//&imgAtt);

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