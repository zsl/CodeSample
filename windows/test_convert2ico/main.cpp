#include <Windows.h>
#include <GdiPlus.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include "IconFormat.h"

#pragma comment(lib, "gdiplus.lib")

class GdiPlusIniter{
public:
    GdiPlusIniter(){
        Gdiplus::GdiplusStartupInput StartupInput;  
        GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL); 
    }

    ~GdiPlusIniter(){
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
    }
private:
    ULONG_PTR m_gdiplusToken;
};

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
    UINT  num = 0;          // number of image encoders
    UINT  size = 0;         // size of the image encoder array in bytes

    Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

    Gdiplus::GetImageEncodersSize(&num, &size);
    if(size == 0)
        return -1;  // Failure

    pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    if(pImageCodecInfo == NULL)
        return -1;  // Failure

    Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

    for(UINT j = 0; j < num; ++j)
    {
        if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
        {
            *pClsid = pImageCodecInfo[j].Clsid;
            free(pImageCodecInfo);
            return j;  // Success
        }    
    }

    free(pImageCodecInfo);
    return -1;  // Failure
}

void convertTagIcon(const WCHAR* str, const WCHAR* sourceImg, const WCHAR* savePath)
{
    assert(sourceImg && savePath);

    Gdiplus::Image* img = Gdiplus::Image::FromFile(sourceImg);


    Gdiplus::Graphics g(img);

    Gdiplus::Font font(L"宋体", 15, 0, Gdiplus::UnitPixel, NULL);
    Gdiplus::StringFormat format;
    format.SetAlignment(Gdiplus::StringAlignmentCenter);
    format.SetLineAlignment(Gdiplus::StringAlignmentCenter);

    Gdiplus::RectF boundRc;
    g.MeasureString(str, -1, &font, Gdiplus::PointF(0, 0), &boundRc);

    float tagsize = boundRc.GetBottom() > boundRc.GetRight() ? boundRc.GetBottom() : boundRc.GetRight();

    Gdiplus::RectF rcDest(img->GetWidth() - tagsize, 0, tagsize, tagsize);

    Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 0, 0));
    g.FillEllipse(&redBrush, rcDest);

    Gdiplus::SolidBrush whiteBrush(Gdiplus::Color(255, 255, 255));
    g.DrawString(str, -1, &font, rcDest, &format, &whiteBrush);

    CLSID pngClsid;
    GetEncoderClsid(L"image/png", &pngClsid);

    img->Save(savePath, &pngClsid, NULL);

    delete img;
    img = NULL;
}

WORD getBitCountFromePixelFormat(Gdiplus::PixelFormat pixelFormat)
{
    switch (pixelFormat)
    {
        case PixelFormat1bppIndexed:
            return 1;
        case PixelFormat4bppIndexed:
            return 4;
        case PixelFormat8bppIndexed:
            return 8;
        case PixelFormat16bppARGB1555:
        case PixelFormat16bppGrayScale:
        case PixelFormat16bppRGB555:
        case PixelFormat16bppRGB565:
            return 16;
        case PixelFormat24bppRGB:
            return 24;
        case PixelFormat32bppARGB:
        case PixelFormat32bppCMYK:
        case PixelFormat32bppPARGB:
        case PixelFormat32bppRGB:
            return 32;
        case PixelFormat64bppARGB:
        case PixelFormat64bppPARGB:
            return 64;
        default:
            return 0;
    }
}

bool convert2Icon(const WCHAR* sourceImg, const WCHAR* savePath)
{
    // ico 格式请参考：http://msdn.microsoft.com/en-us/library/ms997538.aspx
    assert(sourceImg && savePath);

    // 非标准的构造函数
    std::fstream ofs(savePath, std::ios::out|std::ios::binary);

    if (!ofs)
    {
        std::cout << "打开文件失败\n";

        return false;
    }

    Gdiplus::Bitmap* img = Gdiplus::Bitmap::FromFile(sourceImg);
    
    if (!img)
    {
        ofs.close();
        return false;
    }

    UINT frameDimensionCount = img->GetFrameDimensionsCount();
    std::cout << "frameDimensionsCount: " << frameDimensionCount << std::endl;

    UINT frameCount = 1;
    if (frameDimensionCount > 0)
    {
        // 获取Frame数
        std::vector<GUID> dimensionId(frameDimensionCount);
        img->GetFrameDimensionsList(&dimensionId[0], frameDimensionCount);
        frameCount = img->GetFrameCount(&dimensionId[0]);
    }

    std::cout << "frameCount: " << frameCount << std::endl;

    img->RotateFlip(Gdiplus::RotateNoneFlipY);

    // IconDir
    ICONDIR iconDir = { 0 };
    iconDir.idType  = 1; 
    iconDir.idCount = 1;

    ofs.write((const char*)&iconDir, sizeof iconDir);

    // IconEntry
    int paletteSize = img->GetPaletteSize();
    Gdiplus::ColorPalette *palette = (Gdiplus::ColorPalette *)malloc(paletteSize);
    img->GetPalette(palette, paletteSize);

    BITMAPINFOHEADER bmpInfoHeader = { 0 };
    bmpInfoHeader.biSize      = sizeof BITMAPINFOHEADER;
    bmpInfoHeader.biWidth     = img->GetWidth();
    bmpInfoHeader.biHeight    = 2 * img->GetHeight();
    bmpInfoHeader.biPlanes    = 1;
    bmpInfoHeader.biBitCount  = 32;//getBitCountFromePixelFormat(img->GetPixelFormat());

    DWORD xorMaskStride = ((bmpInfoHeader.biBitCount * bmpInfoHeader.biWidth + 31)/ 32 * 4);

    bmpInfoHeader.biSizeImage = xorMaskStride * img->GetHeight();

    std::cout << "width:" << img->GetWidth() << ", height:" << img->GetHeight() << std::endl;
    std::cout << "bitcount: " << bmpInfoHeader.biBitCount << std::endl;
    std::cout << "sizeimage: " << bmpInfoHeader.biSizeImage << std::endl;
    std::cout << "palette size: " << palette->Count << std::endl;


    DWORD xorMaskSize = bmpInfoHeader.biSizeImage;
    DWORD andMaskStride = ((1 * bmpInfoHeader.biWidth + 31)/ 32 * 4);
    DWORD andMaskSize = andMaskStride * bmpInfoHeader.biHeight;


    ICONDIRENTRY iconEntry = { 0 };
    iconEntry.bWidth       = (BYTE)img->GetWidth();
    iconEntry.bHeight      = (BYTE)img->GetHeight();
    iconEntry.bColorCount  = palette->Count;
    iconEntry.wPlanes      = bmpInfoHeader.biPlanes;
    iconEntry.wBitCount    = bmpInfoHeader.biBitCount;
    iconEntry.dwBytesInRes = sizeof(BITMAPINFOHEADER)
                           + iconEntry.bColorCount * sizeof(Gdiplus::ARGB)
                           + xorMaskSize
                           + andMaskSize;

    iconEntry.dwImageOffset = sizeof ICONDIR + 1 * sizeof ICONDIRENTRY;

    ofs.write((const char*)&iconEntry, sizeof iconEntry);

    // IconImage
    // BMPINFOHEADER
    ofs.write((const char*)&bmpInfoHeader, sizeof bmpInfoHeader);

    // REGQUAD
    if (palette->Count != 0)
    {
        // write REGQUAD
    }

    // Set XOR and AND mask
    std::vector<char> vecXorMask(xorMaskSize);
    std::vector<char> vecAndMask(andMaskSize);

    char* xorMask = &vecXorMask[0];
    char* andMask = &vecAndMask[0];

    Gdiplus::BitmapData bmpData;
    Gdiplus::Rect rect(0, 0, img->GetWidth(), img->GetHeight());
    img->LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpData);

    memcpy(xorMask, bmpData.Scan0, xorMaskSize);

    img->UnlockBits(&bmpData);


    for (int y = 0; y < bmpData.Height; ++y)
    {
        for (int x = 0; x < bmpData.Width; ++x)
        {
            if (xorMask[xorMaskStride * y + x * 4 + 3] == 0)
            {
                andMask[andMaskStride * y + x / 8] |= ( 0x80 >> (x % 8) );
            }
        }
    }

    ofs.write(xorMask, xorMaskSize);
    ofs.write(andMask, andMaskSize);

    free(palette);
    delete img;
    img = NULL;
}

namespace ximage
{
    bool convert2Icon(const WCHAR* sourceImg, const WCHAR* savePath);
}

int main()
{
    GdiPlusIniter gdiplusIniter;

    ximage::convert2Icon(L"D:\\icons\\convert.png", L"d:\\icons\\icon1.ico");
}