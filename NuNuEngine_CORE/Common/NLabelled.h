#pragma once
#include "CommonInclude.h"
using namespace NuNu::math;

namespace NuNu
{
	class Labelled
	{
	public:
		Labelled();
		virtual ~Labelled();

		void SetName(const std::wstring& name) { mName = name; }
		std::wstring& GetName() { return mName; }

	private:
		std::wstring mName;
	};
}