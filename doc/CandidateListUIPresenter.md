## 3.27 候选窗口控制器

候选窗口控制器，控制候选窗口的显示。

## 3.27.1 创建候选窗口

当用户按下编码键，输入法生成候选列表后，调用CCandidateListUIPresenter::_StartCandidateList()函数，创建候选窗口。

首先，调用_StartLayout()函数，安装文本布局消息接收器。
然后，调用BeginUIElement()函数，确定是否应显示文本服务的UI。
接着，调用MakeCandidateWindow()函数，创建候选窗口。
然后，调用Show()函数，显示候选窗口。
接着，调用_GetTextExt()函数，获取屏幕坐标。
最后，调用_LayoutChangeNotification()函数，实现光标跟随。

```C++
HRESULT CCandidateListUIPresenter::_StartCandidateList(TfClientId tfClientId, _In_ ITfDocumentMgr *pDocumentMgr, _In_ ITfContext *pContextDocument, TfEditCookie ec, _In_ ITfRange *pRangeComposition, UINT wndWidth)
{
	pDocumentMgr;tfClientId;

    HRESULT hr = E_FAIL;

    if (FAILED(_StartLayout(pContextDocument, ec, pRangeComposition)))
    {
        goto Exit;
    }

    BeginUIElement();

    hr = MakeCandidateWindow(pContextDocument, wndWidth);
    if (FAILED(hr))
    {
        goto Exit;
    }

    Show(_isShowMode);

    RECT rcTextExt;
    if (SUCCEEDED(_GetTextExt(&rcTextExt)))
    {
        _LayoutChangeNotification(&rcTextExt);
    }

Exit:
    if (FAILED(hr))
    {
        _EndCandidateList();
    }
    return hr;
}
```

## 3.27.2 无界面模式

Interface				|Description
-|-
[ITfTextLayoutSink][1]	|当上下文视图的布局发生更改时，会收到通知。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfTextLayoutSink.md

```C++
HRESULT CCandidateListUIPresenter::BeginUIElement()
{
    HRESULT hr = S_OK;

    ITfThreadMgr* pThreadMgr = _pTextService->_GetThreadMgr();
    if (nullptr ==pThreadMgr)
    {
        hr = E_FAIL;
        goto Exit;
    }

    ITfUIElementMgr* pUIElementMgr = nullptr;
    hr = pThreadMgr->QueryInterface(IID_ITfUIElementMgr, (void **)&pUIElementMgr);
    if (hr == S_OK)
    {
        pUIElementMgr->BeginUIElement(this, &_isShowMode, &_uiElementId);
        pUIElementMgr->Release();
    }

Exit:
    return hr;
}
```