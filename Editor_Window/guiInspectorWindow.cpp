#include "guiInspectorWindow.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "GameObject/NGameObject.h"
#include "Component/Transform/NTransform.h"
#include "Component/MeshRenderer/NMeshRenderer.h"
#include "Component/Collider3D/NCollider3D.h"
#include "Component/CharacterController/NCharacterController.h"
#include "Component/Health/NHealth.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NKarmaComponent.h"

using namespace NuNu;
using namespace NuNu::math;

namespace gui
{
	// 컴포넌트 섹션 헤더 — 접힌 상태 반환
	static bool componentHeader(const char* label)
	{
		ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.30f, 0.35f, 1.0f));
		bool open = ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen);
		ImGui::PopStyleColor(2);
		return open;
	}

	// ── Transform ──────────────────────────────────────────────────────────────
	static void drawTransform(Transform* tr)
	{
		if (!componentHeader("Transform")) return;

		Vector3 pos = tr->GetPosition();
		Vector3 rot = tr->GetRotation();
		Vector3 scl = tr->GetScale();

		// DragFloat3: 드래그로 조작, Ctrl+클릭으로 숫자 직접 입력
		ImGui::PushItemWidth(-1.0f);

		ImGui::Text("Position");
		if (ImGui::DragFloat3("##pos", &pos.x, 0.05f, 0.0f, 0.0f, "%.3f"))
			tr->SetPosition(pos);

		ImGui::Text("Rotation");
		if (ImGui::DragFloat3("##rot", &rot.x, 0.5f, 0.0f, 0.0f, "%.2f"))
			tr->SetRotation(rot);

		ImGui::Text("Scale");
		if (ImGui::DragFloat3("##scl", &scl.x, 0.01f, 0.001f, 100.0f, "%.3f"))
			tr->SetScale(scl);

		ImGui::PopItemWidth();
		ImGui::Spacing();
	}

	// ── MeshRenderer ───────────────────────────────────────────────────────────
	static void drawMeshRenderer(MeshRenderer* mr)
	{
		if (!componentHeader("MeshRenderer")) return;

		float col[4];
		memcpy(col, mr->GetColor(), sizeof(col));
		if (ImGui::ColorEdit4("Tint Color", col))
			mr->SetColor(col[0], col[1], col[2], col[3]);

		ImGui::Spacing();
	}

	// ── Collider3D ─────────────────────────────────────────────────────────────
	static void drawCollider3D(Collider3D* col)
	{
		if (!componentHeader("Collider3D")) return;

		ImGui::PushItemWidth(-1.0f);

		ImGui::Text("Half Extents");
		ImGui::DragFloat3("##he", &col->mHalfExtents.x, 0.05f, 0.001f, 500.0f, "%.3f");

		ImGui::Text("Offset");
		ImGui::DragFloat3("##off", &col->mOffset.x, 0.05f, -100.0f, 100.0f, "%.3f");

		ImGui::PopItemWidth();
		ImGui::Spacing();
	}

	// ── CharacterController ────────────────────────────────────────────────────
	static void drawCharacterController(CharacterController* cc)
	{
		if (!componentHeader("CharacterController")) return;

		ImGui::PushItemWidth(120.0f);

		ImGui::DragFloat("Move Speed",   &cc->mMoveSpeed,   0.1f, 0.1f, 50.0f);
		ImGui::DragFloat("Jump Speed",   &cc->mJumpSpeed,   0.1f, 0.1f, 30.0f);
		ImGui::DragFloat("Gravity",      &cc->mGravity,     0.1f, 1.0f, 50.0f);
		ImGui::DragFloat("Eye Height",   &cc->mEyeHeight,   0.01f, 0.1f, 3.0f);
		ImGui::DragFloat("Capsule H",    &cc->mCapsuleHeight, 0.01f, 0.1f, 5.0f);
		ImGui::DragFloat("Capsule R",    &cc->mCapsuleRadius, 0.01f, 0.05f, 2.0f);

		ImGui::PopItemWidth();
		ImGui::Spacing();
	}

	// ── Health ─────────────────────────────────────────────────────────────────
	static void drawHealth(Health* hp)
	{
		if (!componentHeader("Health")) return;

		float ratio = hp->GetHp() / hp->GetMaxHp();
		ImVec4 barCol = hp->IsDead()
			? ImVec4(0.7f, 0.1f, 0.1f, 1.0f)
			: ImVec4(0.1f + ratio * 0.5f, ratio * 0.8f, 0.1f, 1.0f);

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, barCol);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::ProgressBar(ratio, ImVec2(-1, 0));
		ImGui::PopStyleColor(2);

		ImGui::Text("HP: %.0f / %.0f", hp->GetHp(), hp->GetMaxHp());
		ImGui::DragFloat("Max HP", &hp->mMaxHp, 1.0f, 1.0f, 9999.0f);

		if (hp->IsDead())
			ImGui::TextColored(ImVec4(1, 0.3f, 0.3f, 1), "DEAD");

		if (ImGui::Button("TakeDamage 10"))  hp->TakeDamage(10.0f);
		ImGui::SameLine();
		if (ImGui::Button("Kill"))           hp->TakeDamage(hp->GetMaxHp());
		ImGui::SameLine();
		if (ImGui::Button("Heal Full"))      hp->Heal(hp->GetMaxHp());

		ImGui::Spacing();
	}

	// ── CurseComponent ─────────────────────────────────────────────────────────
	static void drawCurseComponent(CurseComponent* cc)
	{
		if (!componentHeader("CurseComponent")) return;

		static const char* kNames[] = {
			"무거운몸","소형화","거인화","유리몸","불꽃몸",
			"투명화","자석","강제점프","시야반전","조작반전"
		};

		const auto& list = cc->GetCurseList();
		if (list.empty())
		{
			ImGui::TextDisabled("저주 없음");
		}
		else
		{
			for (int i = 0; i < (int)list.size(); i++)
			{
				int idx = (int)list[i];
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f),
					"[%d] %s", i, (idx < 10) ? kNames[idx] : "?");
			}
		}

		// 이동속도/데미지 배율 읽기 전용 표시
		ImGui::Spacing();
		ImGui::Text("MoveSpeed x%.2f", cc->GetMoveSpeedMultiplier());
		ImGui::Text("IncomingDmg x%.2f", cc->GetIncomingDamageMultiplier());
		if (cc->IsControlInverted()) ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "CONTROL INVERTED");
		if (cc->IsVisionInverted())  ImGui::TextColored(ImVec4(1,0.4f,0.4f,1), "VISION INVERTED");

		ImGui::Spacing();
	}

	// ── KarmaComponent ─────────────────────────────────────────────────────────
	static void drawKarmaComponent(KarmaComponent* karma)
	{
		if (!componentHeader("KarmaComponent")) return;

		float ratio = karma->GetKarma() / karma->GetMaxKarma();
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.8f, 0.6f, 0.1f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
		ImGui::ProgressBar(ratio, ImVec2(-1, 0));
		ImGui::PopStyleColor(2);

		ImGui::Text("Karma: %.1f / %.1f", karma->GetKarma(), karma->GetMaxKarma());

		if (karma->IsSpeedBoosted()) ImGui::TextColored(ImVec4(0.3f,1,0.3f,1), "SPEED BOOST");
		if (karma->IsCurseImmune())  ImGui::TextColored(ImVec4(0.3f,0.7f,1,1), "CURSE IMMUNE");
		if (karma->IsStunned())      ImGui::TextColored(ImVec4(1,0.3f,0.3f,1), "STUNNED");

		ImGui::Spacing();
	}

	// ── InspectorWindow ────────────────────────────────────────────────────────

	InspectorWindow::InspectorWindow()
	{
		SetName("Inspector");
		SetSize(ImVec2(300, 600));
	}

	InspectorWindow::~InspectorWindow() {}

	void InspectorWindow::Initialize() {}
	void InspectorWindow::Update()
	{
		for (Editor* editor : mEditors)
			editor->Update();
	}

	void InspectorWindow::OnGUI()
	{
		for (Editor* editor : mEditors)
			editor->OnGUI();

		GameObject* obj = renderer::selectedObject;
		if (obj == nullptr)
		{
			ImGui::TextDisabled("오브젝트를 선택하세요.");
			return;
		}

		// ── 오브젝트 이름 편집 ──────────────────────────────────────────────────
		{
			// wstring → char buf 변환
			static char nameBuf[256] = {};
			const std::wstring& wn = const_cast<GameObject*>(obj)->GetName();
			WideCharToMultiByte(CP_UTF8, 0, wn.c_str(), -1, nameBuf, sizeof(nameBuf), nullptr, nullptr);

			ImGui::SetNextItemWidth(-1.0f);
			if (ImGui::InputText("##objname", nameBuf, sizeof(nameBuf), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				int wlen = MultiByteToWideChar(CP_UTF8, 0, nameBuf, -1, nullptr, 0);
				std::wstring newName(wlen - 1, 0);
				MultiByteToWideChar(CP_UTF8, 0, nameBuf, -1, newName.data(), wlen);
				obj->SetName(newName);
			}
		}

		ImGui::Separator();

		// ── 컴포넌트 패널들 ────────────────────────────────────────────────────
		if (auto* tr   = obj->GetComponent<Transform>())           drawTransform(tr);
		if (auto* mr   = obj->GetComponent<MeshRenderer>())        drawMeshRenderer(mr);
		if (auto* col  = obj->GetComponent<Collider3D>())          drawCollider3D(col);
		if (auto* cc   = obj->GetComponent<CharacterController>()) drawCharacterController(cc);
		if (auto* hp   = obj->GetComponent<Health>())              drawHealth(hp);
		if (auto* curs = obj->GetComponent<CurseComponent>())      drawCurseComponent(curs);
		if (auto* karm = obj->GetComponent<KarmaComponent>())      drawKarmaComponent(karm);
	}

	void InspectorWindow::Run()
	{
		bool Active = (bool)GetState();
		ImGui::Begin(GetName().c_str(), &Active, GetFlag());
		Update();
		OnGUI();
		ImGui::End();
	}

	void InspectorWindow::OnEnable()  {}
	void InspectorWindow::OnDisable() {}
	void InspectorWindow::OnDestroy() {}
}
