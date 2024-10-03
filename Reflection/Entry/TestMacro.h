#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define FILENAME __FILE__
#define LINE_NUMBER TOSTRING(__LINE__)

#define CREATE_FILE_LINE CURRENT_FILE "_" LINE_NUMBER


#define MYNUM 1
#define MYNUM 2
#define MYNUM 3

///GENERATEDFILE 내용 만들어보자
#undef CURRENT_FILE
#define CURRENT_FILE "TestMacro_h"
