#include "guiStageDebugWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "GameObject/NGameObject.h"
#include "Stage/NStageManager.h"
#include "Component/Health/NHealth.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NKarmaComponent.h"

using namespace NuNu;
using namespace NuNu::enums;

namespace gui
{
	static const char* kCurseNamesStage[] =
	{
		"없음",
		"HeavyBody", "Miniaturize", "Gigantify", "GlassBody", "FireBody",
		"Invisible",  "Magnet",     "ForceJump", "VisionInvert", "ControlInvert",
	};

	StageDebugWindow::StageDebugWindow()
	{
		SetName("Stage Debugger");
		SetSize(ImVec2(320, 400));
	}

	void StageDebugWindow::Initialize() {}
	void StageDebugWindow::Update()     {}

	void StageDebugWindow::OnGUI()
	{
		// --- 현재 상태 ---
		auto state = StageManager::GetState();
		switch (state)
		{
		case StageManager::eState::Idle:
			ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "State: Idle");
			break;
		case StageManager::eState::InStage:
		{
			ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "State: InStage");
			const StageData& s = StageManager::GetCurrentStage();
			ImGui::Text("Stage %d  |  저주 %d개/인", s.stageId, s.cursesPerPlayer);
			break;
		}
		case StageManager::eState::StageClear:
			ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "State: STAGE CLEAR!");
			break;
		}

		ImGui::Separator();

		// --- 플레이어 등록 ---
		ImGui::SeparatorText("Player 등록");
		ImGui::TextDisabled("selectedObject를 플레이어로 등록");

		if (ImGui::Button("Register Selected"))
		{
			GameObject* obj = renderer::selectedObject;
			if (obj != nullptr)
			{
				// Health / CurseComponent / KarmaComponent 없으면 자동 추가
				if (obj->GetComponent<Health>() == nullptr)
					obj->AddComponent<Health>();
				if (obj->GetComponent<CurseComponent>() == nullptr)
					obj->AddComponent<CurseComponent>();
				if (obj->GetComponent<KarmaComponent>() == nullptr)
					obj->AddComponent<KarmaComponent>();

				StageManager::RegisterPlayer(obj);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Players"))
			StageManager::ClearPlayers();

		// --- 스테이지 로드 ---
		ImGui::SeparatorText("Stage Load");

		static int  sStageId         = 1;
		static int  sCursesPerPlayer  = 1;
		static int  sRequiredIdx      = 0; // 0 = 없음

		ImGui::InputInt("Stage ID",      &sStageId);
		ImGui::SliderInt("저주 수/인",    &sCursesPerPlayer, 1, (int)eCurseType::End);
		if (ImGui::BeginCombo("필수 저주", kCurseNamesStage[sRequiredIdx]))
		{
			for (int i = 0; i <= (int)eCurseType::End; i++)
			{
				if (ImGui::Selectable(kCurseNamesStage[i], sRequiredIdx == i))
					sRequiredIdx = i;
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Load Stage"))
		{
			StageData data;
			data.stageId        = sStageId;
			data.cursesPerPlayer = sCursesPerPlayer;
			data.requiredCurse  = (sRequiredIdx == 0)
				? eCurseType::End
				: (eCurseType)(sRequiredIdx - 1);
			StageManager::LoadStage(data);
		}

		// --- 수동 클리어 ---
		ImGui::SeparatorText("Debug Actions");
		if (ImGui::Button("Stage Clear (수동)"))
			StageManager::OnStageClear();

		// --- 등록된 플레이어 Health 표시 ---
		ImGui::SeparatorText("Registered Players");
		// selectedObject의 Health 상태 표시 (간이)
		GameObject* obj = renderer::selectedObject;
		if (obj)
		{
			Health* hp = obj->GetComponent<Health>();
			if (hp)
			{
				ImGui::Text("HP: %.0f / %.0f", hp->GetHp(), hp->GetMaxHp());
				if (hp->IsDead())
					ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "DEAD");

				if (ImGui::Button("TakeDamage 10"))  hp->TakeDamage(10.0f);
				ImGui::SameLine();
				if (ImGui::Button("Kill"))            hp->TakeDamage(hp->GetMaxHp());
				ImGui::SameLine();
				if (ImGui::Button("Heal Full"))       hp->Heal(hp->GetMaxHp());
			}
			else
			{
				ImGui::TextDisabled("Health 컴포넌트 없음");
			}
		}
	}

	void StageDebugWindow::Run()
	{
		bool active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &active, GetFlag());
		Update();
		OnGUI();
		ImGui::End();
	}
}
