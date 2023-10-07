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

## 3.30.3 添加候选窗口数据

CCandidateWindow::_AddString()函数只是简单为候选列表生成了一个副本。<br>
在实际的输入法开发中，一般也应该为每个候选窗口生成一个副本。<br>
这是因为输入法可能在多个软件中运行，每个候选窗口的候选列表应该是独立的。<br>

```C++
void CCandidateWindow::_AddString(_Inout_ CCandidateListItem *pCandidateItem, _In_ BOOL isAddFindKeyCode)
{
    DWORD_PTR dwItemString = pCandidateItem->_ItemString.GetLength();
    const WCHAR* pwchString = nullptr;
    if (dwItemString)
    {
        pwchString = new (std::nothrow) WCHAR[ dwItemString ];
        if (!pwchString)
        {
            return;
        }
        memcpy((void*)pwchString, pCandidateItem->_ItemString.Get(), dwItemString * sizeof(WCHAR));
    }

    DWORD_PTR itemWildcard = pCandidateItem->_FindKeyCode.GetLength();
    const WCHAR* pwchWildcard = nullptr;
    if (itemWildcard && isAddFindKeyCode)
    {
        pwchWildcard = new (std::nothrow) WCHAR[ itemWildcard ];
        if (!pwchWildcard)
        {
            if (pwchString)
            {
                delete [] pwchString;
            }
            return;
        }
        memcpy((void*)pwchWildcard, pCandidateItem->_FindKeyCode.Get(), itemWildcard * sizeof(WCHAR));
    }

    CCandidateListItem* pLI = nullptr;
    pLI = _candidateList.Append();
    if (!pLI)
    {
        if (pwchString)
        {
            delete [] pwchString;
            pwchString = nullptr;
        }
        if (pwchWildcard)
        {
            delete [] pwchWildcard;
            pwchWildcard = nullptr;
        }
        return;
    }

    if (pwchString)
    {
        pLI->_ItemString.Set(pwchString, dwItemString);
    }
    if (pwchWildcard)
    {
        pLI->_FindKeyCode.Set(pwchWildcard, itemWildcard);
    }

    return;
}
```

## 3.30.4 绘制消息

当输入法调用_pCandidateWnd->_InvalidateRect()函数，失效工作区后，系统向窗口发送WM_PAINT消息。<br>
窗口过程调用CCandidateWindow::_OnPaint()函数，处理WM_PAINT消息。

首先，调用_GetCurrentPage()函数，获取当前候选页。<br>
然后，调用_AdjustPageIndex()函数，调整当前候选项。<br>
最后，调用_DrawList()函数，绘制当前候选页面。

```C++
void CCandidateWindow::_OnPaint(_In_ HDC dcHandle, _In_ PAINTSTRUCT *pPaintStruct)
{
    SetBkMode(dcHandle, TRANSPARENT);//透明

    HFONT hFontOld = (HFONT)SelectObject(dcHandle, Global::defaultlFontHandle);

    FillRect(dcHandle, &pPaintStruct->rcPaint, _brshBkColor);//COLOR_WINDOW+1

    UINT currentPageIndex = 0;
    UINT currentPage = 0;

    if (FAILED(_GetCurrentPage(&currentPage)))
    {
        goto cleanup;
    }
    
    _AdjustPageIndex(currentPage, currentPageIndex);

    _DrawList(dcHandle, currentPageIndex, &pPaintStruct->rcPaint);

cleanup:
    SelectObject(dcHandle, hFontOld);
}
```