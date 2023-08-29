## 3.10 线程输入焦点消息接收器

线程输入焦点消息接收器，在线程接收或失去UI焦点时接收通知。

## 3.10.1 安装线程输入焦点消息接收器

```C++
    if (FAILED(_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource)))
    {
        return FALSE;
    }

    if (FAILED(pSource->AdviseSink(IID_ITfThreadFocusSink, (ITfThreadFocusSink *)this, &_dwThreadFocusSinkCookie)))
    {
        pSource->Release();
        return FALSE;
    }
```

## 3.10.2 响应线程输入焦点消息

当线程获得输入焦点时，重绘候选窗口。

Interface				|Description
-|-
[ITfThreadFocusSink][1]	|线程输入焦点消息接收器，在线程接收或失去UI焦点时接收通知。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfThreadFocusSink.md