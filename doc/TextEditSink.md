## 3.7 编辑会话完成消息接收器

编辑会话完成消息接收器，用于感知其他文本服务对编辑内容的更改。

## 3.7.1 安装编辑会话完成消息接收器

Interface			|Description
-|-
[ITfDocumentMgr][1]	|文档管理器，主要用来创建和管理上下文。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfDocumentMgr.md

首先清除已安装的编辑会话完成消息接收器。

```C++
    if (_textEditSinkCookie != TF_INVALID_COOKIE)
    {
        if (SUCCEEDED(_pTextEditSinkContext->QueryInterface(IID_ITfSource, (void **)&pSource)))
        {
            pSource->UnadviseSink(_textEditSinkCookie);
            pSource->Release();
        }

        _pTextEditSinkContext->Release();
        _pTextEditSinkContext = nullptr;
        _textEditSinkCookie = TF_INVALID_COOKIE;
    }
```

然后得到文档顶部的上下文。

```C++
    if (FAILED(pDocMgr->GetTop(&_pTextEditSinkContext)))
    {
        return FALSE;
    }
```

最后在上下文中安装编辑会话完成消息接收器。

```C++
    if (SUCCEEDED(_pTextEditSinkContext->QueryInterface(IID_ITfSource, (void **)&pSource)))
    {
        if (SUCCEEDED(pSource->AdviseSink(IID_ITfTextEditSink, (ITfTextEditSink *)this, &_textEditSinkCookie)))
        {
            ret = TRUE;
        }
        else
        {
            _textEditSinkCookie = TF_INVALID_COOKIE;
        }
        pSource->Release();
    }
```

## 3.7.2 处理编辑会话完成消息

Interface			|Description
-|-
[ITfEditRecord][2]	|编辑记录，用来确定编辑会话期间更改的内容。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfEditRecord.md

安装ITfTextEditSink编辑会话完成消息接收器后，每当写访问文档锁被释放时，TSF管理器调用OnEndEdit()方法。

首先判断编辑内容是否已更改。

```C++
    if (SUCCEEDED(pEditRecord->GetSelectionStatus(&isSelectionChanged)) &&
        isSelectionChanged)
    {
```

当编辑内容已更改，且处于输入状态时，获取文档中的选定内容。

```C++
        if (_IsComposing())
        {
            TF_SELECTION tfSelection;
            ULONG fetched = 0;

            if (pContext == nullptr)
            {
                return E_INVALIDARG;
            }
            if (FAILED(pContext->GetSelection(ecReadOnly, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched)) || fetched != 1)
            {
                return S_FALSE;
            }
```

然后获取一个范围对象，该对象包含合成所涵盖的文本。

```C++
            ITfRange* pRangeComposition = nullptr;
            if (SUCCEEDED(_pComposition->GetRange(&pRangeComposition)))
            {
```

如果其他文本服务更改的编辑内容覆盖了输入法正在编辑的文本范围，则结束合成。

```C++
                if (!_IsRangeCovered(ecReadOnly, tfSelection.range, pRangeComposition))
                {
                    _EndComposition(pContext);
                }
```

## 3.7.3 文本范围

Interface			|Description
-|-
[ITfRange][3]	|文本范围，用来引用和操作给定上下文中的文本

[3]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfRange.md

TSF不使用绝对索引来定位文本流的位置，而是使用被称为ITfRange文本范围的相对位置。

这段代码的意思是，如果输入组合的起点在更改后的文本范围起点之前，则返回FALSE。

```C++
    if (FAILED(pRangeCover->CompareStart(ec, pRangeTest, TF_ANCHOR_START, &lResult)) 
        || (lResult > 0))
    {
        return FALSE;
    }
```

下面这段代码的意思是，如果输入组合的终点在更改后的文本范围终点之后，则返回FALSE。

```C++
    if (FAILED(pRangeCover->CompareEnd(ec, pRangeTest, TF_ANCHOR_END, &lResult)) 
        || (lResult < 0))
    {
        return FALSE;
    }
```

作者并不明白这个覆盖的含义，大致意思是说，只要输入组合的端点超出了修改后文本范围，就要结束合成。