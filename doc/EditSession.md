## 3.19 编辑会话

编辑会话基接口，本章工程所有编辑会话均继承此接口。

## 3.19.1 编辑会话简介

Interface				|Description
-|-
[ITfEditSession][1]	|编辑会话，由TSF管理器调用，用来修改上下文的文本和属性。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfEditSession.md

当文本服务必须在上下文中获取、修改或设置文本时，必须发起编辑会话。
发起编辑会话时，TSF管理器会与目标上下文的所有者协商请求类型的文档锁。
授予文档锁后，TSF管理器允许文本服务读取和/或写入上下文中的文本。