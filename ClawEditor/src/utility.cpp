#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#endif

#include "utility.h"
#include "renderer.h"

std::string FileDialogOpenFile(const char* filter)
{
#ifdef _WIN32
	OPENFILENAME ofn = {0};
	TCHAR szFile[260] = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = rWindow->getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}
	return std::string();
#endif
}

std::string FileDialogSaveFile(const char* filter)
{
#ifdef _WIN32
	OPENFILENAME ofn = {0};
	TCHAR szFile[260] = {0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = rWindow->getSystemHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}
	return std::string();
#endif
}