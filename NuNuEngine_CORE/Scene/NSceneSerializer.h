#pragma once
#include <string>

namespace NuNu
{
	class Scene;

	class SceneSerializer
	{
	public:
		// 씬 → JSON 파일 저장
		static bool Save(Scene* scene, const std::string& path);

		// JSON 파일 → 씬 오브젝트 복원 (activeScene에 추가)
		static bool Load(const std::string& path);
	};
}
