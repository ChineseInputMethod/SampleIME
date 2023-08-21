## 3.6 线程管理器事件接收器

线程管理器事件接收器，主要处理焦点事件。当输入法获取焦点后，OnSetFocus()方法将被调用。

## 3.6.1 安装线程管理器事件接收器

Interface		|Description
-|-
[ITfSource][1]	|事件安装器。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfSource.md

获取事件安装器。

```C++
_pThreadMgr->QueryInterface(IID_ITfSource, (void **)&pSource);
```

安装线程管理器事件接收器。

```C++
pSource->AdviseSink(IID_ITfThreadMgrEventSink, (ITfThreadMgrEventSink *)this, &_threadMgrEventSinkCookie);
```

## 3.6.2 线程焦点

Interface			|Description
-|-
[ITfContext][2]		|上下文。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfContext.md

在OnSetFocus()方法中，主要完成三个方面的工作。<br/>
重新安装编辑会话完成消息接收器：

```C++
_InitTextEditSink(pDocMgrFocus);
```

更新托盘图标：

```C++
_UpdateLanguageBarOnSetFocus(pDocMgrFocus);
```

调用候选窗口焦点事件处理函数：

```C++
    if (_pCandidateListUIPresenter)
    {
        ITfDocumentMgr* pCandidateListDocumentMgr = nullptr;
        ITfContext* pTfContext = _pCandidateListUIPresenter->_GetContextDocument();
        if ((nullptr != pTfContext) && SUCCEEDED(pTfContext->GetDocumentMgr(&pCandidateListDocumentMgr)))
        {
            if (pCandidateListDocumentMgr != pDocMgrFocus)
            {
                _pCandidateListUIPresenter->OnKillThreadFocus();
            }
            else 
            {
                _pCandidateListUIPresenter->OnSetThreadFocus();
            }

            pCandidateListDocumentMgr->Release();
        }
    }
```

## 3.6.3 TSF框架和中文输入观念的不同

Windows系统是英文开发者开发的，无论TSF框架还是过去的IME框架其实都是为日文系统设计的。这些语言的文字与键盘编码是一一对应的，没有汉字输入法的“汉字编码”概念。<br/>
最初的输入法设计者们也深受这种影响，想要发明出汉字与编码“XX”对应的输入法。这里的“XX”指发明人的各种理念。<br/>

与本小节相关的问题是，线程管理器事件接收器处理焦点事件，其实就是对应的更新输入窗口、候选窗口和状态窗口。<br/>
对于英文字处理软件，编码就是文字，文字就是编码。编码存在于输入法的输入窗口中，显然是违和的。<br/>
对于日文字处理软件，虽然有由罗马字转换为假名和汉字的这一“合成”过程，但是这种转换发生在字处理软件中，显然比先发生在输入法的窗口中，然后将转换结果发送给字处理软件，更加直接一些。

所以微软输入法的标志特点是：输入窗口和候选窗口是分离的。而其他几乎所有的中文输入法的输入窗口和候选窗口是在一起的。<br/>
实际上微软输入法才是TSF框架的标准实现方式。具体到本小节，在OnSetFocus()方法中，只重新安装了编辑会话完成消息接收器，并没有对输入编码进行处理。<br/>
在TSF框架里输入编码是由应用程序处理（UI部分）的。

现在的字处理软件已经可以很好的处理半角空格和全角空格，以及标点符号。但是，早期这是一个很麻烦的问题。当时的输入法需要一个状态条，指示当前的标点和空格状态。
现在的中文输入法开发者依然希望有一个状态条，以显示更多的信息。然而微软的设计理念是，只希望提供一个中英文状态的系统托盘图标。

微软甚至希望连候选窗口也消灭掉，因为由系统或应用程序来提供输入UI，可以带来一致性，保持程序的整体风格。而输入法窗口，更像一个悬浮窗口，其风格和显示位置是不受应用程序控制的。<br/>
过去经常会出现输入法窗口遮挡了应用程序，用户不得不拖动输入法窗口或关闭输入法，以执行某项操作。现在这个问题，已经有所改观不是那么严重了。