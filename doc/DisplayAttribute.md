## 3.38 显示属性

显示属性，合成中编码或汉字的显示样式。

TSF框架由TSF管理器管理输入编码和转换字符（日韩输入法中的概念）的显示，输入法负责向TSF管理器提供显示的内容和样式。<br>
内容就是合成中的输入编码和转换字符。样式就是本主题介绍的显示属性。

## 3.38.1 激活

Interface			|Description
-|-
[ITfCategoryMgr][1]	|类别管理器，为输入法管理类别。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfCategoryMgr.md

因为效率的原因，输入法在激活的时候，需要调用ITfCategoryMgr::RegisterGUID()方法，将GUID添加到内部表，并获取GUID的原子。

```C++
BOOL CSampleIME::_InitDisplayAttributeGuidAtom()
{
    ITfCategoryMgr* pCategoryMgr = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER, IID_ITfCategoryMgr, (void**)&pCategoryMgr);

    if (FAILED(hr))
    {
        return FALSE;
    }

    // register the display attribute for input text.
    hr = pCategoryMgr->RegisterGUID(Global::SampleIMEGuidDisplayAttributeInput, &_gaDisplayAttributeInput);
	if (FAILED(hr))
    {
        goto Exit;
    }
    // register the display attribute for the converted text.
    hr = pCategoryMgr->RegisterGUID(Global::SampleIMEGuidDisplayAttributeConverted, &_gaDisplayAttributeConverted);
	if (FAILED(hr))
    {
        goto Exit;
    }

Exit:
    pCategoryMgr->Release();

    return (hr == S_OK);
}
```

## 3.38.2 合成

在CSampleIME::_SetInputString()函数中，将编码或汉字写入到合成后，调用_SetCompositionDisplayAttributes()函数，为文本范围设置显示属性。

首先，获取合成中的文本范围。然后，为该文本范围设置显示属性。

```C++
BOOL CSampleIME::_SetCompositionDisplayAttributes(TfEditCookie ec, _In_ ITfContext *pContext, TfGuidAtom gaDisplayAttribute)
{
    ITfRange* pRangeComposition = nullptr;
    ITfProperty* pDisplayAttributeProperty = nullptr;
    HRESULT hr = S_OK;

    // we need a range and the context it lives in
    hr = _pComposition->GetRange(&pRangeComposition);
    if (FAILED(hr))
    {
        return FALSE;
    }

    hr = E_FAIL;

    // get our the display attribute property
    if (SUCCEEDED(pContext->GetProperty(GUID_PROP_ATTRIBUTE, &pDisplayAttributeProperty)))
    {
        VARIANT var;
        // set the value over the range
        // the application will use this guid atom to lookup the acutal rendering information
        var.vt = VT_I4; // we're going to set a TfGuidAtom
        var.lVal = gaDisplayAttribute; 

        hr = pDisplayAttributeProperty->SetValue(ec, pRangeComposition, &var);

        pDisplayAttributeProperty->Release();
    }

    pRangeComposition->Release();
    return (hr == S_OK);
}
```

输入法调用ITfProperty::SetValue()方法后，TSF管理器会获取ITfDisplayAttributeProvider接口，
调用ITfDisplayAttributeProvider::GetDisplayAttributeInfo()方法，获取显示属性信息。

## 3.38.3 清除

Interface			|Description
-|-
[ITfProperty][2]	|属性设置，由客户端(应用程序或文本服务)用来修改显示属性。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfProperty.md

在结束合成之前，输入法调用CSampleIME::_ClearCompositionDisplayAttributes()函数，清除合成显示属性。

```C++
void CSampleIME::_ClearCompositionDisplayAttributes(TfEditCookie ec, _In_ ITfContext *pContext)
{
    ITfRange* pRangeComposition = nullptr;
    ITfProperty* pDisplayAttributeProperty = nullptr;

    // get the compositon range.
    if (FAILED(_pComposition->GetRange(&pRangeComposition)))
    {
        return;
    }

    // get our the display attribute property
    if (SUCCEEDED(pContext->GetProperty(GUID_PROP_ATTRIBUTE, &pDisplayAttributeProperty)))
    {
        // clear the value over the range
        pDisplayAttributeProperty->Clear(ec, pRangeComposition);

        pDisplayAttributeProperty->Release();
    }

    pRangeComposition->Release();
}
```