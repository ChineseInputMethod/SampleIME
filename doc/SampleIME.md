## 3.5 TSF文本服务框架

CSampleIME继承的接口分为两类。一类是隐式声明具备的能力（例如注册的类别）。当需要调用相应接口时，TSF管理器会查询调用的接口是否实现：

Interface									|Description
-|-
[ITfTextInputProcessorEx][1]				|文本输入处理器，继承ITfTextInputProcessor，扩展了文本服务的激活方式。
[ITfDisplayAttributeProvider][2]			|显示属性提供者，在显示属性专题讲解。当输入法注册了显示属性类别后，由客户端查询显示属性。
[ITfCompositionSink][3]						|输入组合终止消息接收器，在输入合成专题讲解。意外终止合成时被调用。
[ITfFnGetPreferredTouchKeyboardLayout][4]	|获取首选触摸键盘布局。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfTextInputProcessorEx.md
[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfDisplayAttributeProvider.md
[3]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfCompositionSink.md
[4]: https://learn.microsoft.com/zh-cn/windows/win32/api/ctffunc/nn-ctffunc-itffngetpreferredtouchkeyboardlayout

另一类是在输入法激活时显式安装的事件接收器，当发生相应事件后，TSF管理器会调用相应的事件接收器：

Interface								|Description
-|-
[ITfThreadMgrEventSink][5]				|线程管理器事件接收器，主要处理焦点事件。
[ITfTextEditSink][6]					|编辑会话完成消息接收器，用于感知其他文本服务对编辑内容的更改。
[ITfKeyEventSink][7]					|键盘事件接收器，用于接收按键和虚拟按键事件。
[ITfActiveLanguageProfileNotifySink][8]	|语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。
[ITfThreadFocusSink][9]					|线程输入焦点消息接收器。
[ITfFunctionProvider][10]				|扩展功能提供者。


[5]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfThreadMgrEventSink.md
[6]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfTextEditSink.md
[7]: https://learn.microsoft.com/zh-cn/windows/win32/api/msctf/nn-msctf-itfkeyeventsink
[8]: https://learn.microsoft.com/zh-cn/windows/win32/api/msctf/nn-msctf-itfactivelanguageprofilenotifysink
[9]: https://learn.microsoft.com/zh-cn/windows/win32/api/msctf/nn-msctf-itfthreadfocussink
[10]: https://learn.microsoft.com/zh-cn/windows/win32/api/msctf/nn-msctf-itffunctionprovider


## 3.5.1 激活输入法

当输入法被激活时（键盘布局被载入），ITfTextInputProcessorEx::ActivateEx方法首先被调用。
ITfTextInputProcessorEx是ITfTextInputProcessor的升级版本，扩展了激活方式，例如增加了对uiless模式的支持。

激活输入法的主要任务是安装事件接收器和初始化输入法。

Function								|Description
-|-
_InitThreadMgrEventSink()				|安装线程管理器事件接收器。
_InitTextEditSink()						|安装编辑会话完成消息接收器。
_InitKeyEventSink()						|安装键盘事件接收器。
_InitActiveLanguageProfileNotifySink()	|安装语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。
_InitThreadFocusSink()					|安装线程输入焦点消息接收器。
_InitFunctionProviderSink()				|安装扩展功能提供者。创建一个候选词搜索引擎。

Function						|Description
-|-
_InitDisplayAttributeGuidAtom()	|注册显示属性标识符。
_AddTextProcessorEngine()		|添加文本处理引擎。创建一个编码分词引擎。

## 3.5.2 触摸键盘布局

## 3.5.3 创建候选词搜索引擎