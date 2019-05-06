#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <tchar.h> 
#include <strsafe.h>
#include "fs_windows_builder.h"
#include "runtime_exception.h"
#pragma comment(lib, "User32.lib")
#define BUFSIZE 4096

static std::string joinPath(std::string one, std::string & another);

FsFile *FsWindowsBuilder::buildFile(std::string & filePath) {
	auto delimiter_pos = filePath.find_last_of('\\');
	if (delimiter_pos == std::string::npos || delimiter_pos + 1 > filePath.length()) {
		throw RuntimeException("Couldn't build file, filePath is wrong");
	}
	auto fileName = filePath;
	fileName.erase(0, delimiter_pos + 1);
	return new FsFile(fileName, filePath);
}

//https://docs.microsoft.com/ru-ru/windows/desktop/FileIO/listing-the-files-in-a-directory

FsFolder *FsWindowsBuilder::buildFolder(std::string & folderPath, const char * folderName) {
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;
	StringCchLength(folderPath.c_str(), MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 3)) {
		throw RuntimeException("Directory path is too long.");
	}
	StringCchCopy(szDir, MAX_PATH, folderPath.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
	// Find the first file in the directory.
	hFind = FindFirstFile(szDir, &ffd);
	if (INVALID_HANDLE_VALUE == hFind) {
		throw RuntimeException("Couldn't find first file");
	}
	auto folder = new FsFolder(folderName);
	// List all the files in the directory with some info about them.
	do {
		if (strcmp(ffd.cFileName, ".") == 0 || strcmp(ffd.cFileName, "..") == 0) {
			continue;
		} else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			std::string name = ffd.cFileName;
			std::string currentPath = joinPath(folderPath, name);
			folder->add(buildFolder(currentPath, ffd.cFileName));
		} else {
			std::string name = ffd.cFileName;
			std::string currentPath = joinPath(folderPath, name);
			DWORD  retval = 0;
			BOOL   success;
			TCHAR  buffer[BUFSIZE] = TEXT("");
			TCHAR  buf[BUFSIZE] = TEXT("");
			TCHAR** lppPart = { NULL };
			// Retrieve the full path name for a file. 
			// The file does not need to exist.
			retval = GetFullPathName(currentPath.c_str(),
				BUFSIZE,
				buffer,
				lppPart
			);
			if (retval == 0) {
				// Handle an error condition.
				throw RuntimeException("GetFullPathName failed");
			}
			std::string fullPath = buffer;
			folder->add(buildFile(fullPath));
		}
	} while (FindNextFile(hFind, &ffd) != 0);
	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) {
		throw RuntimeException("Error while listining files in a directory");
	}
	FindClose(hFind);
	root = folder;
	return folder;
}

std::string joinPath(std::string one, std::string & another) {
	if (one[one.length() - 1] != '\\') {
		one.append("\\");
	}
	return one + another;
}


#endif