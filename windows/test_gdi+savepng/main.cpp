#include <Windows.h>
#include <GdiPlus.h>

#include <cassert>

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

    Gdiplus::Font font(L"ËÎÌו", 15, 0, Gdiplus::UnitPixel, NULL);
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

}

int main()
{
    GdiPlusIniter gdiplusIniter;

    convertTagIcon(L"1234", L"D:\\icons\\Logo.png", L"d:\\icons\\convert.png");
}