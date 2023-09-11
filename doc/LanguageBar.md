## 3.15 语言栏

语言栏，设置输入法状态开关。

## 3.15.1 语言栏项管理器

Interface				|Description
-|-
[ITfLangBarItemMgr][1]	|语言栏项管理器，用于管理语言栏中的项。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/LanguageBar/ITfLangBarItemMgr.md

```C++
HRESULT CLangBarItemButton::_AddItem(_In_ ITfThreadMgr *pThreadMgr)
{
    HRESULT hr = S_OK;
    ITfLangBarItemMgr* pLangBarItemMgr = nullptr;

    if (_isAddedToLanguageBar)
    {
        return S_OK;
    }

    hr = pThreadMgr->QueryInterface(IID_ITfLangBarItemMgr, (void **)&pLangBarItemMgr);
    if (SUCCEEDED(hr))
    {
        hr = pLangBarItemMgr->AddItem(this);
        if (SUCCEEDED(hr))
        {
            _isAddedToLanguageBar = TRUE;
        }
        pLangBarItemMgr->Release();
    }

    return hr;
}
```

## 3.15.2 语言栏项信息

Interface			|Description
-|-
[ITfLangBarItem][2]	|语言栏项信息，由语言栏管理器用来获取语言栏项的详细信息。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfLangBarItem.md

```C++
STDAPI CLangBarItemButton::GetInfo(_Out_ TF_LANGBARITEMINFO *pInfo)
{
    _tfLangBarItemInfo.dwStyle |= TF_LBI_STYLE_SHOWNINTRAY;
    *pInfo = _tfLangBarItemInfo;
    return S_OK;
}
```

## 3.15.3 语言栏项消息接收器

Interface				|Description
-|-
[ITfLangBarItemSink][3]	|语言栏项消息接收器，用于将语言栏项中的更改通知语言栏。

[3]: https://github.com/ChineseInputMethod/Interface/blob/master/LanguageBar/ITfLangBarItemSink.md

当调用ITfLangBarItemMgr::AddItem()方法，将语言栏项（输入法）安装到语言栏后，语言栏（系统）立即调用ITfSource::AdviseSink()方法，将ITfLangBarItemSink语言栏项消息接收器安装到语言栏项（输入法）中。

```C++
STDAPI CLangBarItemButton::AdviseSink(__RPC__in REFIID riid, __RPC__in_opt IUnknown *punk, __RPC__out DWORD *pdwCookie)
{
    // We allow only ITfLangBarItemSink interface.
    if (!IsEqualIID(IID_ITfLangBarItemSink, riid))
    {
        return CONNECT_E_CANNOTCONNECT;
    }

    // We support only one sink once.
    if (_pLangBarItemSink != nullptr)
    {
        return CONNECT_E_ADVISELIMIT;
    }

    // Query the ITfLangBarItemSink interface and store it into _pLangBarItemSink.
    if (punk == nullptr)
    {
        return E_INVALIDARG;
    }
    if (punk->QueryInterface(IID_ITfLangBarItemSink, (void **)&_pLangBarItemSink) != S_OK)
    {
        _pLangBarItemSink = nullptr;
        return E_NOINTERFACE;
    }

    // return our cookie.
    *pdwCookie = _cookie;
    return S_OK;
}
```

当更改输入法状态时，输入法调用ITfLangBarItemSink::OnUpdate()方法，通知语言栏（系统）。

## 3.15.4 语言栏按钮项信息

Interface					|Description
-|-
[ITfLangBarItemButton][4]	|语言栏按钮项信息，由语言栏管理器用来获取语言栏上的按钮项信息。

[4]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfLangBarItemButton.md

```C++
STDAPI CLangBarItemButton::OnClick(TfLBIClick click, POINT pt, _In_ const RECT *prcArea)
{
    click;pt;
    prcArea;

    BOOL isOn = FALSE;

    _pCompartment->_GetCompartmentBOOL(isOn);
    _pCompartment->_SetCompartmentBOOL(isOn ? FALSE : TRUE);

    return S_OK;
}
```