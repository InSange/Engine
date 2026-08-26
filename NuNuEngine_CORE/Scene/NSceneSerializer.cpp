#include "NSceneSerializer.h"
#include "NScene.h"
#include "NSceneManager.h"
#include "Layer/NLayer.h"
#include "Object/NObject.h"
#include "GameObject/NGameObject.h"
#include "Component/Component/NComponent.h"
#include "Component/Transform/NTransform.h"
#include "Component/MeshRenderer/NMeshRenderer.h"
#include "Component/Collider3D/NCollider3D.h"
#include "Component/CharacterController/NCharacterController.h"
#include "Component/Camera/NSceneCamera.h"
#include "Component/Health/NHealth.h"
#include "Component/Curse/NCurseComponent.h"
#include "Component/Curse/NKarmaComponent.h"
#include "Resource/NResources.h"
#include "Resource/Mesh3D/NMesh3D.h"
#include "Resource/Graphics/Shader/NShader.h"
#include "High Level Interface/Renderer/NRenderer.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <filesystem>
#include <functional>
#include <unordered_map>

using json = nlohmann::json;

namespace NuNu
{
	// ── 유틸 ───────────────────────────────────────────────────────────────────

	static std::string wtos(const std::wstring& ws)
	{
		if (ws.empty()) return "";
		int n = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string s(n - 1, 0);
		WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, s.data(), n, nullptr, nullptr);
		return s;
	}

	static std::wstring stow(const std::string& s)
	{
		if (s.empty()) return L"";
		int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, nullptr, 0);
		std::wstring ws(n - 1, 0);
		MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, ws.data(), n);
		return ws;
	}

	static json vec3(const math::Vector3& v) { return { v.x, v.y, v.z }; }
	static math::Vector3 vec3(const json& j)
	{
		return { j[0].get<float>(), j[1].get<float>(), j[2].get<float>() };
	}

	// ── ComponentHandler ───────────────────────────────────────────────────────
	// 새 컴포넌트 추가 시 RegisterHandlers()에 항목 1개만 추가하면 됨

	struct ComponentHandler
	{
		std::function<void(json&, Component*)>        save;    // 컴포넌트 → json
		std::function<Component*(GameObject*)>        create;  // json 없이 컴포넌트 생성
		std::function<void(const json&, Component*)>  load;    // json → 컴포넌트 설정
	};

	static std::unordered_map<std::string, ComponentHandler> sHandlers;

	static void RegisterHandlers()
	{
		if (!sHandlers.empty()) return; // 이미 등록됨

		// ── Transform ──────────────────────────────────────────────────────────
		sHandlers["Transform"] = {
			[](json& j, Component* c) {
				auto* tr = static_cast<Transform*>(c);
				j["position"] = vec3(tr->GetPosition());
				j["rotation"] = vec3(tr->GetRotation());
				j["scale"]    = vec3(tr->GetScale());
			},
			nullptr, // Transform은 GameObject 생성 시 자동 추가
			[](const json& j, Component* c) {
				auto* tr = static_cast<Transform*>(c);
				tr->SetPosition(vec3(j["position"]));
				tr->SetRotation(vec3(j["rotation"]));
				tr->SetScale(vec3(j["scale"]));
			}
		};

		// ── MeshRenderer ───────────────────────────────────────────────────────
		sHandlers["MeshRenderer"] = {
			[](json& j, Component* c) {
				auto* mr = static_cast<MeshRenderer*>(c);
				const float* col = mr->GetColor();
				j["mesh"]      = mr->GetMesh3DName();
				j["shader"]    = mr->GetShaderName();
				j["texture"]   = mr->GetTextureName();
				j["tintColor"] = { col[0], col[1], col[2], col[3] };
			},
			[](GameObject* obj) { return obj->AddComponent<MeshRenderer>(); },
			[](const json& j, Component* c) {
				auto* mr = static_cast<MeshRenderer*>(c);
				std::wstring mesh   = stow(j.value("mesh",   ""));
				std::wstring shader = stow(j.value("shader", ""));
				std::string  tex    = j.value("texture", "");
				if (!mesh.empty())   mr->SetMesh3D(Resources::Find<Mesh3D>(mesh));
				if (!shader.empty()) mr->SetShader3D(Resources::Find<graphics::Shader>(shader));
				if      (tex == "barbarian")  mr->SetTextureSRV(renderer::barbarianTexSRV);
				else if (tex == "platformer") mr->SetTextureSRV(renderer::platformerTexSRV);
				mr->SetTextureName(tex);
				if (j.contains("tintColor"))
				{
					auto& tc = j["tintColor"];
					mr->SetColor(tc[0], tc[1], tc[2], tc[3]);
				}
			}
		};

		// ── Collider3D ─────────────────────────────────────────────────────────
		sHandlers["Collider3D"] = {
			[](json& j, Component* c) {
				auto* col = static_cast<Collider3D*>(c);
				j["halfExtents"] = vec3(col->mHalfExtents);
				j["offset"]      = vec3(col->mOffset);
			},
			[](GameObject* obj) { return obj->AddComponent<Collider3D>(); },
			[](const json& j, Component* c) {
				auto* col = static_cast<Collider3D*>(c);
				col->mHalfExtents = vec3(j["halfExtents"]);
				col->mOffset      = vec3(j["offset"]);
			}
		};

		// ── CharacterController ────────────────────────────────────────────────
		sHandlers["CharacterController"] = {
			[](json& j, Component* c) {
				auto* cc = static_cast<CharacterController*>(c);
				j["moveSpeed"]     = cc->mMoveSpeed;
				j["rotateSpeed"]   = cc->mRotateSpeed;
				j["gravity"]       = cc->mGravity;
				j["jumpSpeed"]     = cc->mJumpSpeed;
				j["eyeHeight"]     = cc->mEyeHeight;
				j["capsuleHeight"] = cc->mCapsuleHeight;
				j["capsuleRadius"] = cc->mCapsuleRadius;
			},
			[](GameObject* obj) { return obj->AddComponent<CharacterController>(); },
			[](const json& j, Component* c) {
				auto* cc = static_cast<CharacterController*>(c);
				cc->mMoveSpeed     = j.value("moveSpeed",     5.0f);
				cc->mRotateSpeed   = j.value("rotateSpeed",   0.2f);
				cc->mGravity       = j.value("gravity",      -20.0f);
				cc->mJumpSpeed     = j.value("jumpSpeed",     8.0f);
				cc->mEyeHeight     = j.value("eyeHeight",     1.7f);
				cc->mCapsuleHeight = j.value("capsuleHeight", 1.8f);
				cc->mCapsuleRadius = j.value("capsuleRadius", 0.4f);
			}
		};

		// ── SceneCamera ────────────────────────────────────────────────────────
		sHandlers["SceneCamera"] = {
			[](json& j, Component* c) {
				j["isMain"] = (renderer::mainCamera == static_cast<SceneCamera*>(c));
			},
			[](GameObject* obj) {
				auto* cam = obj->AddComponent<SceneCamera>();
				cam->SetProjectionType(Camera::eProjectionType::Perspective);
				return cam;
			},
			[](const json& j, Component* c) {
				if (j.value("isMain", false))
					renderer::mainCamera = static_cast<SceneCamera*>(c);
			}
		};

		// ── Health ─────────────────────────────────────────────────────────────
		sHandlers["Health"] = {
			[](json& j, Component* c) { j["maxHp"] = static_cast<Health*>(c)->mMaxHp; },
			[](GameObject* obj)       { return obj->AddComponent<Health>(); },
			[](const json& j, Component* c) {
				static_cast<Health*>(c)->mMaxHp = j.value("maxHp", 100.0f);
			}
		};

		// ── CurseComponent ─────────────────────────────────────────────────────
		sHandlers["CurseComponent"] = {
			[](json& j, Component*) { j["present"] = true; },
			[](GameObject* obj)     { return obj->AddComponent<CurseComponent>(); },
			[](const json&, Component*) {}
		};

		// ── KarmaComponent ─────────────────────────────────────────────────────
		sHandlers["KarmaComponent"] = {
			[](json& j, Component*) { j["present"] = true; },
			[](GameObject* obj)     { return obj->AddComponent<KarmaComponent>(); },
			[](const json&, Component*) {}
		};
	}

	// ── Save ──────────────────────────────────────────────────────────────────

	bool SceneSerializer::Save(Scene* scene, const std::string& path)
	{
		if (scene == nullptr) return false;
		RegisterHandlers();

		json root;
		root["scene"]   = wtos(scene->GetName());
		root["objects"] = json::array();

		const auto& layers = scene->GetLayers();
		for (int li = 0; li < (int)layers.size(); li++)
		{
			for (GameObject* obj : layers[li]->GetGameObjects())
			{
				if (obj == nullptr || obj->IsDead()) continue;

				json jObj;
				jObj["name"]       = wtos(obj->GetName());
				jObj["layer"]      = li;
				jObj["components"] = json::object();

				// GetComponents()를 순회 — 각 컴포넌트가 자신의 키를 반환
				for (Component* comp : obj->GetComponents())
				{
					if (comp == nullptr) continue;
					const char* key = comp->GetComponentKey();
					if (key == nullptr) continue; // 직렬화 제외 컴포넌트

					auto it = sHandlers.find(key);
					if (it == sHandlers.end()) continue;

					json jComp;
					it->second.save(jComp, comp);
					jObj["components"][key] = jComp;
				}

				root["objects"].push_back(jObj);
			}
		}

		std::filesystem::create_directories(std::filesystem::path(path).parent_path());
		std::ofstream f(path);
		if (!f.is_open()) return false;
		f << root.dump(2);
		return true;
	}

	// ── Load ──────────────────────────────────────────────────────────────────

	bool SceneSerializer::Load(const std::string& path)
	{
		RegisterHandlers();

		std::ifstream f(path);
		if (!f.is_open()) return false;

		json root;
		try { root = json::parse(f); }
		catch (...) { return false; }

		for (auto& jObj : root["objects"])
		{
			int       li    = jObj.value("layer", 0);
			eLayerType layer = static_cast<eLayerType>(li);

			// position은 Instantiate에 넘김
			math::Vector3 spawnPos = math::Vector3::Zero;
			if (jObj["components"].contains("Transform"))
				spawnPos = vec3(jObj["components"]["Transform"]["position"]);

			GameObject* obj = object::Instantiate<GameObject>(layer, spawnPos);
			obj->SetName(stow(jObj.value("name", "GameObject")));

			const json& comps = jObj["components"];

			// Transform은 이미 존재 — rotation/scale만 적용
			if (comps.contains("Transform"))
			{
				auto it = sHandlers.find("Transform");
				it->second.load(comps["Transform"], obj->GetComponent<Transform>());
			}

			// 나머지 컴포넌트: 핸들러로 생성 + 설정
			for (auto& [key, jComp] : comps.items())
			{
				if (key == "Transform") continue; // 이미 처리

				auto it = sHandlers.find(key);
				if (it == sHandlers.end()) continue;

				Component* comp = it->second.create(obj);
				if (comp) it->second.load(jComp, comp);
			}
		}

		return true;
	}
}
