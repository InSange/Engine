#pragma once
#include "Scene/NScene.h"
#include "Contents/NTile.h"

namespace NuNu
{
	class ToolScene : public Scene
	{
	public:
		ToolScene();
		~ToolScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void OnEnter() override;
		void OnExit() override;

		void Save();
		void Load();

	private:
		void renderGrid(HDC hdc);
		void createTileObject();

	private:
		std::vector<Tile*> mTiles;
	};
}

LRESULT CALLBACK WndTileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

