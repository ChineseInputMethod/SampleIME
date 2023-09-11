## 3.12 合成处理器引擎

在输入法被激活时，调用CSampleIME::_AddTextProcessorEngine()方法，设置输入法配置。

## 3.12.1 获取语言配置文件

首先获取输入法的语言配置文件。

```C++
    if (FAILED(profile.CreateInstance()))
    {
        return FALSE;
    }

    if (FAILED(profile.GetCurrentLanguage(&langid)))
    {
        return FALSE;
    }

    if (FAILED(profile.GetDefaultLanguageProfile(langid, GUID_TFCAT_TIP_KEYBOARD, &clsid, &guidProfile)))
    {
        return FALSE;
    }
```

## 3.12.2 创建合成处理器引擎

然后创建合成处理器引擎，这是输入法的核心类。

```C++
    if (_pCompositionProcessorEngine == nullptr)
    {
        _pCompositionProcessorEngine = new (std::nothrow) CCompositionProcessorEngine();
    }
```

## 3.12.3 设置语言配置文件

最后调用_pCompositionProcessorEngine->SetupLanguageProfile()方法，设置输入法的语言配置文件。

Function							|Description
-|-
SetupPreserved()					|设置输入法状态开关快捷键。
InitializeSampleIMECompartment()	|将输入法默认状态设置到缓冲区。
SetupPunctuationPair()				|设置标点对。
SetupLanguageBar()					|设置输入法状态开关。
SetupKeystroke()					|设置编码键。
SetupConfiguration()				|设置候选栏。
SetupDictionaryFile()				|打开输入法词典文件。

## 3.12.4 完成初始化

调用_AddTextProcessorEngine()完成后，ActivateEx()方法退出，输入法完成初始化。
然后根据安装的事件接收器，开始进入工作状态。

首先ITfActiveLanguageProfileNotifySink::OnActivated()方法被调用。这是输入法做为普通应用程序响应键盘布局激活事件。

然后ITfThreadMgrEventSink::OnSetFocus()方法被调用，这是输入法做为普通应用程序响应输入焦点事件。