## 3.15 键盘事件接收器

键盘事件接收器，用于接收按键和虚拟按键事件。

## 3.15.1 安装键盘事件接收器

Interface				|Description
-|-
[ITfKeystrokeMgr][1]	|按键管理器，主要用来安装键盘事件接收器和注册保留键。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfKeystrokeMgr.md

```C++
    if (FAILED(_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr)))
    {
        return FALSE;
    }

    hr = pKeystrokeMgr->AdviseKeyEventSink(_tfClientId, (ITfKeyEventSink *)this, TRUE);
```

## 3.15.2 预处理按键

当按下和释放按键时，OnTestKeyDown()和OnTestKeyUp()方法将被调用。
OnTestKeyDown()和OnTestKeyUp()方法调用_IsKeyEaten()函数，判断是否需要继续处理按键事件。
当_IsKeyEaten()函数返回TRUE时，TSF管理器将继续调用相应的OnKeyDown()和OnKeyUp()函数。

>如果用wordpad.exe调试输入法，不会调用OnTestKeyDown()和OnTestKeyUp()函数。

## 3.15.3 处理按键事件

Interface				|Description
-|-
[ITfKeyEventSink][2]	|键盘事件接收器，用于接收按键和保留键事件。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfKeyEventSink.md

当按下编码键，TSF管理器会调用输入法的OnKeyDown()方法。
在OnKeyDown()方法中，当_IsKeyEaten()函数返回TRUE时，调用_InvokeKeyHandler()方法处理按键事件。