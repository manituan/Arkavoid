#include "D3DCore_impl.h"

void* D3DCore_Impl::File_Load(LPCWSTR filename, DWORD *size)
{
	void *ptr;
	HANDLE hF;

	// Load from file
	hF = CreateFile(File_MakePath(filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);
	if(hF == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	DWORD uncompressed_size = GetFileSize(hF, NULL);
	ptr = malloc(uncompressed_size);
	if(!ptr)
	{
		CloseHandle(hF);
		return 0;
	}
	if(ReadFile(hF, ptr, uncompressed_size, &uncompressed_size, NULL ) == 0)
	{
		CloseHandle(hF);
		free(ptr);
		return 0;
	}
	
	CloseHandle(hF);
	if(size) *size=uncompressed_size;
	return ptr;
}


void D3DCore_Impl::File_Free(void *res)
{
	if(res) free(res);
}


WCHAR* D3DCore_Impl::File_MakePath(LPCWSTR filename)
{
	int i;

	if(!filename)
		wcscpy(szTmpFilename, szAppPath);
	else if(filename[0]==L'\\' || filename[0]==L'/' || filename[1]==L':')
		wcscpy(szTmpFilename, filename);
	else
	{
		wcscpy(szTmpFilename, szAppPath);
		if(filename) wcscat(szTmpFilename, filename);
	}

	for(i=0; szTmpFilename[i]; i++) { if(szTmpFilename[i]==L'/') szTmpFilename[i]=L'\\'; }
	return szTmpFilename;
}

WCHAR* D3DCore_Impl::File_EnumFiles(LPCWSTR wildcard)
{
	if(wildcard)
	{
		if(hSearch) { FindClose(hSearch); hSearch=0; }
		hSearch=FindFirstFile(File_MakePath(wildcard), &SearchData);
		if(hSearch==INVALID_HANDLE_VALUE) { hSearch=0; return 0; }

		if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
		else return File_EnumFiles();
	}
	else
	{
		if(!hSearch) return 0;
		for(;;)
		{
			if(!FindNextFile(hSearch, &SearchData))	{ FindClose(hSearch); hSearch=0; return 0; }
			if(!(SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return SearchData.cFileName;
		}
	}
}

WCHAR* D3DCore_Impl::File_EnumFolders(LPCWSTR wildcard)
{
	if(wildcard)
	{
		if(hSearch) { FindClose(hSearch); hSearch=0; }
		hSearch=FindFirstFile(File_MakePath(wildcard), &SearchData);
		if(hSearch==INVALID_HANDLE_VALUE) { hSearch=0; return 0; }

		if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
		   wcscmp(SearchData.cFileName,L".") && wcscmp(SearchData.cFileName,L".."))
				return SearchData.cFileName;
		else return File_EnumFolders();
	}
	else
	{
		if(!hSearch) return 0;
		for(;;)
		{
			if(!FindNextFile(hSearch, &SearchData))	{ FindClose(hSearch); hSearch=0; return 0; }
			if((SearchData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			   wcscmp(SearchData.cFileName,L".") && wcscmp(SearchData.cFileName,L".."))
					return SearchData.cFileName;
		}
	}
}
