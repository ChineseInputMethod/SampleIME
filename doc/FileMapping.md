## 3.17 文件映射

文件映射，将码表文件创建为文件映射对象。

## 3.17.1 创建文件映射对象

当前工程的码表文件是UTF-16 LE格式。
所以Unicode编码的字符，在码表文件中存储为低端在前。
例如编码为4E00的汉字“一”，在码表文件中存储为004E。

```C++
BOOL CFileMapping::SetupReadBuffer()
{
    if (_fileSize > sizeof(WCHAR))
    {
        //
        // Read file in file mapping
        //文件映射对象
        _fileMappingHandle = CreateFileMapping(_fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
        if (_fileMappingHandle)
        {
            _pMapBuffer = (const WCHAR *)MapViewOfFile(_fileMappingHandle, FILE_MAP_READ, 0, 0, 0);
            if (_pMapBuffer)
            {
                if (IsTextUnicode(_pMapBuffer, (int)_fileSize, NULL))
                {
                    _pReadBuffer = (WCHAR*)_pMapBuffer;

                    // skip Unicode byte order mark
                    if (*((WCHAR*)_pMapBuffer) == Global::UnicodeByteOrderMark)
                    {
                        _pReadBuffer++;
                        _fileSize--;
                    }
                    return TRUE;
                }

                UnmapViewOfFile(_pReadBuffer);
                _pReadBuffer = nullptr;
            }

            CloseHandle(_fileMappingHandle);
            _fileMappingHandle = nullptr;
        }
    }

    return FALSE;
}
```