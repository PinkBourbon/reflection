#pragma once
#include <filesystem>

/// <summary>
/// 헤더 파일에 #include header_generated.h 를 추가합니다.
/// 이미 추가되어있다면 추가하지 않습니다.
/// </summary>
/// <param name="headerPath">헤더파일 경로</param>
void AddIncludeGeneratedFile(std::filesystem::path headerPath);
