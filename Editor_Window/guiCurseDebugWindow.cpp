#include "guiCurseDebugWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "GameObject/NGameObject.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NCurseManager.h"

using namespace NuNu;
using namespace NuNu::enums;

namespace gui
{
	static const char* kCurseLabels[] =
	{
		"HeavyBody     (이동속도 -50%)",
		"Miniaturize   (크기 0.3배)",
		"Gigantify     (크기 3배)",
		"GlassBody     (받는 피해 3배)",
		"FireBody      (지속 피해)",
		"Invisible     (아군 시야 사라짐)",
		"Magnet        (금속 달라붙음)",
		"ForceJump     (5초마다 강제 점프)",
		"VisionInvert  (상하 시야 반전)",
		"ControlInvert (좌우 이동 반전)",
	};
	static_assert((int)eCurseType::End == 10, "kCurseLabels size mismatch");

	CurseDebugWindow::CurseDebugWindow()
	{
		SetName("Curse Debugger");
		SetSize(ImVec2(320, 400));
	}

	void CurseDebugWindow::Initialize() {}
	void CurseDebugWindow::Update()     {}

	void CurseDebugWindow::OnGUI()
	{
		GameObject* obj = renderer::selectedObject;
		if (obj == nullptr)
		{
			ImGui::TextDisabled("No object selected");
			return;
		}

		CurseComponent* curse = obj->GetComponent<CurseComponent>();

		if (curse == nullptr)
		{
			ImGui::TextDisabled("CurseComponent 없음");
			if (ImGui::Button("Add CurseComponent"))
				obj->AddComponent<CurseComponent>();
			return;
		}

		// --- CurseManager 테스트 ---
		ImGui::SeparatorText("CurseManager Test");

		static int sCursesPerPlayer = 2;
		static int sRequiredCurse   = (int)eCurseType::End; // End = 없음
		ImGui::SliderInt("Per Player", &sCursesPerPlayer, 1, (int)eCurseType::End);

		const char* requiredLabel = (sRequiredCurse == (int)eCurseType::End)
			? "없음"
			: kCurseLabels[sRequiredCurse];
		if (ImGui::BeginCombo("Required", requiredLabel))
		{
			if (ImGui::Selectable("없음", sRequiredCurse == (int)eCurseType::End))
				sRequiredCurse = (int)eCurseType::End;
			for (int i = 0; i < (int)eCurseType::End; i++)
			{
				if (ImGui::Selectable(kCurseLabels[i], sRequiredCurse == i))
					sRequiredCurse = i;
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Assign (이 오브젝트)"))
		{
			CurseManager::ClearAll({ curse });
			CurseManager::AssignCurses(
				{ curse },
				sCursesPerPlayer,
				(eCurseType)sRequiredCurse);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear All"))
			CurseManager::ClearAll({ curse });

		ImGui::SeparatorText("Curses");

		constexpr int kCount = (int)eCurseType::End;
		for (int i = 0; i < kCount; i++)
		{
			eCurseType type = (eCurseType)i;
			bool has = curse->HasCurse(type);
			if (ImGui::Checkbox(kCurseLabels[i], &has))
			{
				if (has) curse->AddCurse(type);
				else     curse->RemoveCurse(type);
			}
		}

		ImGui::SeparatorText("Modifiers");
		ImGui::Text("MoveSpeed    x %.2f", curse->GetMoveSpeedMultiplier());
		ImGui::Text("IncomingDmg  x %.2f", curse->GetIncomingDamageMultiplier());

		if (curse->IsControlInverted())
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "CONTROL INVERTED");
		if (curse->IsVisionInverted())
			ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "VISION INVERTED");
	}

	void CurseDebugWindow::Run()
	{
		bool active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &active, GetFlag());
		Update();
		OnGUI();
		ImGui::End();
	}
}
