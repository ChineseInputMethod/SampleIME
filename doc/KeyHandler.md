## 3.18 按键处理器

按键处理器，用来处理按键事件。

## 3.18.1 发起编辑会话

当按下键盘按键，TSF管理器会调用输入法的OnKeyDown()方法。
当输入法处理该按键时，调用_InvokeKeyHandler()方法处理按键事件。
输入法在_InvokeKeyHandler()方法中，发起ITfEditSession编辑会话。

```C++
HRESULT CSampleIME::_InvokeKeyHandler(_In_ ITfContext *pContext, UINT code, WCHAR wch, DWORD flags, _KEYSTROKE_STATE keyState)
{
    flags;

    CKeyHandlerEditSession* pEditSession = nullptr;
    HRESULT hr = E_FAIL;

    // we'll insert a char ourselves in place of this keystroke
    pEditSession = new (std::nothrow) CKeyHandlerEditSession(this, pContext, code, wch, keyState);
    if (pEditSession == nullptr)
    {
        goto Exit;
    }

    //
    // Call CKeyHandlerEditSession::DoEditSession().
    //
    // Do not specify TF_ES_SYNC so edit session is not invoked on WinWord
    //
    hr = pContext->RequestEditSession(_tfClientId, pEditSession, TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, &hr);

    pEditSession->Release();

Exit:
    return hr;
}
```

## 3.18.2 处理编码键

当按下编码键后，如果没有开始合成，则开始合成，保存合成以及上下文。<br>
然后获取当前插入范围,与合成范围比较，如果合成范围不覆盖插入点，则不继续处理。<br>
然后将编码保存到合成处理器引擎。<br>
最后调用_HandleCompositionInputWorker()，开始处理编码。

```C++
HRESULT CSampleIME::_HandleCompositionInput(TfEditCookie ec, _In_ ITfContext *pContext, WCHAR wch)
{
    ITfRange* pRangeComposition = nullptr;
    TF_SELECTION tfSelection;
    ULONG fetched = 0;
    BOOL isCovered = TRUE;

    CCompositionProcessorEngine* pCompositionProcessorEngine = nullptr;
    pCompositionProcessorEngine = _pCompositionProcessorEngine;

    if ((_pCandidateListUIPresenter != nullptr) && (_candidateMode != CANDIDATE_INCREMENTAL))
    {
        _HandleCompositionFinalize(ec, pContext, FALSE);
    }

    // Start the new (std::nothrow) compositon if there is no composition.
    if (!_IsComposing())
    {
        _StartComposition(pContext);
    }

    // first, test where a keystroke would go in the document if we did an insert
    if (pContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched) != S_OK || fetched != 1)
    {
        return S_FALSE;
    }

    // is the insertion point covered by a composition?
    if (SUCCEEDED(_pComposition->GetRange(&pRangeComposition)))
    {
        isCovered = _IsRangeCovered(ec, tfSelection.range, pRangeComposition);

        pRangeComposition->Release();

        if (!isCovered)
        {
            goto Exit;
        }
    }

    // Add virtual key to composition processor engine
    pCompositionProcessorEngine->AddVirtualKey(wch);

    _HandleCompositionInputWorker(pCompositionProcessorEngine, ec, pContext);

Exit:
    tfSelection.range->Release();
    return S_OK;
}
```

## 3.18.3 处理编码

假设最简单的场景，既只输入一个编码“a”。那么输入法主要完成两项工作：<br>
1、在输入窗口显示编码“a”。<br>
2、在候选窗口显示汉字“啊……”。

在TSF框架里编码是由TSF管理器控制的，所以输入法仅仅是将编码写入Composition合成。<br>
Windows程序习惯将数据与视图分离，所以有单独的CandidateList候选列表数据处理过程。

在CSampleIME::_HandleCompositionInputWorker()函数中，首先从合成处理器引擎中获取汉字编码。<br>
然后将汉字编码写入Composition合成。<br>
接着获取CandidateList候选列表。<br>
最后将CandidateList候选列表写入候选列表UI控制器。

```C++
HRESULT CSampleIME::_HandleCompositionInputWorker(_In_ CCompositionProcessorEngine *pCompositionProcessorEngine, TfEditCookie ec, _In_ ITfContext *pContext)
{
    HRESULT hr = S_OK;
    CSampleImeArray<CStringRange> readingStrings;
    BOOL isWildcardIncluded = TRUE;

    //
    // Get reading string from composition processor engine
    //
    pCompositionProcessorEngine->GetReadingStrings(&readingStrings, &isWildcardIncluded);

    for (UINT index = 0; index < readingStrings.Count(); index++)
    {
        hr = _AddComposingAndChar(ec, pContext, readingStrings.GetAt(index));
        if (FAILED(hr))
        {
            return hr;
        }
    }

    //
    // Get candidate string from composition processor engine
    //
    CSampleImeArray<CCandidateListItem> candidateList;

    pCompositionProcessorEngine->GetCandidateList(&candidateList, TRUE, FALSE);

    if ((candidateList.Count()))
    {
        hr = _CreateAndStartCandidate(pCompositionProcessorEngine, ec, pContext);
        if (SUCCEEDED(hr))
        {
            _pCandidateListUIPresenter->_ClearList();
            _pCandidateListUIPresenter->_SetText(&candidateList, TRUE);
        }
    }
    else if (_pCandidateListUIPresenter)
    {
        _pCandidateListUIPresenter->_ClearList();
    }
    else if (readingStrings.Count() && isWildcardIncluded)
    {
        hr = _CreateAndStartCandidate(pCompositionProcessorEngine, ec, pContext);
        if (SUCCEEDED(hr))
        {
            _pCandidateListUIPresenter->_ClearList();
        }
    }
    return hr;
}
```

>_AddComposingAndChar(ec, pContext, readingStrings.GetAt(index))将在合成主题讲解。
>GetCandidateList(&candidateList, TRUE, FALSE)将在候选列表主题讲解。
>_pCandidateListUIPresenter->_SetText(&candidateList, TRUE)将在候选窗口主题讲解。

其他更复杂的编码处理，将在后续主题讲解。