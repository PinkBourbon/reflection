#include "HeaderAnalyzer.h"
#include "clang-c/Index.h"
#include <string>

enum CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	CXString name = clang_getCursorSpelling(cursor);
	CXCursorKind kind = clang_getCursorKind(cursor);

	const char* kindName = clang_getCString(clang_getCursorKindSpelling(kind));
	printf("Cursor Kind: %s\n", kindName);
	printf("Cursor Name: %s\n", clang_getCString(name));

	if (kind == CXCursor_MacroExpansion)
	{
		CXString name = clang_getCursorSpelling(cursor);
		printf("매크로 이름: %s\n", clang_getCString(name));

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

	printf("--------------------------------\n");

	clang_disposeString(name);
	return CXChildVisit_Recurse;
}

HeaderAnalyzer::HeaderAnalyzer(std::filesystem::path headerPath)
{
	CXIndex index = clang_createIndex(0, 0);

	unsigned int options = CXTranslationUnit_DetailedPreprocessingRecord
		| CXTranslationUnit_Incomplete
		| CXTranslationUnit_SkipFunctionBodies
		| CXTranslationUnit_VisitImplicitAttributes
		| CXTranslationUnit_SingleFileParse
		| CXTranslationUnit_VisitImplicitAttributes;

	// 번역 단위 생성
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		headerPath.string().c_str(),
		NULL, 0,
		NULL, 0,
		options);

	// 루트 커서 얻기
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	// AST 순회
	clang_visitChildren(cursor, visitNode, NULL);

	// 리소스 해제
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
}
