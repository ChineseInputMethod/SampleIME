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

## 输入法配置

Source Files														|Document
-|-
[CompositionProcessorEngine.cpp](doc/CompositionProcessorEngine.md)	|合成处理器引擎，输入法核心类。
[TfInputProcessorProfile.cpp](doc/TfInputProcessorProfile.md)		|文本服务语言配置，用来操作一个或多个文本服务的语言配置文件。
[Compartment.cpp](doc/Compartment.md)								|公共缓冲池，用来保存输入法状态开关。
[LanguageBar.cpp](doc/LanguageBar.md)								|语言栏，设置输入法状态开关。
[File.cpp](doc/File.md)												|文件，打开码表文件。
[FileMapping.cpp](doc/FileMapping.md)								|文件映射，将码表文件创建为文件映射对象。

## 按键事件

Source Files												|Document
-|-
[KeyHandler.cpp](doc/KeyHandler.md)							|按键处理器，用来处理按键事件。
[EditSession.cpp](doc/EditSession.md)						|编辑会话基接口，本章工程所有编辑会话均继承此接口。
[KeyHandlerEditSession.cpp](doc/KeyHandlerEditSession.md)	|按键处理编辑会话，在会话中按照按键分类调用相应处理函数。
[KeyStateCategory.cpp](doc/KeyStateCategory.md)				|按键分类，按照按键分类调用相应处理函数。

## 候选列表

Source Files												|Document
-|-
[SampleIMEBaseStructure.h](doc/SampleIMEBaseStructure.md)	|数据结构，输入法自定义数据结构。
[TableDictionaryEngine.cpp](doc/TableDictionaryEngine.md)	|词典引擎，获取候选列表。
[DictionarySearch.cpp](doc/DictionarySearch.md)				|词典搜索，在词典中搜索编码为输入编码的汉字。
[DictionaryParser.cpp](doc/DictionaryParser.md)				|词典搜索基类，为词典搜索提供辅助函数。
[BaseDictionaryEngine.cpp](doc/BaseDictionaryEngine.md)		|词典引擎基类，为词典引擎提供辅助函数。

## 候选窗口控制

Source Files													|Document
-|-
[CandidateListUIPresenter.cpp](doc/CandidateListUIPresenter.md)	|候选窗口控制器，控制候选窗口的显示。
[TfTextLayoutSink.cpp](doc/TfTextLayoutSink.md)					|文本布局消息接收器，通过响应布局更改消息，实现光标跟随。
[GetTextExtentEditSession.cpp](doc/GetTextExtentEditSession.md)	|文本布局会话，处理文本布局消息。

## 候选窗口绘制

Source Files									|Document
-|-
[CandidateWindow.cpp](doc/CandidateWindow.md)	|候选窗口，在主窗口中显示候选列表。
[BaseWindow.cpp](doc/BaseWindow.md)				|窗口基类，为派生类调用API函数。
[ShadowWindow.cpp](doc/ShadowWindow.md)			|阴影窗口，为窗口添加阴影。
[ScrollBarWindow.cpp](doc/ScrollBarWindow.md)	|滚动条窗口。
[ButtonWindow.cpp](doc/ButtonWindow.md)			|按钮窗口。

## 合成

Source Files												|Document
-|-
[StartComposition.cpp](doc/composition/StartComposition.md)	|开始合成，在上下文中创建一个合成。
[Composition.cpp](doc/composition/Composition.md)			|合成，标记为GUID_PROP_COMPOSING属性的文本范围。
[EndComposition.cpp](doc/composition/EndComposition.md)		|结束合成，完成汉字输入。

## 显示属性

Source Files													|Document
-|-
[DisplayAttribute.cpp](doc/DisplayAttribute.md)					|显示属性，合成中编码或汉字的显示样式。
[DisplayAttributeProvider.cpp](doc/DisplayAttributeProvider.md)	|显示属性提供者，由TSF管理器用来枚举和获取单个显示属性信息对象。
[DisplayAttributeInfo.cpp](doc/DisplayAttributeInfo.md)			|显示属性信息对象，为应用程序提供显示属性信息。
[EnumDisplayAttributeInfo.cpp](doc/EnumDisplayAttributeInfo.md)	|显示属性信息对象枚举器，未演示此接口。

## 无界面模式

Source Files						|Document
-|-
[UIless.md](doc/uiless/UIless.md)	|无界面模式，由应用程序显示UI元素。

## 其他主题

Source Files													|Document
-|-
[RegKey.cpp](doc/RegKey.md)										|注册表类，未讲解。
[SearchCandidateProvider.cpp](doc/SearchCandidateProvider.md)	|搜索候选提供者，未讲解。
[TipCandidateList.cpp](doc/TipCandidateList.md)					|候选列表对象，未讲解。
[TipCandidateString.cpp](doc/TipCandidateString.md)				|候选文字串对象，未讲解。
[EnumTfCandidates.cpp](doc/EnumTfCandidates.md)					|候选文字串对象枚举器，未讲解。

## 附录A

1. [启动过程](doc/appendix/启动过程.md)
2. [按键处理](doc/appendix/按键处理.md)
3. [候选列表](doc/appendix/候选列表.md)

## 附录B

1. [选择候选字词](doc/appendix/选择候选字词.md)
2. [销毁候选窗口](doc/appendix/销毁候选窗口.md)
3. [完成汉字输入](doc/appendix/完成汉字输入.md)

## 附录C

1. [启动过程](doc/uiless/启动过程.md)
2. [按键处理](doc/uiless/按键处理.md)
3. [候选列表](doc/uiless/候选列表.md)