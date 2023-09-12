## 3.12 文本服务语言配置

文本服务语言配置，注册表中输入法的属性设置。

Interface						|Description
-|-
[ITfInputProcessorProfiles][1]	|文本服务语言配置，用来操作一个或多个文本服务的语言配置文件。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfInputProcessorProfiles.md

## 3.12.1 获取语言配置接口指针

```C++
HRESULT CTfInputProcessorProfile::CreateInstance()
{
    HRESULT	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER,
        IID_ITfInputProcessorProfiles, (void**)&_pInputProcessorProfile);

    return hr;
}
```

## 3.12.2 获取当前活动语言的标识符

```C++
HRESULT CTfInputProcessorProfile::GetCurrentLanguage(_Out_ LANGID *plangid)
{
    if (_pInputProcessorProfile)
    {
        return _pInputProcessorProfile->GetCurrentLanguage(plangid);
    }
    else
    {
        return E_FAIL;
    }
}
```

## 3.12.3 获取当前活动语言的默认配置文件

```C++
HRESULT CTfInputProcessorProfile::GetDefaultLanguageProfile(LANGID langid, REFGUID catid, _Out_ CLSID *pclsid, _Out_ GUID *pguidProfile)
{
    if (_pInputProcessorProfile)
    {
        return _pInputProcessorProfile->GetDefaultLanguageProfile(langid, catid, pclsid, pguidProfile);
    }
    else
    {
        return E_FAIL;
    }
}
```