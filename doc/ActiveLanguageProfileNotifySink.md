## 3.9 语言配置激活消息接收器

语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。

## 3.9.1 安装语言配置激活消息接收器

```C++
    if (_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource) != S_OK)
    {
        return ret;
    }

    if (pSource->AdviseSink(IID_ITfActiveLanguageProfileNotifySink, (ITfActiveLanguageProfileNotifySink *)this, &_activeLanguageProfileNotifySinkCookie) != S_OK)
    {
        _activeLanguageProfileNotifySinkCookie = TF_INVALID_COOKIE;
        goto Exit;
    }
```

## 3.9.2 响应输入法激活事件

ITfActiveLanguageProfileNotifySink接口，用于响应输入法激活、停用消息。

Interface								|Description
-|-
[ITfActiveLanguageProfileNotifySink][1]	|语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/Application/ITfActiveLanguageProfileNotifySink.md