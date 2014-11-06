#include "ximage/ximage.h"
#include <memory>
#include <vector>
#include <iostream>

using std::tr1::shared_ptr;

namespace
{
    void changeFrame(shared_ptr<CxImage> image, CxImage::CXTEXTINFO* textInfo, SIZE sz)
    {
        std::cout << "frame width: " << image->GetWidth()
                  << ", frame height: " << image->GetHeight() << std::endl;
        
        long x = image->GetWidth() - sz.cx;
        x += sz.cx / 2;

        long y = sz.cy;

        image->DrawStringEx(NULL, x, y, textInfo, true);
    }
}

namespace ximage
{
    bool convert2Icon(const WCHAR* sourceImg, const WCHAR* savePath)
    {
        shared_ptr<CxImage> image(new CxImage(sourceImg, CXIMAGE_FORMAT_ICO));
        if (!image->IsValid())
        {
            std::cout << "open img failed\n";
            return false;
        }

        std::vector<shared_ptr<CxImage>> frames(image->GetNumFrames());

        for (int frameIndex = 0; frameIndex < frames.size(); ++frameIndex)
        {
            frames[frameIndex].reset(new CxImage);
            frames[frameIndex]->SetFrame(frameIndex);
            bool loadSucc = frames[frameIndex]->Load(sourceImg, CXIMAGE_FORMAT_ICO);

            if (!loadSucc)
            {
                std::cout << "load frame " << frameIndex << " failed\n";
                return false;
            }
        }

        // ������������Ҫ��ÿ��frame���л�ͼ
        DWORD fcolor = RGB(255, 255, 255) | (255 << 24);
        DWORD bcolor = RGB(255, 0, 0) | (255 << 24);
        CxImage::CXTEXTINFO text = { 0 };
        image->InitTextInfo(&text);
        text.lfont.lfCharSet = DEFAULT_CHARSET;
        //text.lfont.lfWeight = FW_BOLD;
        text.lfont.lfHeight = -12;
        text.fcolor = fcolor;
        text.opaque = TRUE;
        text.bcolor = bcolor;
        text.b_round = 20;
        _tcscpy(text.lfont.lfFaceName, _T("������"));

        _tcscpy(text.text, _T("11"));


        for (int frameIndex = 0; frameIndex < frames.size(); ++frameIndex)
        {
            text.lfont.lfHeight = -(frames[frameIndex]->GetWidth() / 7 * 2);

            SIZE sz = { 0 };
            long ret = image->GetDrawStringExSize(NULL, &text, sz);

            if (!ret)
            {
                std::cout << "get string size failed\n";
                return false;
            }

            std::cout << "font size: " << sz.cx << ", " << sz.cy << std::endl;

            changeFrame(frames[frameIndex], &text, sz);
        }

        FILE* hFile = _wfopen(savePath,L"w+b");

        if (!hFile)
        {
            std::cout << "open savefile failed.\n";
            return false;
        }

        std::vector<CxImage*> iconframe(frames.size());

        for (int frameIndex = 0; frameIndex < iconframe.size(); ++frameIndex)
        {
            iconframe[frameIndex] = frames[frameIndex].get();
        }

        CxImage icoTmp;
        icoTmp.Encode(hFile, &iconframe[0], iconframe.size(), CXIMAGE_FORMAT_ICO);

        fclose(hFile);

        return true;
    }
}