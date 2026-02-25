#pragma once
#include "NEntity.h"

namespace NuNu
{
	class Resource abstract : public Entity
	{
	public:
		Resource(enums::eResourceType type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetPath() { return mPath; }
		void SetPath(const std::wstring& path) { mPath = path; }

	private:
		const enums::eResourceType mType;
		std::wstring mPath;
	};
}

// 이미지, 사운드, 데이터 등등등