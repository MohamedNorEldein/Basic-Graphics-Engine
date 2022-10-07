
#pragma once
#include <Graphics.h>

bool mndCHECK(HRESULT);
class Error:std::exception{};

#define CHECK(hr) if(!mndCHECK((hr))){	printf("Line Number %s->%s:%d\n", __FILE__, __FUNCTION__, __LINE__);}
#define ERROR printf("Line Number %s->%s:%d\n", __FILE__, __FUNCTION__, __LINE__);  throw Error();