## 3.0 文件说明

本章输入法源自[Windows-classic-samples](https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/IME)。
使用Visual Studio 2019编译。测试环境为Windows 7和Windows 10的64位版。

## 3.1 框架类文件

Source Files					|Document
-|-
[DllMain.cpp](doc/DllMain.md)	|DLL入口点，注册窗口类。
[Server.cpp](doc/Server.md)		|

## 3.2 

Source Files																	|Document
-|-
[ActiveLanguageProfileNotifySink.cpp](doc/ActiveLanguageProfileNotifySink.md)	|语言配置激活消息接收器，当更改激活语言配置文件时，框架调用接收器。
[BaseDictionaryEngine.cpp](doc/BaseDictionaryEngine.md)							|词典引擎基类。
[BaseWindow.cpp](doc/BaseWindow.md)												|窗口基类，派生窗口。
[ButtonWindow.cpp](doc/ButtonWindow.md)											|按钮窗口。
[CandidateWindow.cpp](doc/CandidateWindow.md)									|候选窗口。
[CandidateListUIPresenter.cpp](doc/CandidateListUIPresenter.md)					|候选列表控制器。
[Compartment.cpp](doc/Compartment.md)											|缓冲池。
[Composition.cpp](doc/Composition.md)											|输入组合终止消息接收器。