// dllmain.h : 模块类的声明。

class CShellToolsModule : public CAtlDllModuleT< CShellToolsModule >
{
public :
	DECLARE_LIBID(LIBID_ShellToolsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SHELLTOOLS, "{30B3B5CE-FCFC-4451-A038-4F5E8C406718}")
};

extern class CShellToolsModule _AtlModule;
