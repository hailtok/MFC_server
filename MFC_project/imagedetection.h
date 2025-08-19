#pragma once
#include <afxwin.h>
#include<cmath>
#include<vector>
std::vector<std::pair<int, int> > DetectFeaturesFromBitmap(HBITMAP hBitmap);
HBITMAP LoadBitmapFromMemory(const std::vector<char>& bmpData);