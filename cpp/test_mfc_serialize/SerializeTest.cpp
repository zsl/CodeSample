// SerializeTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SerializeTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;


class CSeriablize : public CObject{
    DECLARE_SERIAL(CSeriablize)

public:
    int data;

    void Serialize(CArchive& ar);
};

IMPLEMENT_SERIAL(CSeriablize, CObject, 1)

void CSeriablize::Serialize(CArchive& ar)
{
    CObject::Serialize(ar);
    if (ar.IsStoring()){
        ar << data;
    }
    else{
        ar >> data;
    }
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.

        CSeriablize *o1 = new CSeriablize;
        o1->data = 1;
        CSeriablize *o2 = o1;

        TCHAR *strName = _T("f:\\filedata.dat");
        CFile file(strName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive);
        CArchive saveArchive(&file, CArchive::store | CArchive::bNoFlushOnDelete);

        saveArchive << o1 << o2;

        delete o1;
        o1 = o2 = NULL;

        saveArchive.Close();
        file.Close();

        CFile fileLoad(strName, CFile::modeRead | CFile::shareDenyWrite);
        CArchive loadArchive(&fileLoad, CArchive::load | CArchive::bNoFlushOnDelete);

        loadArchive >> o1 >> o2;

        loadArchive.Close();
        fileLoad.Close();

	}

	return nRetCode;
}
