#include <Windows.h>
#include <filesystem>
namespace fs = std::filesystem;
DWORD WINAPI thread_func(void* hModule) {
	char path[MAX_PATH];
	GetModuleFileName(reinterpret_cast<HMODULE>(hModule), path, sizeof(path));

	fs::path p = path;
    p = p.parent_path() / "Pyw32";

    SetDllDirectoryA(p.string().c_str());

    auto py38 = p / "python38.dll";
    auto pyedit = p.parent_path() / "pyedit.dll";
    LoadLibraryA(py38.string().c_str());
    LoadLibraryA(pyedit.string().c_str());

    return true;
}
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0x100, thread_func, handle, 0, 0);
    }
    return TRUE;
}