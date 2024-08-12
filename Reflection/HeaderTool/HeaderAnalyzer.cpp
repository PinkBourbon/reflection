﻿#include "HeaderAnalyzer.h"
#include <string>
#include <iostream>

#define REFL_CLASS_SYMBOL "REFL_CLASS"
#define REFL_FUNC_SYMBOL "REFL_FUNC"
#define REFL_PROP_SYMBOL "REFL_PROP"

unsigned int GetLineNumber(CXCursor cursor)
{
	unsigned int line = 0;
	CXSourceLocation location = clang_getCursorLocation(cursor);
	clang_getSpellingLocation(location, NULL, &line, NULL, NULL);
	return line;
}

// 현재 반드시 매크로가 먼저 모두 검색된다는 가정하에 작성되었습니다.
CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, void* client_data)
{

	{
		CXCursorKind cursorKind = clang_getCursorKind(cursor);
		CXType cursorType = clang_getCursorType(cursor);
		std::string typeString = clang_getCString(clang_getTypeSpelling(cursorType));

		CXFile file;
		unsigned int line, column;
		CXSourceLocation cursorLocation = clang_getCursorLocation(cursor);
		clang_getFileLocation(cursorLocation, &file, &line, &column, NULL);

		if (clang_Location_isFromMainFile(cursorLocation))
		{
			switch (cursorKind)
			{
				case CXCursor_ClassDecl:
				{
					std::cout << typeString << std::endl;
				}
			}
		}
	}

	HeaderAnalyzer* thisPtr = static_cast<HeaderAnalyzer*>(client_data);

	CXString name = clang_getCursorSpelling(cursor);
	const char* name_cstr = clang_getCString(name);
	CXCursorKind kind = clang_getCursorKind(cursor);
	const char* kindName = clang_getCString(clang_getCursorKindSpelling(kind));
	unsigned int line = GetLineNumber(cursor);

	const char* parentName_cstr = clang_getCString(clang_getCursorSpelling(parent));
	const char* parentKindName = clang_getCString(clang_getCursorKindSpelling(clang_getCursorKind(parent)));

	std::string typeString = clang_getCString(clang_getTypeSpelling(clang_getCursorType(cursor)));


	std::string tempName = name_cstr;
	if(tempName.find("Example") != tempName.npos)
	{
		int i = 0;
	}

	switch (kind)
	{
		case CXCursor_MacroDefinition:
			break;
		case CXCursor_MacroExpansion:
		{
			if (strcmp(name_cstr, REFL_CLASS_SYMBOL) == 0)
			{
				thisPtr->FindClassSymbol();

				CXToken* tokens;
				unsigned int num_tokens;
				CXTranslationUnit tu = clang_Cursor_getTranslationUnit(cursor);
				CXSourceRange range = clang_getCursorExtent(cursor);
				clang_tokenize(tu, range, &tokens, &num_tokens);

				if (num_tokens > 1)
				{
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

			}
			else if (strcmp(name_cstr, REFL_PROP_SYMBOL) == 0)
			{

			}
		}
		break;
		case CXCursor_ClassDecl:
		{
			std::cout << "클래스: " << name_cstr << " (라인: " << line << ")" << std::endl;
			std::vector<std::pair<std::string, unsigned int>> bases;
			clang_visitChildren(
				cursor
				, [](CXCursor c, CXCursor parent, CXClientData client_data)
				{
					if (clang_getCursorKind(c) == CXCursor_CXXBaseSpecifier)
					{
						CXString baseSpelling = clang_getCursorSpelling(clang_getCursorReferenced(c));
						unsigned int baseLine = GetLineNumber(c);
						static_cast<std::vector<std::pair<std::string, unsigned int>>*>(client_data)->push_back(
							{ clang_getCString(baseSpelling), baseLine });
						clang_disposeString(baseSpelling);
					}
					return CXChildVisit_Continue;
				}
				, &bases);

			if (!bases.empty())
			{
				std::cout << "  상속: ";
				for (size_t i = 0; i < bases.size(); ++i) {
					if (i > 0) std::cout << ", ";
					std::cout << bases[i].first << " (라인: " << bases[i].second << ")";
				}
				std::cout << std::endl;
			}
		}
		break;

		case CXCursor_CXXMethod:
		{
			std::cout << "멤버 함수: " << name_cstr << " (라인: " << line << ")" << std::endl;
		}
		break;
		case CXCursor_FieldDecl:
		{
			std::cout << "멤버 변수: " << name_cstr << " (라인: " << line << ")" << std::endl;
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
	: _functions()
	, _members()
	, _isfindClassSymbol(false)
{

}

bool HeaderAnalyzer::Analyze(std::filesystem::path headerPath, std::vector<char>* buffer)
{
	const char* version = clang_getCString(clang_getClangVersion()); // 전체 버전 문자열

	CXIndex index = clang_createIndex(0, 0);

	unsigned int options = CXTranslationUnit_DetailedPreprocessingRecord
		| CXTranslationUnit_SingleFileParse
		| CXTranslationUnit_Incomplete
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
	clang_visitChildren(cursor, visitNode, this);

	// 리소스 해제
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return true;
}

void HeaderAnalyzer::AddFunction()
{
	std::cout << "AddFunction" << std::endl;
}

void HeaderAnalyzer::AddMember()
{
	std::cout << "AddMember" << std::endl;
}
