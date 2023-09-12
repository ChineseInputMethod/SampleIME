## 3.10 扩展功能提供者

扩展功能提供者，提供各种函数对象。

## 3.10.1 安装扩展功能提供者

Interface				|Description
-|-
[ITfSourceSingle][1]	|单一事件安装器，与ITfSource的不同之处在于，ITfSourceSingle安装的事件接收器一次仅支持一个事件接收器。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfSourceSingle.md

```C++
    if (SUCCEEDED(_pThreadMgr->QueryInterface(IID_ITfSourceSingle, (void **)&pSourceSingle)))
    {
        IUnknown* punk = nullptr;
        if (SUCCEEDED(QueryInterface(IID_IUnknown, (void **)&punk)))
        {
            if (SUCCEEDED(pSourceSingle->AdviseSingleSink(_tfClientId, IID_ITfFunctionProvider, punk)))
            {
...
            }
            punk->Release();
        }
        pSourceSingle->Release();
    }
```

## 3.10.2 获取指定的函数对象

安装扩展功能提供者后，TSF管理器会调用CSampleIME::GetFunction()方法，获取指定的函数对象。

Interface					|Description
-|-
[ITfFunctionProvider][1]	|扩展功能提供者，提供各种函数对象。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfThreadFocusSink.md

当前工程实现为：提供搜索集成服务。

```C++
HRESULT CSampleIME::GetFunction(__RPC__in REFGUID rguid, __RPC__in REFIID riid, __RPC__deref_out_opt IUnknown **ppunk)
{
    HRESULT hr = E_NOINTERFACE;

    if ((IsEqualGUID(rguid, GUID_NULL)) 
        && (IsEqualGUID(riid, __uuidof(ITfFnSearchCandidateProvider))))
    {
        hr = _pITfFnSearchCandidateProvider->QueryInterface(riid, (void**)ppunk);
    }
    else if (IsEqualGUID(rguid, GUID_NULL))
    {
        hr = QueryInterface(riid, (void **)ppunk);
    }

    return hr;
}
```

## 3.10.3 搜索集成

>https://learn.microsoft.com/zh-cn/windows/apps/design/input/input-method-editor-requirements#ime-search-integration

在Windows 8中安装激活本工程，在磁贴页面点击搜索，即可观测上面链接描述的搜索集成。