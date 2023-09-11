## 文件说明

本章输入法源自[Windows-classic-samples](https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/IME)。
使用Visual Studio 2019编译。测试环境为Windows 7和Windows 10的64位版。

Files			|Description
-|-
track.xml		|断点文件，跟踪主要函数的调用关系。
install.bat		|安装文件，安装debug64位版。
uninstall.bat	|卸载文件。

## COM组件框架

Source Files					|Document
-|-
[DllMain.cpp](doc/DllMain.md)	|DLL入口点，注册候选窗口类。
[Globals.cpp](doc/Globals.md)	|全局设置，全局变量、类型、函数，对标点符号以及快捷键进行通用设置。
[Server.cpp](doc/Server.md)		|COM导出函数，注册输入法以及创建输入法实例。
[Register.cpp](doc/Register.md)	|注册输入法，注册TSF输入法的标准实现。

## TSF文本服务框架

Source Files																	|Document
-|-
[SampleIME.cpp](doc/SampleIME.md)												|TSF文本服务框架，扩展了对无界面元素程序和触摸键盘支持。
[ThreadMgrEventSink.cpp](doc/ThreadMgrEventSink.md)								|线程管理器事件接收器，主要处理焦点事件。
[TextEditSink.cpp](doc/TextEditSink.md)											|编辑会话完成消息接收器，用于感知其他文本服务对编辑内容的更改。
[KeyEventSink.cpp](doc/KeyEventSink.md)											|键盘事件接收器，用于接收按键和虚拟按键事件。
[ActiveLanguageProfileNotifySink.cpp](doc/ActiveLanguageProfileNotifySink.md)	|语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。
[ThreadFocusSink.cpp](doc/ThreadFocusSink.md)									|线程输入焦点消息接收器，在线程接收或失去UI焦点时接收通知。
[FunctionProviderSink.cpp](doc/FunctionProviderSink.md)							|扩展功能提供者，提供各种函数对象。

## 配置输入法

Source Files																	|Document
-|-
[CompositionProcessorEngine.cpp](doc/CompositionProcessorEngine.md)				|合成处理器引擎，输入法核心类。
[TfInputProcessorProfile.cpp](doc/TfInputProcessorProfile.md)					|文本服务语言配置，用来操作一个或多个文本服务的语言配置文件。
[Compartment.cpp](doc/Compartment.md)											|公共缓冲池，用来保存输入法状态开关。
[LanguageBar.cpp](doc/LanguageBar.md)											|语言栏，设置输入法状态开关。

## 当按下编码键之后

Source Files																	|Document
-|-
[CompositionProcessorEngine.cpp](doc/CompositionProcessorEngine.md)				|合成处理器引擎，输入法核心类。

## 3.4 

Source Files																	|Document
-|-
[BaseDictionaryEngine.cpp](doc/BaseDictionaryEngine.md)							|词典引擎基类。
[TableDictionaryEngine.cpp](doc/TableDictionaryEngine.md)						|切分表。
[DictionaryParser.cpp](doc/DictionaryParser.md)									|句法分析器。
[DictionarySearch.cpp](doc/DictionarySearch.md)									|词典搜索。
[BaseWindow.cpp](doc/BaseWindow.md)												|窗口基类，派生窗口。
[ButtonWindow.cpp](doc/ButtonWindow.md)											|按钮窗口。
[CandidateWindow.cpp](doc/CandidateWindow.md)									|候选窗口。
[ScrollBarWindow.cpp](doc/ScrollBarWindow.md)									|滚动条窗口。
[ShadowWindow.cpp](doc/ShadowWindow.md)											|阴影窗口。
[CandidateListUIPresenter.cpp](doc/CandidateListUIPresenter.md)					|候选列表控制器。
[Composition.cpp](doc/Composition.md)											|输入组合终止消息接收器。
[RegKey.cpp](doc/RegKey.md)														|注册表类。
[DisplayAttribute.cpp](doc/DisplayAttribute.md)									|显示属性。
[DisplayAttributeInfo.cpp](doc/DisplayAttributeInfo.md)							|显示属性信息。
[DisplayAttributeProvider.cpp](doc/DisplayAttributeProvider.md)					|显示属性提供者。
[EditSession.cpp](doc/EditSession.md)											|编辑会话。
[GetTextExtentEditSession.cpp](doc/GetTextExtentEditSession.md)					|文本布局。
[EndComposition.cpp](doc/EndComposition.md)										|结束输入。
[EnumDisplayAttributeInfo.cpp](doc/EnumDisplayAttributeInfo.md)					|显示属性信息对象枚举器。
[EnumTfCandidates.cpp](doc/EnumTfCandidates.md)									|候选文字串对象枚举器。
[File.cpp](doc/File.md)															|文件，读取词典文件。
[FileMapping.cpp](doc/FileMapping.md)											|文件映射对象，将词典文件映射为内存数组。
[TfTextLayoutSink.cpp](doc/TfTextLayoutSink.md)		|文本布局消息接收器。
[KeyHandler.cpp](doc/KeyHandler.md)												|处理按键。
[KeyHandlerEditSession.cpp](doc/KeyHandlerEditSession.md)						|按键处理编辑会话。
[KeyStateCategory.cpp](doc/KeyStateCategory.md)									|按键处理分类。
[SampleIMEBaseStructure.cpp](doc/SampleIMEBaseStructure.md)						|数据结构。
[SearchCandidateProvider.cpp](doc/SearchCandidateProvider.md)					|搜索候选提供者。
[StartComposition.cpp](doc/StartComposition.md)									|开始输入。
[TipCandidateList.cpp](doc/TipCandidateList.md)									|候选列表对象。
[TipCandidateString.cpp](doc/TipCandidateString.md)								|候选文字串对象。