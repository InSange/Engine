#pragma once

#define NOMINMAX
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <memory>

#include <vector>
#include <algorithm>
#include <ranges>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <bitset>
#include <functional>
#include <ostream>
#include <sstream>
#include <filesystem>
#include <mutex>
#include <thread>

#include <mmsystem.h>
#include <dinput.h>

#include "Common/NEnum.h"
#include "Common/NMath.h"

#define BIT(x) (1 << x)

#define CAST_UINT(value) static_cast<UINT>(value)
#define CAST_FLOAT(value) static_cast<float>(value)

#define SAFE_DELETE(p) if (p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if (p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if (p) { p->Release(); p = nullptr; }

#define N_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }