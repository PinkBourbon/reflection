#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "AddIncludeGeneratedFile.h"
#include "CodeGenerator.h"

/// 테스트용 include
#include <windows.h>

#pragma comment(lib, "libclang.lib")

/*
std::string clangType[] =
{
	/ *000* /"Invalid",
	/ *001* /"UnexposedDecl",
	/ *002* /"StructDecl",
	/ *003* /"UnionDecl",
	/ *004* /"ClassDecl",
	/ *005* /"EnumDecl",
	/ *006* /"FieldDecl",
	/ *007* /"EnumConstantDecl",
	/ *008* /"FunctionDecl",
	/ *009* /"VarDecl",
	/ *010* /"ParmDecl",
	/ *011* /"ObjCInterfaceDecl",
	/ *012* /"ObjCCategoryDecl",
	/ *013* /"ObjCProtocolDecl",
	/ *014* /"ObjCPropertyDecl",
	/ *015* /"ObjCIvarDecl",
	/ *016* /"ObjCInstanceMethodDecl",
	/ *017* /"ObjCClassMethodDecl",
	/ *018* /"ObjCImplementationDecl",
	/ *019* /"ObjCCategoryImplDecl",
	/ *020* /"TypedefDecl",
	/ *021* /"CXXMethod",
	/ *022* /"Namespace",
	/ *023* /"LinkageSpec",
	/ *024* /"Constructor",
	/ *025* /"Destructor",
	/ *026* /"ConversionFunction",
	/ *027* /"TemplateTypeParameter",
	/ *028* /"NonTypeTemplateParameter",
	/ *029* /"TemplateTemplateParameter",
	/ *030* /"FunctionTemplate",
	/ *031* /"ClassTemplate",
	/ *032* /"ClassTemplatePartialSpecialization",
	/ *033* /"NamespaceAlias",
	/ *034* /"UsingDirective",
	/ *035* /"UsingDeclaration",
	/ *036* /"TypeAliasDecl",
	/ *037* /"ObjCSynthesizeDecl",
	/ *038* /"ObjCDynamicDecl",
	/ *039* /"CXXAccessSpecifier",
	/ *040* /"FirstRef",
	/ *041* /"ObjCProtocolRef",
	/ *042* /"ObjCClassRef",
	/ *043* /"TypeRef",
	/ *044* /"CXXBaseSpecifier",
	/ *045* /"TemplateRef",
	/ *046* /"NamespaceRef",
	/ *047* /"MemberRef",
	/ *048* /"LabelRef",
	/ *049* /"OverloadedDeclRef",
	/ *050* /"VariableRef"
	/ *060* /
	/ *061* /
	/ *062* /
	/ *063* /
	/ *064* /
	/ *065* /
	/ *066* /
	/ *067* /
	/ *068* /
	/ *069* /
	/ *070* /
	/ *071* /
	/ *072* /
	/ *073* /
	/ *074* /
	/ *075* /
	/ *076* /
	/ *077* /
	/ *078* /
	/ *079* /
	/ *080* /
	/ *081* /
	/ *082* /
	/ *083* /
	/ *084* /
	/ *085* /
	/ *086* /
	/ *087* /
	/ *088* /
	/ *089* /
	/ *090* /
	/ *091* /
	/ *092* /
	/ *093* /
	/ *094* /
	/ *095* /
	/ *096* /
	/ *097* /
	/ *098* /
	/ *099* /


};*/

int main(int argc, char* argv[])
{
	BOOL ret = AllocConsole();
	if (ret == 0)
	{
		DWORD errorCode = GetLastError();
	}
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

	printf("argc num : %d", argc);

	std::string path = argv[1];
	std::string projectPath = argv[2];
	std::string relativePath = argv[3];

	std::cout << path << std::endl;
	std::cout << projectPath << std::endl;
	std::cout << relativePath << std::endl;

	CodeGenerator generator;
	
	if (generator.Generate(path, relativePath))
	{
		AddIncludeGeneratedFile(path);
	}

	return 0;
}
