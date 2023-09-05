## 3.13 文本服务语言配置

文本服务语言配置，用来操作一个或多个文本服务的语言配置文件。

Interface						|Description
-|-
[ITfInputProcessorProfiles][1]	|文本服务语言配置，用来操作一个或多个文本服务的语言配置文件。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfInputProcessorProfiles.md

## 3.13.1 获取语言配置接口指针

```C++
HRESULT CTfInputProcessorProfile::CreateInstance()
{
    HRESULT	hr = CoCreateInstance(CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER,
        IID_ITfInputProcessorProfiles, (void**)&_pInputProcessorProfile);

    return hr;
}
```

## 3.13.2 获取当前活动语言的标识符

## 3.13.2 获取当前活动语言的标识符