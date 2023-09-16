## 3.16 文件

文件，打开码表文件。

## 3.16.1 打开码表文件

输入法的权限继承首次激活输入法的应用程序的权限。当第一个激活输入法的应用程序不具备读写输入法码表文件的能力时，输入法就无法正常工作。
所以，一般输入法都有一个自启动服务，在后台为输入法提供码表文件。或者，将码表文件放入一个无权限限制的文件夹中。

当前工程，以只读方式打开码表文件，所以在大多数环境中都能正常工作。

```C++
BOOL CFile::CreateFile(_In_ PCWSTR pFileName, DWORD desiredAccess,
    DWORD creationDisposition,
    DWORD sharedMode, _Inout_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD flagsAndAttributes, _Inout_opt_ HANDLE templateFileHandle)
{
    size_t fullPathLen = wcslen(pFileName);
    if (!_pFileName)
    {
        _pFileName = new (std::nothrow) WCHAR[ fullPathLen + 1 ];
    }
    if (!_pFileName)
    {
        return FALSE;
    }

    StringCchCopyN(_pFileName, fullPathLen + 1, pFileName, fullPathLen);

    _fileHandle = ::CreateFile(pFileName, desiredAccess, sharedMode,
        lpSecurityAttributes, creationDisposition, flagsAndAttributes, templateFileHandle);

    if (_fileHandle == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    _fileSize = ::GetFileSize(_fileHandle, NULL);

    return TRUE;
}
```