## 3.36 合成

合成，标记为GUID_PROP_COMPOSING属性的文本范围。

处理输入编码的函数调用过程，请参考：[按键处理](../appendix/按键处理.md)

## 3.36.1 写入合成字符

Interface		|Description
-|-
[ITfContext][1]	|上下文，用来创建和管理编辑上下文。
[ITfRange][2]	|文本范围，用来操作给定上下文中的文本。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfContext.md
[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfRange.md

当用户按下编码键，输入法调用CSampleIME::_AddComposingAndChar()函数，将输入编码写入到上下文中。

首先，调用_FindComposingRange()函数，查找已有合成。<br>
然后，调用_SetInputString()函数，将输入编码写入上下文。

```C++
HRESULT CSampleIME::_AddComposingAndChar(TfEditCookie ec, _In_ ITfContext *pContext, _In_ CStringRange *pstrAddString)
{
    HRESULT hr = S_OK;

    ULONG fetched = 0;
    TF_SELECTION tfSelection;
    //当前插入点
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched) != S_OK || fetched == 0)
        return S_FALSE;

    //
    // make range start to selection
    //
    ITfRange* pAheadSelection = nullptr;
    hr = pContext->GetStart(ec, &pAheadSelection);//上下文开始
    if (SUCCEEDED(hr))
    {//将文本范围设置为从上下文开始到当前插入点的起始定位点
        hr = pAheadSelection->ShiftEndToRange(ec, tfSelection.range, TF_ANCHOR_START);
        if (SUCCEEDED(hr))
        {
            ITfRange* pRange = nullptr;
            BOOL exist_composing = _FindComposingRange(ec, pContext, pAheadSelection, &pRange);

            _SetInputString(ec, pContext, pRange, pstrAddString, exist_composing);

            if (pRange)
            {
                pRange->Release();
            }
        }
    }

    tfSelection.range->Release();

    if (pAheadSelection)
    {
        pAheadSelection->Release();
    }

    return S_OK;
}
```

## 3.36.2 查找已有合成

Interface					|Description
-|-
[ITfReadOnlyProperty][3]	|只读属性，ITfProperty继承此接口。
[IEnumTfRanges][4]			|文本范围枚举器，枚举文本范围对象。

[3]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfReadOnlyProperty.md
[4]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/IEnumTfRanges.md

第一次按下编码键会创建合成，并将第一个编码写入到上下文中，当按下第二个编码键时，输入法需要去上下文中查询是否存在合成。
这是因为COM框架的特性，输入法使用接口交互，而接口无法保存数据，所以输入法需要查询上下文中是否已经存在合成了。

## 3.36.3 覆盖文本范围内容

当在上下文中创建或查找到已有合成后，调用CSampleIME::_SetInputString()函数，将输入编码写入到上下文中。

当第一次按下编码键时，因为插入点之前没有GUID_PROP_COMPOSING属性，所以_InsertAtSelection()函数会被调用。
当按下第二个编码键时，因为插入点之前，也就是第一个编码被设置为了GUID_PROP_COMPOSING属性，所以_InsertAtSelection()函数不会被调用。

因为TSF框架的特性，第二个编码并不是追加到第一个编码的后面。例如，第一个编码输入"a"，第二个编码输入"b"。
ITfRange::SetText()方法，是用字符串"ab"，替换覆盖掉原文本范围内容"a"。
（从形式上看两者相同，但逻辑上不同）

```C++
HRESULT CSampleIME::_SetInputString(TfEditCookie ec, _In_ ITfContext *pContext, _Out_opt_ ITfRange *pRange, _In_ CStringRange *pstrAddString, BOOL exist_composing)
{
    ITfRange* pRangeInsert = nullptr;
    if (!exist_composing)
    {
        _InsertAtSelection(ec, pContext, pstrAddString, &pRangeInsert);
        if (pRangeInsert == nullptr)
        {
            return S_OK;
        }
        pRange = pRangeInsert;
    }
    if (pRange != nullptr)
    {
        pRange->SetText(ec, 0, pstrAddString->Get(), (LONG)pstrAddString->GetLength());
    }

    _SetCompositionLanguage(ec, pContext);

    _SetCompositionDisplayAttributes(ec, pContext, _gaDisplayAttributeInput);

    // update the selection, we'll make it an insertion point just past
    // the inserted text.
    ITfRange* pSelection = nullptr;
    TF_SELECTION sel;

    if ((pRange != nullptr) && (pRange->Clone(&pSelection) == S_OK))
    {
        pSelection->Collapse(ec, TF_ANCHOR_END);

        sel.range = pSelection;
        sel.style.ase = TF_AE_NONE;
        sel.style.fInterimChar = FALSE;
        pContext->SetSelection(ec, 1, &sel);
        pSelection->Release();
    }

    if (pRangeInsert)
    {
        pRangeInsert->Release();
    }


    return S_OK;
}
```

然后，调用_SetCompositionLanguage()函数，为合成设置语言属性，本章未讲解此性质。<br>
接着，调用_SetCompositionDisplayAttributes()函数，为合成设置显示属性，该性质在显示属性主题讲解。<br>
最后，调用ITfRange::Collapse()方法，将文本范围的起始点设置为终止点。（移动插入点位置）
然后，调用ITfContext::SetSelection()方法，更新调整后的插入点。

>注释掉239行，取消240行注释，可观察到向前插入输入编码。
注释掉239行，可观察到选中的输入编码。

## 3.36.4 输出汉字

当用户按下空格键选择候选字词后，CSampleIME::_AddComposingAndChar()函数再次被调用。<br>
可以发现将编码或汉字写入到合成，是没有任何区别的，实际上TSF管理器也并不知道，输入法写入的是编码还是汉字。<br>
不同之处是，在输出汉字后，输入法会调用ITfComposition::EndComposition()方法结束合成。

>TSF输入法的核心就是上面短短的几句话。只不过被套上了一层又一层外壳。作者从IME时代就搞不懂，怎么输出一个汉字。找到了一个源码，然后打印出来看，因为作者没有电脑。
看了很长时间也看不懂。其实，输出汉字就是下面几个步骤。

1. 首先，从ITfContext中获取ITfInsertAtSelection，然后从ITfInsertAtSelection中获取ITfRange，备用。
2. 接着，从ITfContext中获取ITfContextComposition，将ITfRange放入ITfContextComposition，获取ITfComposition，备用。（开始合成）
3. 然后，从ITfContext中获取ITfRange，将输入编码写入ITfRange，再将修改后的当前选择放到ITfContext中。（合成中）
4. 接着，重复步骤3，完成汉字输入或继续重复步骤3，完成自造词、语句输入等功能。
5. 最后，结束ITfComposition，完成输入。（结束合成）

我现在一直在想，为什么这么简单的东西，我做了这么久。答案竟然是，我足够幸运，才最终找出了答案。
非常想记录下每一个节点，我的心理感受，然而我没有时间。

## 3.36.4 合成终止消息接收器

Interface				|Description
-|-
[ITfCompositionSink][5]	|合成终止消息接收器，用于在终止ITfComposition合成时接收通知。

[5]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfCompositionSink.md

当合成被意外终止时，ITfCompositionSink::OnCompositionTerminated()方法将被调用。

```C++
STDAPI CSampleIME::OnCompositionTerminated(TfEditCookie ecWrite, _In_ ITfComposition *pComposition)
{
    // Clear dummy composition
    _RemoveDummyCompositionForComposing(ecWrite, pComposition);

    // Clear display attribute and end composition, _EndComposition will release composition for us
    ITfContext* pContext = _pContext;
    if (pContext)
    {
        pContext->AddRef();
    }

    _EndComposition(_pContext);

    _DeleteCandidateList(FALSE, pContext);

    if (pContext)
    {
        pContext->Release();
        pContext = nullptr;
    }

    return S_OK;
}
```