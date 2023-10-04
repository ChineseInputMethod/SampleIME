## 3.27 候选窗口控制器

候选窗口控制器，控制候选窗口的显示。

## 3.27.1 候选窗口创建过程

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
[ITfUIElementMgr][1]	|UI元素管理器，文本服务调用ITfUIElementMgr，向应用程序查询UI可见性。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfUIElementMgr.md

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

## 3.27.3 创建候选窗口

输入法在MakeCandidateWindow()创建候选窗口类，开始创建候选窗口。

```C++
HRESULT CCandidateListUIPresenter::MakeCandidateWindow(_In_ ITfContext *pContextDocument, _In_ UINT wndWidth)
{
    HRESULT hr = S_OK;

    if (nullptr != _pCandidateWnd)
    {
        return hr;
    }

    _pCandidateWnd = new (std::nothrow) CCandidateWindow(_CandWndCallback, this, _pIndexRange, _pTextService->_IsStoreAppMode());
    if (_pCandidateWnd == nullptr)
    {
        hr = E_OUTOFMEMORY;
        goto Exit;
    }

    HWND parentWndHandle = nullptr;
    ITfContextView* pView = nullptr;
    if (SUCCEEDED(pContextDocument->GetActiveView(&pView)))
    {
        pView->GetWnd(&parentWndHandle);
    }

    if (!_pCandidateWnd->_Create(_atom, wndWidth, parentWndHandle))
    {
        hr = E_OUTOFMEMORY;
        goto Exit;
    }

Exit:
    return hr;
}
```

## 3.27.4 显示候选列表

当用户按下编码键，输入法生成候选列表，创建候选窗口，然后将候选列表添加到候选窗口中。

首先，调用AddCandidateToCandidateListUI()函数，将候选列表复制一份副本给候选窗口。
然后，调用SetPageIndexWithScrollInfo()函数，设置候选页索引。
最后，调用_pCandidateWnd->_InvalidateRect()函数，失效工作区。

```C++
void CCandidateListUIPresenter::_SetText(_In_ CSampleImeArray<CCandidateListItem> *pCandidateList, BOOL isAddFindKeyCode)
{
    AddCandidateToCandidateListUI(pCandidateList, isAddFindKeyCode);

    SetPageIndexWithScrollInfo(pCandidateList);

    if (_isShowMode)
    {
        _pCandidateWnd->_InvalidateRect();
    }
    else
    {
        _updatedFlags = TF_CLUIE_COUNT       |
            TF_CLUIE_SELECTION   |
            TF_CLUIE_STRING      |
            TF_CLUIE_PAGEINDEX   |
            TF_CLUIE_CURRENTPAGE;
        _UpdateUIElement();
    }
}
```