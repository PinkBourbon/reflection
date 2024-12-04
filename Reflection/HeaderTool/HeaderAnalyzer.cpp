#include "HeaderAnalyzer.h"
#include <string>
#include <iostream>

#include "clang-c/Index.h"

// TODO: 나중에 리플렉션 관련된 심볼들을 모아서 정리를 해야 함.
#define REFL_CLASS_SYMBOL "REFL_CLASS"
#define AUTO_REFL_CLASS_SYMBOL "AUTO"
#define REFL_FUNC_SYMBOL "REFL_FUNC"
#define REFL_PROP_SYMBOL "REFL_PROP"
#define REFL_BODY_SYMBOL "REFL_BODY"

unsigned int GetLineNumber(CXCursor cursor)
{
	unsigned int line = 0;
	CXSourceLocation location = clang_getCursorLocation(cursor);
	clang_getSpellingLocation(location, NULL, &line, NULL, NULL);
	return line;
}

// 현재 반드시 매크로가 먼저 모두 검색된다는 가정하에 작성되었습니다.
// 매크로 정의 문이 다른 파일에 있을 경우 CXTranslationUnit_SingleFileParse 플래그를 적용하면
// 매크로 확장을 제대로 찾질 못한다.
CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, void* client_data)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);

	CXFile file;
	clang_getFileLocation(location, &file, NULL, NULL, NULL);
	if (clang_Location_isFromMainFile(location) == 0)
	{
		return CXChildVisit_Continue;
	}

	ReflectionInfoCollector* collector = static_cast<ReflectionInfoCollector*>(client_data);

	CXString name = clang_getCursorSpelling(cursor);
	const char* name_cstr = clang_getCString(name);
	CXCursorKind kind = clang_getCursorKind(cursor);
	const char* kindName = clang_getCString(clang_getCursorKindSpelling(kind));
	unsigned int line = GetLineNumber(cursor);

	const char* parentName_cstr = clang_getCString(clang_getCursorSpelling(parent));
	const char* parentKindName = clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(parent)));

	//printf("parse : %s, line : %d\n", name_cstr, line);
	if (line == 15)
	{
		int debug = 0;
	}

	switch (kind)
	{
		case CXCursor_MacroDefinition:
			break;
		case CXCursor_MacroExpansion:
		{
			if (strcmp(name_cstr, REFL_CLASS_SYMBOL) == 0)
			{
				collector->AddReflectionTarget(line);
				//collector->FindClassSymbol();

				CXToken* tokens;
				unsigned int num_tokens;
				CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
				CXSourceRange range = clang_getCursorExtent(cursor);
				clang_tokenize(tu, range, &tokens, &num_tokens);

				{
					CXString token_spelling = clang_getTokenSpelling(tu, tokens[0]);
					const char* token_str = clang_getCString(token_spelling);
					printf("매크로 : %s\n", token_str);
				}


				if (num_tokens > 1)
				{
					// TODO: 함수형 매크로일 경우 멤버들을 모두 리플렉션 처리하는지 확인이 필요.
					CXToken second_token = tokens[1];
					CXTokenKind token_kind = clang_getTokenKind(second_token);

					if (token_kind == CXToken_Punctuation)
					{
						CXString token_spelling = clang_getTokenSpelling(tu, second_token);
						const char* token_str = clang_getCString(token_spelling);

						if (strcmp(token_str, "(") == 0)
						{
							printf("  함수형 매크로입니다.\n");
							printf("  매개변수: ");

							for (unsigned int i = 2; i < num_tokens; i++)
							{
								CXToken token = tokens[i];
								CXString token_spelling = clang_getTokenSpelling(tu, token);
								const char* token_str = clang_getCString(token_spelling);
								
								if (strcmp(token_str, ")") == 0) break;
								if (strcmp(token_str, ",") != 0)
								{
									printf("%s ", token_str);

									if (strcmp(token_str, AUTO_REFL_CLASS_SYMBOL) == 0)
									{

									}
								}

								clang_disposeString(token_spelling);
							}
							printf("\n");
						}

						clang_disposeString(token_spelling);
					}
				}
			}
			else if (strcmp(name_cstr, REFL_FUNC_SYMBOL) == 0)
			{
				collector->AddReflectionTarget(line);
			}
			else if (strcmp(name_cstr, REFL_PROP_SYMBOL) == 0)
			{
				collector->AddReflectionTarget(line);
			}
			else if (strcmp(name_cstr, REFL_BODY_SYMBOL) == 0)
			{
				collector->AddReflBodyLine(line);
			}
		}
		break;
		case CXCursor_Namespace:
		{
			std::cout << "네임스페이스 : " << name_cstr << " (라인: " << line << ")" << std::endl;
			std::cout << "scope : " << collector->GetScope() << std::endl;
			collector->EnterScope(name_cstr, line);
			clang_visitChildren(cursor, visitNode, client_data);
			collector->ExitScope(name_cstr);
		}
		break;
		case CXCursor_ClassDecl:
		{
			// 리플렉션 대상인지 확인
			//thisPtr->GetReflectionTarget(line);
			std::cout << "클래스: " << name_cstr << " (라인: " << line << ")" << std::endl;
			std::cout << "scope : " << collector->GetScope() << std::endl;
			std::vector<std::tuple<std::string, unsigned int, bool>> bases;

			//if (!collector->IsReflectionTarget(line))
			//{
			//	break;
			//}

			collector->EnterScope(name_cstr, line);
			clang_visitChildren(cursor, visitNode, client_data);
			collector->ExitScope(name_cstr);
			std::cout << "클래스 자식 순회 끝" << std::endl;

			//clang_visitChildren(
			//	cursor,
			//	[](CXCursor c, CXCursor parent, CXClientData client_data)
			//	{
			//		if (clang_getCursorKind(c) == CXCursor_CXXBaseSpecifier)
			//		{
			//			CXType baseType = clang_getCursorType(clang_getCursorReferenced(c));
			//			CXString baseTypeSpelling = clang_getTypeSpelling(baseType);
			//			unsigned int baseLine = GetLineNumber(c);
			//			bool isVirtual = clang_isVirtualBase(c);
			//			static_cast<std::vector<std::tuple<std::string, unsigned int, bool>>*>(client_data)->push_back(
			//				std::make_tuple(clang_getCString(baseTypeSpelling), baseLine, isVirtual));
			//			clang_disposeString(baseTypeSpelling);
			//		}
			//		return CXChildVisit_Continue;
			//	},
			//	&bases);
			//if (!bases.empty())
			//{
			//	std::cout << "  상속: ";
			//	for (size_t i = 0; i < bases.size(); ++i) {
			//		if (i > 0) std::cout << ", ";
			//		std::cout << std::get<0>(bases[i]) << " (라인: " << std::get<1>(bases[i]) << ", "
			//			<< (std::get<2>(bases[i]) ? "가상" : "일반") << ")";
			//	}
			//	std::cout << std::endl;
			//}
		}
		break;

		case CXCursor_CXXMethod:
		{
			std::cout << "멤버 함수: " << name_cstr << " (라인: " << line << ")" << std::endl;

			//CXType functionType = clang_getCursorType(cursor);
			//unsigned int numArgs = clang_Cursor_getNumArguments(cursor);
			//for (unsigned int i = 0; i < numArgs; i++) {
			//	CXCursor argCursor = clang_Cursor_getArgument(cursor, i);
			//	CXString argName = clang_getCursorSpelling(argCursor);
			//	CXType argType = clang_getCursorType(argCursor);
			//	CXString argTypeName = clang_getTypeSpelling(argType);

			//	// 매개변수 이름과 타입 사용
			//	printf("매개변수 %d: %s %s\n", i, clang_getCString(argTypeName), clang_getCString(argName));

			//	clang_disposeString(argName);
			//	clang_disposeString(argTypeName);
			//}

			if (!collector->IsReflectionTarget(line))
			{
				break;
			}

			collector->AddMethod(name_cstr);
		}
		break;
		case CXCursor_FieldDecl:
		{
			std::cout << "멤버 변수: " << name_cstr << " (라인: " << line << ")" << std::endl;
			if (!collector->IsReflectionTarget(line))
			{
				break;
			}

			collector->AddField(name_cstr);
		}
		break;
		case CXCursor_CXXBaseSpecifier:
		{
			CXType baseType = clang_getCursorType(clang_getCursorReferenced(cursor));
			CXString baseTypeSpelling = clang_getTypeSpelling(baseType);
			unsigned int baseLine = GetLineNumber(cursor);
			bool isVirtual = clang_isVirtualBase(cursor);
			//static_cast<std::vector<std::tuple<std::string, unsigned int, bool>>*>(client_data)->push_back(
			//	std::make_tuple(clang_getCString(baseTypeSpelling), baseLine, isVirtual));

			std::cout << "  상속: ";
			std::cout << clang_getCString(baseTypeSpelling) << " (라인: " << baseLine << ", "
				<< (isVirtual ? "가상" : "일반") << ")";
			std::cout << std::endl;
			clang_disposeString(baseTypeSpelling);

			if (!collector->IsReflectionTarget(line))
			{
				break;
			}

			collector->AddBaseClass(clang_getCString(baseTypeSpelling));
		}
		break;
		default:
		{
			int i = 0;
		}
		break;
	}

	clang_disposeString(name);

	return CXChildVisit_Continue;
}

HeaderAnalyzer::HeaderAnalyzer()
{

}

bool HeaderAnalyzer::Analyze(std::filesystem::path headerPath, std::vector<std::string>* outReflectionCodes)
{
	const char* version = clang_getCString(clang_getClangVersion()); // 전체 버전 문자열

	CXIndex index = clang_createIndex(0, 0);

	unsigned int options = CXTranslationUnit_DetailedPreprocessingRecord
		| CXTranslationUnit_Incomplete
		//| CXTranslationUnit_SingleFileParse
		| CXTranslationUnit_SkipFunctionBodies
		| CXTranslationUnit_VisitImplicitAttributes
		| CXTranslationUnit_VisitImplicitAttributes;

	const char* args[] =
	{
		"-x",
		"c++",
		"-std=c++20"
	};

	int numArgs = sizeof(args) / sizeof(args[0]);

	// 번역 단위 생성
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		headerPath.string().c_str(),
		args, numArgs,
		NULL, 0,
		options);

	// 루트 커서 얻기
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	// AST 순회
	ReflectionInfoCollector reflGenerater;
	clang_visitChildren(cursor, visitNode, &reflGenerater);

	reflGenerater.GenerateReflectionCode(outReflectionCodes);

	// 리소스 해제
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return true;
}
