## 3.20 按键处理编辑会话

按键处理编辑会话，在会话中按照按键分类调用相应处理函数。

## 3.20.1 文档锁

Function		|Description
-|-
DoEditSession()	|由TSF管理器提供的TfEditCookie唯一标识符，用于访问上下文。

当输入法申请ITfEditSession编辑会话后，TSF管理器锁定上下文，调用ITfEditSession::DoEditSession()函数，读取和/或修改上下文的文本和属性。