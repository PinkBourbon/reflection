#pragma once
#include "Type.h"
#include "Method.h"
#include "Property.h"
#include "BaseRegister.h"
//#include "TypeMacro.h"
//#include "MethodMacro.h"
//#include "PropertyMacro.h"


#define REFL_CLASS()
#define REFL_CLASS(X)

/// REFL_CLASS 에 넣을 매개변수에 해당하는 매크로들
#define NOT_AUTO

/// 리플렉션 대상 클래스에 리플렉션을 적용할 대상을 지정하는 매크로
#define REFL_FUNC
#define REFL_PROP


#ifndef CURRENT_FILE_PATH
#define CURRENT_FILE_PATH
#endif

#define REFL_CONCAT_IMPL(a, b, c, d) a##b##c##d
#define REFL_TO_STRING(x) #x
#define REFL_MACRO_CONCAT(x, y) REFL_CONCAT_IMPL(x, _, y, _FLT_REFL)
#define REFL_BODY REFL_MACRO_CONCAT(CURRENT_FILE_PATH, __LINE__)
