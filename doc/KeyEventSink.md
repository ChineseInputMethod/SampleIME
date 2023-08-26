## 3.8 键盘事件接收器

键盘事件接收器，用于接收按键和虚拟按键事件。

## 3.8.1 安装键盘事件接收器

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

## 3.8.2 预处理按键

当按下和释放按键时，OnTestKeyDown()和OnTestKeyUp()函数将被调用。
OnTestKeyDown()和OnTestKeyUp()函数调用_IsKeyEaten()函数，判断是否需要继续处理按键事件。


```C++
    if (FAILED(_pThreadMgr->QueryInterface(IID_ITfKeystrokeMgr, (void **)&pKeystrokeMgr)))
    {
        return FALSE;
    }

    hr = pKeystrokeMgr->AdviseKeyEventSink(_tfClientId, (ITfKeyEventSink *)this, TRUE);
```

## 3.8.3 处理按键事件

当_IsKeyEaten()返回TRUE时，TSF管理器将继续调用相应的OnKeyDown()和OnKeyUp()函数。