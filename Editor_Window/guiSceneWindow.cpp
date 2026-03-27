#include "guiSceneWindow.h"


#include "High Level Interface/Renderer/NRenderer.h"
#include "Component/Transform/NTransform.h"
#include "Graphics/GraphicDevice/NGraphicDevice_DX12.h"
#include "High Level Interface/NApplication.h"
#include "High Level Interface/NWindow.h"
#include "Scene/NSceneManager.h"

#include "guiEditorApplication.h"

extern NuNu::Application application;

namespace gui
{
	SceneWindow::SceneWindow()
		: mEditorCameraObject(nullptr)
		, mEditorCamera(nullptr)
		, ViewportFocused(false)
		, ViewportHovered(false)
		, GuizmoType(-1)
		, ViewportBounds{}
		, ViewportSize{}
	{
		SetName("Scene");
		SetSize(ImVec2(300, 600));

		Initialize();
	}

	SceneWindow::~SceneWindow()
	{
		delete mEditorCameraObject;
		mEditorCameraObject = nullptr;
	}

	void SceneWindow::Initialize()
	{
		mEditorCameraObject = new NuNu::GameObject();
		mEditorCameraObject->SetName(L"EditorCamera");

		NuNu::Transform* tr = mEditorCameraObject->GetComponent<NuNu::Transform>();
		tr->SetPosition(3.0f, 0.0f, -20.0f);
		tr->SetRotation(0.0f, 0.0f, 0.0f);

		mEditorCamera = mEditorCameraObject->AddComponent<NuNu::EditorCamera>();
		mEditorCamera->SetProjectionType(NuNu::Camera::eProjectionType::Perspective);

		// set the render target for the editor camera
		const NuNu::Window::WindowData& windowData = application.GetWindow().GetData();
		mEditorCamera->CreateRenderTarget(windowData.Width, windowData.Height);
	}

	void SceneWindow::Update()
	{
		for (Editor* editor : mEditors)
		{
			editor->Update();
		}
	}

	void SceneWindow::OnGUI()
	{

		for (Editor* editor : mEditors)
		{
			editor->OnGUI();
		}
	}

	void SceneWindow::Run()
	{
		bool Active = (bool)GetState();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin(GetName().c_str(), &Active, GetFlag());

		Update();
		OnGUI();

		// Calculate view, projection, and camera position
		NuNu::Transform* cameraTr = mEditorCamera->GetOwner()->GetComponent<NuNu::Transform>();
		cameraTr->LateUpdate();
		mEditorCamera->LateUpdate();

#if 0
		// clear the render target view & depth stencil view
		NuNu::graphics::RenderTarget* rt = mEditorCamera->GetRenderTarget();

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rtv = rt->GetAttachmentTexture(0)->GetRTV();
		NuNu::graphics::GetDevice<NuNu::graphics::GraphicDevice_DX11>()->ClearRenderTargetView(rtv);
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv = rt->GetDepthAttachment()->GetDSV();
		NuNu::graphics::GetDevice<NuNu::graphics::GraphicDevice_DX11>()->ClearDepthStencilView(dsv);

		// set scene view render target & depth stencil view
		NuNu::graphics::GetDevice<NuNu::graphics::GraphicDevice_DX11>()->BindRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
#endif

		// render the scene
		Matrix viewMatrix = mEditorCamera->GetViewMatrix();
		Matrix projectionMatrix = mEditorCamera->GetProjectionMatrix();
		Vector3 cameraPos = mEditorCamera->GetOwner()->GetComponent<NuNu::Transform>()->GetPosition();

		std::vector<NuNu::GameObject*> opaqueList = {};
		std::vector<NuNu::GameObject*> cutoutList = {};
		std::vector<NuNu::GameObject*> transparentList = {};

		// collect randerables(game objects)
		NuNu::Scene* scene = NuNu::SceneManager::GetActiveScene();
		NuNu::renderer::CollectRenderables(scene, opaqueList, cutoutList, transparentList);

		// sorting renderables by distance (between camera and game object)
		NuNu::renderer::SortByDistance(opaqueList, cameraPos, true);
		NuNu::renderer::SortByDistance(cutoutList, cameraPos, true);
		NuNu::renderer::SortByDistance(transparentList, cameraPos, false);

		//render game objects
		/*NuNu::renderer::RenderRenderables(opaqueList, viewMatrix, projectionMatrix);
		NuNu::renderer::RenderRenderables(cutoutList, viewMatrix, projectionMatrix);
		NuNu::renderer::RenderRenderables(transparentList, viewMatrix, projectionMatrix);*/

		// render the scene from the editor camera
		NuNu::renderer::RenderSceneFromCamera(scene, mEditorCamera);

		// imgui scene view viewport
		const auto viewportMinRegion = ImGui::GetWindowContentRegionMin(); // 씬뷰의 최소 좌표
		const auto viewportMaxRegion = ImGui::GetWindowContentRegionMax(); // 씬뷰의 최대 좌표
		const auto viewportOffset = ImGui::GetWindowPos(); // 씬뷰의 위치

		constexpr int letTop = 0;
		constexpr int rightBottom = 1;
		ViewportBounds[letTop] = Vector2{ viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		ViewportBounds[rightBottom] = Vector2{ viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };


		// get the camera render target view
		// rendering framebuffer image to the sceneview
		NuNu::graphics::RenderTarget* frameBuffer = mEditorCamera->GetRenderTarget();
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		NuNu::math::Vector2 ViewportSize = Vector2{ viewportPanelSize.x, viewportPanelSize.y };
#if 0 // DX12 전환 중: RenderTarget GPU 리소스 미구현
		NuNu::graphics::Texture* texture = frameBuffer->GetAttachmentTexture(0);
		ImGui::Image((ImTextureID)texture->GetSRV().Get(), ImVec2{ ViewportSize.x, ViewportSize.y }
		, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });
#endif

		// To do : guizmo
		NuNu::GameObject* selectedObject = NuNu::renderer::selectedObject;
		if (selectedObject && GuizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetGizmoSizeClipSpace(0.15f);
			ImGuizmo::SetRect(ViewportBounds[0].x, ViewportBounds[0].y
				, ViewportBounds[1].x - ViewportBounds[0].x, ViewportBounds[1].y - ViewportBounds[0].y);

			// To do : guizmo...
			// game view camera setting

			// Scene Camera
			const NuNu::math::Matrix& viewMatrix = mEditorCamera->GetViewMatrix();
			const NuNu::math::Matrix& projectionMatrix = mEditorCamera->GetProjectionMatrix();

			// Object Transform
			NuNu::Transform* transform = selectedObject->GetComponent<NuNu::Transform>();
			NuNu::math::Matrix worldMatrix = transform->GetWorldMatrix();

			// snapping
			bool snap = NuNu::Input::GetKey(NuNu::eKeyCode::Leftcontrol);
			float snapValue = 0.5f;

			// snap to 45 degrees for rotation
			if (GuizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(*viewMatrix.m, *projectionMatrix.m, static_cast<ImGuizmo::OPERATION>(GuizmoType)
				, ImGuizmo::WORLD, *worldMatrix.m, nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				// Decompose matrix to translation, rotation and scale
				float translation[3];
				float rotation[3];
				float scale[3];
				ImGuizmo::DecomposeMatrixToComponents(*worldMatrix.m, translation, rotation, scale);

				// delta rotation from the current rotation
				NuNu::math::Vector3 deltaRotation = Vector3(rotation) - transform->GetRotation();
				deltaRotation = transform->GetRotation() + deltaRotation;

				// set the new transform
				transform->SetScale(Vector3(scale));
				transform->SetRotation(Vector3(deltaRotation));
				transform->SetPosition(Vector3(translation));
			}
		}

		// repair the default render target
#if 0
		NuNu::graphics::GetDevice<NuNu::graphics::GraphicDevice_DX11>()->BindDefaultRenderTarget();
#endif

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void SceneWindow::OnEnable()
	{
	}

	void SceneWindow::OnDisable()
	{
	}

	void SceneWindow::OnDestroy()
	{
	}

}