## 3.32 阴影窗口

阴影窗口，为窗口添加阴影。<br>

## 3.32.1 创建窗口

候选窗口类将this做为参数调用_pShadowWnd->_Create()函数。

```C++
BOOL CCandidateWindow::_CreateBackGroundShadowWindow()
{
    _pShadowWnd = new (std::nothrow) CShadowWindow(this);
    if (_pShadowWnd == nullptr)
    {
        return FALSE;
    }

    if (!_pShadowWnd->_Create(Global::AtomShadowWindow,
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_LAYERED,
        WS_DISABLED | WS_POPUP, this))
    {
        _DeleteShadowWnd();
        return FALSE;
    }

    return TRUE;
}
```

阴影窗口类调用窗口基类。

```C++
BOOL CShadowWindow::_Create(ATOM atom, DWORD dwExStyle, DWORD dwStyle, _In_opt_ CBaseWindow *pParent, int wndWidth, int wndHeight)
{
    if (!CBaseWindow::_Create(atom, dwExStyle, dwStyle, pParent, wndWidth, wndHeight))
    {
        return FALSE;
    }

    return _Initialize();
}
```

在窗口基类中调用API函数创建窗口。