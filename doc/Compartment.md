## 3.13 公共缓冲池

公共缓冲池，用来保存输入法状态开关。

>作者并不真正理解Compartment的含义，这里也有可能是COM的套间技术，总之有个隔离区域保存输入法的配置。

## 3.13.1 公共缓冲池管理器

Interface				|Description
-|-
[ITfCompartmentMgr][1]	|公共缓冲池管理器，用于管理客户端之间的共享数据。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfCompartmentMgr.md

```C++
HRESULT CCompartment::_GetCompartment(_Outptr_ ITfCompartment **ppCompartment)
{
    HRESULT hr = S_OK;
    ITfCompartmentMgr* pCompartmentMgr = nullptr;

    hr = _punk->QueryInterface(IID_ITfCompartmentMgr, (void **)&pCompartmentMgr);
    if (SUCCEEDED(hr))
    {
        hr = pCompartmentMgr->GetCompartment(_guidCompartment, ppCompartment);
        pCompartmentMgr->Release();
    }

    return hr;
}
```

## 3.13.2 操作公共缓冲池

Interface			|Description
-|-
[ITfCompartment][2]	|公共缓冲池，用于获取和设置公共缓冲池中的数据。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfCompartment.md

```C++
HRESULT CCompartment::_SetCompartmentBOOL(_In_ BOOL flag)
{
    HRESULT hr = S_OK;
    ITfCompartment* pCompartment = nullptr;

    hr = _GetCompartment(&pCompartment);
    if (SUCCEEDED(hr))
    {
        VARIANT var;
        var.vt = VT_I4;
        var.lVal = flag;
        hr = pCompartment->SetValue(_tfClientId, &var);
        pCompartment->Release();
    }

    return hr;
}
```

## 3.13.3 公共缓冲池事件接收器

Interface						|Description
-|-
[ITfCompartmentEventSink][3]	|公共缓冲池事件接收器。

[3]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfCompartmentEventSink.md

安装ITfCompartmentEventSink公共缓冲池事件接收器后，当公共缓冲池发生改变时，ITfCompartmentEventSink::OnChange()函数将被调用。

```C++
HRESULT CCompartmentEventSink::_Advise(_In_ IUnknown *punk, _In_ REFGUID guidCompartment)
{
    HRESULT hr = S_OK;
    ITfCompartmentMgr* pCompartmentMgr = nullptr;
    ITfSource* pSource = nullptr;

    hr = punk->QueryInterface(IID_ITfCompartmentMgr, (void **)&pCompartmentMgr);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = pCompartmentMgr->GetCompartment(guidCompartment, &_pCompartment);
    if (SUCCEEDED(hr))
    {
        hr = _pCompartment->QueryInterface(IID_ITfSource, (void **)&pSource);
        if (SUCCEEDED(hr))
        {
            hr = pSource->AdviseSink(IID_ITfCompartmentEventSink, this, &_dwCookie);
            pSource->Release();
        }
    }

    pCompartmentMgr->Release();

    return hr;
}
```