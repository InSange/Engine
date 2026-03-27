#include "NInputLayout.h"

namespace NuNu::graphics
{
	InputLayout::InputLayout()
	{
	}

	InputLayout::~InputLayout()
	{
	}

//	void InputLayout::CreateInputLayout(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength)
//	{
//#if 0
//		if (!(GetDevice<GraphicDevice_DX11>()->CreateInputLayout(layout, vertexCount
//			, pShaderBytecodeWithInputSignature
//			, BytecodeLength
//			, mInputLayout.GetAddressOf())))
//			assert(nullptr && "Create input layout failed!");
//#endif
//	}

	void InputLayout::Bind() const
	{
#if 0
		if (mInputLayout)
			GetDevice<GraphicDevice_DX11>()->BindInputLayout(mInputLayout.Get());
#endif
	}
}
