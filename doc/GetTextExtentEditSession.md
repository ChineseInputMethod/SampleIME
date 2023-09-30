## 3.29 文本布局会话

文本布局会话，处理文本布局消息。

## 3.29.1 处理文本布局消息

在文本布局会话中，输入法获取合成位置，然后在合成下方显示候选窗口。

```C++
STDAPI CGetTextExtentEditSession::DoEditSession(TfEditCookie ec)
{
    RECT rc = {0, 0, 0, 0};
    BOOL isClipped = TRUE;

    if (SUCCEEDED(_pContextView->GetTextExt(ec, _pRangeComposition, &rc, &isClipped)))
    {
        _pTfTextLayoutSink->_LayoutChangeNotification(&rc);
    }

    return S_OK;
}
```