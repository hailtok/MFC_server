#include"imagedetection.h"
std::vector<std::pair<int, int> > DetectFeaturesFromBitmap(HBITMAP hBitmap) {
    std::vector<std::pair<int, int> > features;
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    int width = bmp.bmWidth;
    int height = bmp.bmHeight;
    int rowSize = ((width * 3 + 3) / 4) * 4;
    std::vector<uint8_t> buffer(rowSize * height);
    GetBitmapBits(hBitmap, buffer.size(), buffer.data());
    // constrct gray image
    std::vector<std::vector<uint8_t>> gray(height, std::vector<uint8_t>(width));
    for (int y = 0; y < height; ++y) {
        uint8_t* row = buffer.data() + y * rowSize;
        for (int x = 0; x < width; ++x) {
            int i = x * 3;
            uint8_t b = row[i], g = row[i + 1], r = row[i + 2];
            gray[height - 1 - y][x] = (r + g + b) / 3;
        }
    }
    // edge detection
    int threshold = 30;
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            int gx = gray[y][x + 1] - gray[y][x - 1];
            int gy = gray[y + 1][x] - gray[y - 1][x];
            int grad = std::sqrt(gx * gx + gy * gy);
            if (grad > threshold) {
                features.push_back({ x, y });
            }
        }
    }
    return features;
}
HBITMAP LoadBitmapFromMemory(const std::vector<char>& bmpData) {
    if (bmpData.size() < sizeof(BITMAPFILEHEADER)) return nullptr;

    const BITMAPFILEHEADER* pFileHeader = reinterpret_cast<const BITMAPFILEHEADER*>(bmpData.data());

    // 檢查 BMP 檔標誌是否正確（應該是 'BM'）
    if (pFileHeader->bfType != 0x4D42) return nullptr;

    const BITMAPINFO* pBitmapInfo = reinterpret_cast<const BITMAPINFO*>(bmpData.data() + sizeof(BITMAPFILEHEADER));
    const void* pPixelData = bmpData.data() + pFileHeader->bfOffBits;

    HDC hdc = GetDC(NULL);
    HBITMAP hBitmap = CreateDIBitmap(
        hdc,
        &pBitmapInfo->bmiHeader,
        CBM_INIT,
        pPixelData,
        pBitmapInfo,
        DIB_RGB_COLORS
    );
    ReleaseDC(NULL, hdc);

    return hBitmap;
}