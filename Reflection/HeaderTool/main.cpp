#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "clang-c/Index.h"

/// 테스트용 include
#include <windows.h>

#pragma comment(lib, "libclang.lib")

std::string clangType[] =
{
	/*000*/"Invalid",
	/*001*/"UnexposedDecl",
	/*002*/"StructDecl",
	/*003*/"UnionDecl",
	/*004*/"ClassDecl",
	/*005*/"EnumDecl",
	/*006*/"FieldDecl",
	/*007*/"EnumConstantDecl",
	/*008*/"FunctionDecl",
	/*009*/"VarDecl",
	/*010*/"ParmDecl",
	/*011*/"ObjCInterfaceDecl",
	/*012*/"ObjCCategoryDecl",
	/*013*/"ObjCProtocolDecl",
	/*014*/"ObjCPropertyDecl",
	/*015*/"ObjCIvarDecl",
	/*016*/"ObjCInstanceMethodDecl",
	/*017*/"ObjCClassMethodDecl",
	/*018*/"ObjCImplementationDecl",
	/*019*/"ObjCCategoryImplDecl",
	/*020*/"TypedefDecl",
	/*021*/"CXXMethod",
	/*022*/"Namespace",
	/*023*/"LinkageSpec",
	/*024*/"Constructor",
	/*025*/"Destructor",
	/*026*/"ConversionFunction",
	/*027*/"TemplateTypeParameter",
	/*028*/"NonTypeTemplateParameter",
	/*029*/"TemplateTemplateParameter",
	/*030*/"FunctionTemplate",
	/*031*/"ClassTemplate",
	/*032*/"ClassTemplatePartialSpecialization",
	/*033*/"NamespaceAlias",
	/*034*/"UsingDirective",
	/*035*/"UsingDeclaration",
	/*036*/"TypeAliasDecl",
	/*037*/"ObjCSynthesizeDecl",
	/*038*/"ObjCDynamicDecl",
	/*039*/"CXXAccessSpecifier",
	/*040*/"FirstRef",
	/*041*/"ObjCProtocolRef",
	/*042*/"ObjCClassRef",
	/*043*/"TypeRef",
	/*044*/"CXXBaseSpecifier",
	/*045*/"TemplateRef",
	/*046*/"NamespaceRef",
	/*047*/"MemberRef",
	/*048*/"LabelRef",
	/*049*/"OverloadedDeclRef",
	/*050*/"VariableRef"
	/*060*/
	/*061*/
	/*062*/
	/*063*/
	/*064*/
	/*065*/
	/*066*/
	/*067*/
	/*068*/
	/*069*/
	/*070*/
	/*071*/
	/*072*/
	/*073*/
	/*074*/
	/*075*/
	/*076*/
	/*077*/
	/*078*/
	/*079*/
	/*080*/
	/*081*/
	/*082*/
	/*083*/
	/*084*/
	/*085*/
	/*086*/
	/*087*/
	/*088*/
	/*089*/
	/*090*/
	/*091*/
	/*092*/
	/*093*/
	/*094*/
	/*095*/
	/*096*/
	/*097*/
	/*098*/
	/*099*/


};

enum CXChildVisitResult visitNode(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
	CXString name = clang_getCursorSpelling(cursor);
	CXCursorKind kind = clang_getCursorKind(cursor);

	const char* kindName = clang_getCString(clang_getCursorKindSpelling(kind));
	printf("Cursor Kind: %s\n", kindName);
	printf("Cursor Name: %s\n", clang_getCString(name));

	if(kind == CXCursor_MacroExpansion)
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

int main(int argc, char* argv[])
{
	AllocConsole();
	HWND _consoleHwnd = GetConsoleWindow();
	ShowWindow(_consoleHwnd, SW_SHOW);

	FILE* console = freopen("CONOUT$", "w", stdout);
	std::cout.clear();
	ShowWindow(_consoleHwnd, SW_SHOW);

	if(argc < 2)
	{
		printf("Usage: HeaderTool.exe <header file>\n");
		return 1;
	}

	std::string path = argv[1];
	std::string generatedPath = path.substr(0, path.find_last_of('.')) + "_generated.h";

	std::cout << path << std::endl;
	std::cout << generatedPath << std::endl;

	return 0;

	CXIndex index = clang_createIndex(0, 0);
	const char* filename = "example.h";  // 분석할 헤더 파일 이름

	unsigned int options = CXTranslationUnit_DetailedPreprocessingRecord
		| CXTranslationUnit_Incomplete
		| CXTranslationUnit_SkipFunctionBodies
		| CXTranslationUnit_VisitImplicitAttributes
		| CXTranslationUnit_SingleFileParse
		| CXTranslationUnit_VisitImplicitAttributes;

	// 번역 단위 생성
	CXTranslationUnit unit = clang_parseTranslationUnit(
		index,
		filename,
		NULL, 0,
		NULL, 0,
		options);

	if (unit == NULL) {
		printf("헤더 파일을 파싱할 수 없습니다.\n");
		return 1;
	}

	// 루트 커서 얻기
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	// AST 순회
	clang_visitChildren(cursor, visitNode, NULL);

	// 리소스 해제
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return 0;
}
