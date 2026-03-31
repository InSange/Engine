#include "guiKarmaDebugWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "GameObject/NGameObject.h"
#include "Component/Curse/NKarmaComponent.h"
#include "Component/Curse/NCurseComponent.h"

using namespace NuNu;
using namespace NuNu::enums;

namespace gui
{
	static const char* kAbilityNames[] =
	{
		"HealSmall",
		"HealFull",
		"CurseRemove",
		"SpeedBoostTemp",
		"CurseImmuneTemp",
		"AddRandomCurse",
		"BleedDoT",
		"PoisonDoT",
		"SleepStun",
		"AreaExplosion",
		"TransferCurseToFriend",
		"TeleportRandom",
		"WeaponItem",
	};
	static_assert((int)eAbilityType::End == 13, "kAbilityNames size mismatch");

	KarmaDebugWindow::KarmaDebugWindow()
	{
		SetName("Karma Debugger");
		SetSize(ImVec2(320, 380));
	}

	void KarmaDebugWindow::Initialize() {}
	void KarmaDebugWindow::Update()     {}

	void KarmaDebugWindow::OnGUI()
	{
		GameObject* obj = renderer::selectedObject;
		if (obj == nullptr)
		{
			ImGui::TextDisabled("오브젝트를 선택하세요.");
			return;
		}

		KarmaComponent* karma = obj->GetComponent<KarmaComponent>();

		if (karma == nullptr)
		{
			if (ImGui::Button("Add KarmaComponent"))
				obj->AddComponent<KarmaComponent>();
			return;
		}

		// --- 카르마 게이지 ---
		ImGui::SeparatorText("Karma");
		float karmaRatio = karma->GetKarma() / karma->GetMaxKarma();
		ImGui::ProgressBar(karmaRatio, ImVec2(-1, 0));
		ImGui::Text("%.1f / %.1f", karma->GetKarma(), karma->GetMaxKarma());

		ImGui::SameLine();
		if (ImGui::Button("+25"))
			karma->AddKarma(25.0f);
		ImGui::SameLine();
		if (ImGui::Button("+100"))
			karma->AddKarma(100.0f);

		// --- 능력 발동 ---
		ImGui::SeparatorText("Spend Karma (25)");
		if (ImGui::Button("Spend Karma (랜덤 능력 발동)"))
		{
			eAbilityType result = karma->SpendKarma();
			if (result == eAbilityType::End)
				ImGui::OpenPopup("no_karma");
		}

		if (ImGui::BeginPopup("no_karma"))
		{
			ImGui::Text("카르마가 부족합니다.");
			ImGui::EndPopup();
		}

		eAbilityType last = karma->GetLastAbility();
		if (last != eAbilityType::End)
		{
			ImGui::Text("마지막 능력: %s", kAbilityNames[(int)last]);
		}

		// --- 직접 발동 (디버그용) ---
		ImGui::SeparatorText("Debug: 직접 발동");
		static int sAbilityIdx = 0;
		ImGui::Combo("능력 선택", &sAbilityIdx, kAbilityNames, (int)eAbilityType::End);
		if (ImGui::Button("강제 발동 (카르마 소모 없음)"))
		{
			karma->AddKarma(KarmaComponent::kKarmaCostPerSpend);
			karma->SpendKarma(); // 실제로는 랜덤이지만 테스트용
		}

		// --- 상태 표시 ---
		ImGui::SeparatorText("현재 상태");
		if (karma->IsSpeedBoosted())
			ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "SPEED BOOST 중");
		if (karma->IsCurseImmune())
			ImGui::TextColored(ImVec4(0.3f, 0.7f, 1.0f, 1.0f), "CURSE IMMUNE 중");
		if (karma->IsStunned())
			ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "STUN 중 (행동 불능)");
		if (karma->GetDoTDamageThisFrame() > 0.0f)
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "DoT: %.2f/frame", karma->GetDoTDamageThisFrame());
	}

	void KarmaDebugWindow::Run()
	{
		bool active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &active, GetFlag());
		Update();
		OnGUI();
		ImGui::End();
	}
}
