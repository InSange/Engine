# 서드파티 고지 (Third-Party Notices)

NuNuEngine은 아래의 서드파티 소프트웨어와 에셋을 포함하거나 사용합니다.
각 구성 요소의 저작권은 원저작자에게 있으며, 아래 명시된 라이선스 조건을 따릅니다.

이 저장소의 엔진 구조가 인프런 얌얌코딩 강의를 따라 구현한 클론코딩이라는 점은
[README의 「프로젝트 성격」](README.md) 항목을 참고하세요.

| 구성 요소 | 버전 | 라이선스 | 저장소 내 위치 |
|---|---|---|---|
| Dear ImGui | 1.92.7 | MIT | `Editor_Window/`, `Editor_SOURCE/` |
| ImGuizmo (ImSequencer, ImCurveEdit, ImGradient, ImZoomSlider 포함) | - | MIT | `Editor_Window/`, `Editor_SOURCE/` |
| stb (rect_pack, textedit, truetype) | - | Public Domain / MIT | `Editor_Window/imstb_*.h` |
| Open Asset Import Library (Assimp) | 5.2.5 | BSD 3-Clause | `External/Include/Assimp/`, `External/Library/Assimp/` |
| DirectXTex | 2.07 | MIT | `Vendor/DirectXTex/` |
| DirectXTK - SimpleMath | - | MIT | `NuNuEngine_CORE/Common/NMath.{h,cpp}` |
| D3D12 Helper Library (`d3dx12.h`) | - | MIT | `External/Include/d3dx12/` |
| JSON for Modern C++ (nlohmann/json) | 3.11.3 | MIT | `External/nlohmann/json.hpp` |
| FMOD Engine | 2.02.14 | 독자 라이선스 (아래 참고) | `External/Include/Fmod/`, `External/Library/Fmod/` |
| KayKit - Character Pack: Adventurers | - | CC0 1.0 | `Contents/Characters/` |
| KayKit - Platformer Pack | - | CC0 1.0 | `Contents/Packs/` |

---

## 개작(Modification) 고지

- **`NuNuEngine_CORE/Common/NMath.{h,cpp}`** 는 Microsoft의 **DirectXTK SimpleMath**를 기반으로 하며,
  네임스페이스를 `NuNu::math`로 변경하고 엔진 규약에 맞추어 수정했습니다.
  원본 저작권 주석은 파일 상단에 유지되어 있습니다.
- **`Editor_Window/GraphEditor.{h,cpp}`**, **`ImSequencer`**, **`ImCurveEdit`**, **`ImGradient`**, **`ImZoomSlider`** 는
  ImGuizmo 배포본에 포함된 파일이며, 일부는 엔진 통합을 위해 수정되었습니다.

---

## MIT License

아래 구성 요소에 적용됩니다.

- **Dear ImGui** - Copyright (c) 2014-2025 Omar Cornut
- **ImGuizmo / ImSequencer / ImCurveEdit / ImGradient / ImZoomSlider** - Copyright (c) 2016 Cedric Guillemet
- **DirectXTex** - Copyright (c) Microsoft Corporation
- **DirectXTK (SimpleMath)** - Copyright (c) Microsoft Corporation
- **D3D12 Helper Library (`d3dx12.h`)** - Copyright (c) Microsoft Corporation
- **JSON for Modern C++ (nlohmann/json)** - Copyright (c) 2013-2023 Niels Lohmann

```
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## BSD 3-Clause License - Open Asset Import Library (Assimp)

```
Copyright (c) 2006-2025, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```

---

## stb (Sean Barrett)

`imstb_rectpack.h`, `imstb_textedit.h`, `imstb_truetype.h` 는 Dear ImGui 배포본에 포함된
stb 라이브러리의 수정본입니다. stb는 Public Domain(Unlicense) 또는 MIT 중 선택 적용됩니다.

```
Copyright (c) 2017 Sean Barrett
```

---

## FMOD Engine - Firelight Technologies Pty Ltd

```
FMOD Engine
Copyright (c) Firelight Technologies Pty Ltd
https://www.fmod.com
```

FMOD Engine은 오픈소스가 아닌 독자 라이선스 소프트웨어입니다.
비상업적·학습 목적 사용 및 일정 수익 이하의 인디 개발자에게는 무상 라이선스가 제공되지만,
사용 시 제품 크레딧에 FMOD 및 Firelight Technologies 표기가 요구되며,
SDK 자체(헤더, 라이브러리, DLL)의 재배포는 별도 조건의 적용을 받습니다.

**확인 필요**
현재 이 저장소에는 FMOD SDK의 헤더(`External/Include/Fmod/`)와
바이너리(`External/Library/Fmod/{Debug,Release}/*.dll, *.lib`)가 포함되어 있습니다.
공개 저장소 재배포가 FMOD 라이선스 조건에 부합하는지 확인이 필요하며,
부합하지 않을 경우 해당 파일을 저장소에서 제외하고 빌드 시 개별 설치하도록 안내를 변경할 예정입니다.

본 프로젝트는 학습 목적의 비상업적 프로젝트입니다.

---

## KayKit - Kay Lousberg

```
KayKit Character Pack : Adventurers
KayKit Platformer Pack
Created by Kay Lousberg - https://kaylousberg.com
Licensed under CC0 1.0 Universal (Public Domain Dedication)
```

`Contents/Characters/` 및 `Contents/Packs/` 의 3D 모델·텍스처 에셋이 여기에 해당합니다.
CC0 라이선스로 저작권 표시 의무는 없으나, 원저작자에 대한 감사의 뜻으로 표기합니다.

---

## 문의

누락되었거나 잘못된 고지가 있다면 이슈로 알려주시면 즉시 반영하겠습니다.
