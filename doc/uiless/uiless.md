## 3.41 无界面模式

无界面模式，由应用程序显示UI元素。

## 3.41.1 调试

该仓库下：https://github.com/yangyuan/meow

有个https://github.com/yangyuan/meow/tree/master/src/meow-uiless工程，可以用于调试输入法。

我在https://github.com/ChineseInputMethod/SampleIME/tree/master/doc/uiless目录下放置了一个编译好的该工程。

如图设置，即可调试无界面模式。

![DEBUG](debug.png)

## 3.41.2 启动和按键处理

无界面模式的启动过程和按键处理函数调用，请参考：[启动过程](启动过程.md) [按键处理](按键处理.md)

启动过程和按键处理与普通模式几乎完全相同，唯一不同的地方是，在ITfTextInputProcessorEx::ActivateEx()方法中，dwFlags的状态值不同。

## 3.41.3 候选列表

无界面模式的候选列表函数调用过程，请参考：[候选列表](候选列表.md)

在CCandidateListUIPresenter::BeginUIElement()函数中，输入法调用ITfUIElementMgr::BeginUIElement()方法，向应用程序查询是否显示候选窗口。

当应用程序自己显示候选窗口时，CCandidateListUIPresenter::_UpdateUIElement()函数将被调用。

在CCandidateListUIPresenter::_UpdateUIElement()函数中，调用ITfUIElementMgr::UpdateUIElement()方法，通知应用程序更新UI元素。

Interface				|Description
-|-
[ITfUIElementMgr][1]	|UI元素管理器，文本服务调用ITfUIElementMgr，操作应用程序UI元素。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfUIElementMgr.md

```C++
HRESULT CCandidateListUIPresenter::_UpdateUIElement()
{
    HRESULT hr = S_OK;

    ITfThreadMgr* pThreadMgr = _pTextService->_GetThreadMgr();
    if (nullptr == pThreadMgr)
    {
        return S_OK;
    }

    ITfUIElementMgr* pUIElementMgr = nullptr;

    hr = pThreadMgr->QueryInterface(IID_ITfUIElementMgr, (void **)&pUIElementMgr);
    if (hr == S_OK)
    {
        pUIElementMgr->UpdateUIElement(_uiElementId);
        pUIElementMgr->Release();
    }

    return S_OK;
}
```

随后，应用程序调用输入法ITfCandidateListUIElement候选列表UI元素接口。

Interface						|Description
-|-
[ITfCandidateListUIElement][2]	|候选列表UI元素，向应用程序提供UI元素。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfUIElementMgr.md

当完成汉字输入后，输入法调用ITfUIElementMgr::EndUIElement()方法，通知应用程序隐藏UI元素。