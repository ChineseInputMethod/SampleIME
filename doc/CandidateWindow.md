## 3.30 候选窗口

候选窗口，在主窗口中显示候选列表。

## 3.30.1 候选窗口结构

首先，调用_CreateMainWindow()函数，创建主窗口。
然后，调用_CreateBackGroundShadowWindow()函数，创建背景阴影窗口。
接着，调用_CreateVScrollWindow()函数，创建滚动条窗口。
最后，调用_ResizeWindow()函数，调整窗口大小。

```C++
BOOL CCandidateWindow::_Create(ATOM atom, _In_ UINT wndWidth, _In_opt_ HWND parentWndHandle)
{
    BOOL ret = FALSE;
    _wndWidth = wndWidth;

    ret = _CreateMainWindow(atom, parentWndHandle);
    if (FALSE == ret)
    {
        goto Exit;
    }

    ret = _CreateBackGroundShadowWindow();
    if (FALSE == ret)
    {
        goto Exit;
    }

    ret = _CreateVScrollWindow();
    if (FALSE == ret)
    {
        goto Exit;
    }

    _ResizeWindow();

Exit:
    return TRUE;
}
```

## 3.30.2 创建主窗口

首先保存当前候选窗口对象，做为主窗口对象。候选子窗口通过CBaseWindow::_GetTopmostUIWnd()函数，获取候选主窗口对象。<br>
然后调用CBaseWindow::_Create()函数，创建候选主窗口。主窗口用于显示候选列表，并做为子窗口的所有者窗口。

```C++
BOOL CCandidateWindow::_CreateMainWindow(ATOM atom, _In_opt_ HWND parentWndHandle)
{
    _SetUIWnd(this);

    if (!CBaseWindow::_Create(atom,
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        WS_BORDER | WS_POPUP,
        NULL, 0, 0, parentWndHandle))
    {
        return FALSE;
    }

    return TRUE;
}
```

## 3.30.3 创建滚动条窗口

```C++

```

## 3.30.4 调整窗口大小