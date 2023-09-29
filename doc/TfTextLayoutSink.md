## 3.28 文本布局消息接收器

文本布局消息接收器，通过响应布局更改消息，实现光标跟随。

## 3.28.1 安装文本布局消息接收器

Interface				|Description
-|-
[ITfTextLayoutSink][1]	|当上下文视图的布局发生更改时，会收到通知。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfTextLayoutSink.md

```C++
HRESULT CTfTextLayoutSink::_AdviseTextLayoutSink()
{
    HRESULT hr = S_OK;
    ITfSource* pSource = nullptr;

    hr = _pContextDocument->QueryInterface(IID_ITfSource, (void **)&pSource);
    if (FAILED(hr))
    {
        return hr;
    }

    hr = pSource->AdviseSink(IID_ITfTextLayoutSink, (ITfTextLayoutSink *)this, &_dwCookieTextLayoutSink);
    if (FAILED(hr))
    {
        pSource->Release();
        return hr;
    }

    pSource->Release();

    return hr;
}
```

## 3.28.2 文本布局更改事件

当上下文视图的布局发生更改时，OnLayoutChange()函数会被调用。

在TSF处理逻辑上，合成中的编码被写入上下文，就会触发文本布局更改事件。输入法响应事件，申请编辑会话，更改候选窗口内容和位置。<br>
但是，第三方输入法一般自己管理输入编码，不会将输入编码实际写入上下文，所以实现光标跟随，需要一点技巧。具体如何实现，在下一章讲解。

```C++
STDAPI CTfTextLayoutSink::OnLayoutChange(_In_ ITfContext *pContext, TfLayoutCode lcode, _In_ ITfContextView *pContextView)
{
    // we're interested in only document context.
    if (pContext != _pContextDocument)
    {
        return S_OK;
    }

    switch (lcode)
    {
    case TF_LC_CHANGE:
        {
            CGetTextExtentEditSession* pEditSession = nullptr;
            pEditSession = new (std::nothrow) CGetTextExtentEditSession(_pTextService, pContext, pContextView, _pRangeComposition, this);
            if (nullptr != (pEditSession))
            {
                HRESULT hr = S_OK;
                pContext->RequestEditSession(_pTextService->_GetClientId(), pEditSession, TF_ES_SYNC | TF_ES_READ, &hr);

                pEditSession->Release();
            }
        }
        break;

    case TF_LC_DESTROY:
        _LayoutDestroyNotification();
        break;

    }
    return S_OK;
}
```

## 3.28.3 获取屏幕坐标

Interface			|Description
-|-
[ITfContextView][2]	|上下文视图对象，由客户端(应用程序或文本服务)用来获取上下文视图的信息。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfContextView.md

```C++
HRESULT CTfTextLayoutSink::_GetTextExt(_Out_ RECT *lpRect)
{
    HRESULT hr = S_OK;
    BOOL isClipped = TRUE;
    ITfContextView* pContextView = nullptr;

    hr = _pContextDocument->GetActiveView(&pContextView);
    if (FAILED(hr))
    {
        return hr;
    }

    if (FAILED(hr = pContextView->GetTextExt(_tfEditCookie, _pRangeComposition, lpRect, &isClipped)))
    {
        return hr;
    }

    pContextView->Release();

    return S_OK;
}
```