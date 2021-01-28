#pragma once

#define SE_LOG(n)   {std::cout << "INFO: " << n << std::endl; OutputDebugStringA(n); OutputDebugStringA("\n");}
#define SE_ERROR(n) {std::cout << "ERROR:" << n << std::endl; OutputDebugStringA(n); OutputDebugStringA("\n");}