## 3.31 窗口基类

窗口基类，为派生类调用API函数。<br>
候选窗口类均派生自窗口基类，窗口基类为子窗口类提供辅助函数。

## 3.31.1 创建窗口

在CBaseWindow::_Create()函数中，窗口基类为子窗口类调用API函数，创建窗口。

```C++
BOOL CBaseWindow::_Create(ATOM atom, DWORD dwExStyle, DWORD dwStyle, _In_opt_ CBaseWindow *pParentWnd, int wndWidth, int wndHeight, _In_opt_ HWND parentWndHandle)
{
    _pParentWnd = pParentWnd;//候选窗口此参数为NULL，子窗口此参数为候选窗口。也就是说，候选窗口的父窗口为文档窗口，子窗口的所有者窗口为候选窗口

    if (atom != 0)
    {
        // create real window

        _wndHandle = CreateWindowEx(dwExStyle,//扩展窗口样式
            (LPCTSTR)atom,//类原子
            NULL,//窗口名称
            dwStyle,//窗口样式
            0, 0,//窗口位置
            wndWidth, wndHeight,//窗口尺寸
            _pParentWnd ? _pParentWnd->_GetWnd() : parentWndHandle,    // parentWndHandle
            NULL,//菜单句柄
            Global::dllInstanceHandle,//实例句柄
            this);   // lpParam

        if (!_wndHandle)
        {
            return FALSE;
        }
    }

    return TRUE;
}
```

>注意类的继承关系，和窗口的继承关系。

## 3.31.2 窗口过程

窗口基类，将窗口消息分发给其派生类的窗口过程。

```C++
LRESULT CALLBACK CBaseWindow::_WindowProc(_In_ HWND wndHandle, UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        _SetThis(wndHandle, ((CREATESTRUCT*)lParam)->lpCreateParams);
    }

    CBaseWindow* pv = _GetThis(wndHandle);
    if (!pv)
    {
        return DefWindowProc(wndHandle, uMsg, wParam, lParam);
    }

    if (uMsg == WM_TIMER)
    {
        switch (wParam)
        {
        case idTimer_UIObject:
            if (pv->_GetTimerObject() != nullptr)
            {
                pv->_GetTimerObject()->_OnTimer();
            }
            break;
        }
        return 0;
    }
    else
    {
        return pv->_WindowProcCallback(wndHandle, uMsg, wParam, lParam);
    }
}
```