## 3.12 合成处理器引擎

在输入法被激活时，调用CSampleIME::_AddTextProcessorEngine()方法，设置输入法配置。

## 3.12.1 获取语言配置文件

首先获取输入法的语言配置文件。

```C++
    if (FAILED(profile.CreateInstance()))
    {
        return FALSE;
    }

    if (FAILED(profile.GetCurrentLanguage(&langid)))
    {
        return FALSE;
    }

    if (FAILED(profile.GetDefaultLanguageProfile(langid, GUID_TFCAT_TIP_KEYBOARD, &clsid, &guidProfile)))
    {
        return FALSE;
    }
```

## 3.12.2 创建合成处理器引擎

然后创建合成处理器引擎，这是输入法的核心类。

```C++
    if (_pCompositionProcessorEngine == nullptr)
    {
        _pCompositionProcessorEngine = new (std::nothrow) CCompositionProcessorEngine();
    }
```

## 3.12.3 设置语言配置文件

最后调用_pCompositionProcessorEngine->SetupLanguageProfile()方法，设置输入法的语言配置文件。

Function							|Description
-|-
SetupPreserved()					|设置输入法状态开关快捷键。
InitializeSampleIMECompartment()	|将输入法默认状态设置到缓冲区。
SetupPunctuationPair()				|设置标点对。
SetupLanguageBar()					|设置输入法状态开关。
SetupKeystroke()					|设置编码键。
SetupConfiguration()				|设置候选栏。
SetupDictionaryFile()				|打开输入法词典文件。

## 3.12.4 完成初始化

调用_AddTextProcessorEngine()完成后，ActivateEx()方法退出，输入法完成初始化。
然后根据配置，开始进入工作状态。

首先ITfActiveLanguageProfileNotifySink::OnActivated()方法被调用。这是输入法做为普通应用程序响应键盘布局激活事件。
然后ITfLangBarItemButton接口被调用，这是因为输入法在语言栏中安装了三个按钮，语言栏调用按钮信息。
至此，输入法进入工作状态，等待响应事件。

## 3.12.5 获取候选列表

当用户按下编码键后，输入法调用CCompositionProcessorEngine::GetCandidateList()函数，获取候选列表。

首先，在编码后面添加一个通配符，只是输入法做的一个简单处理，例如没有拼音为“b”的编码，所以当用户按下b后，候选列表是空的，添加一个通配符防止出现这种情况。<br>
然后，输入法调用CTableDictionaryEngine::CollectWordForWildcard()函数，获取候选列表。<br>
接着，调用CBaseDictionaryEngine::SortListItemByFindKeyCode()函数，为这个候选列表排序。<br>
最后，清除候选列表项编码的通配符。

```C++
void CCompositionProcessorEngine::GetCandidateList(_Inout_ CSampleImeArray<CCandidateListItem> *pCandidateList, BOOL isIncrementalWordSearch, BOOL isWildcardSearch)
{
    if (!IsDictionaryAvailable())
    {
        return;
    }

    if (isIncrementalWordSearch)
    {
        CStringRange wildcardSearch;
        DWORD_PTR keystrokeBufLen = _keystrokeBuffer.GetLength() + 2;
        PWCHAR pwch = new (std::nothrow) WCHAR[ keystrokeBufLen ];
        if (!pwch)
        {
            return;
        }

        // check keystroke buffer already has wildcard char which end user want wildcard serach
        DWORD wildcardIndex = 0;
        BOOL isFindWildcard = FALSE;

        if (IsWildcard())
        {
            for (wildcardIndex = 0; wildcardIndex < _keystrokeBuffer.GetLength(); wildcardIndex++)
            {
                if (IsWildcardChar(*(_keystrokeBuffer.Get() + wildcardIndex)))
                {
                    isFindWildcard = TRUE;
                    break;
                }
            }
        }

        StringCchCopyN(pwch, keystrokeBufLen, _keystrokeBuffer.Get(), _keystrokeBuffer.GetLength());

        if (!isFindWildcard)
        {
            // add wildcard char for incremental search
            StringCchCat(pwch, keystrokeBufLen, L"*");
        }

        size_t len = 0;
        if (StringCchLength(pwch, STRSAFE_MAX_CCH, &len) == S_OK)
        {
            wildcardSearch.Set(pwch, len);
        }
        else
        {
            return;
        }

        _pTableDictionaryEngine->CollectWordForWildcard(&wildcardSearch, pCandidateList);

        if (0 >= pCandidateList->Count())
        {
            return;
        }

        if (IsKeystrokeSort())
        {
            _pTableDictionaryEngine->SortListItemByFindKeyCode(pCandidateList);
        }

        // Incremental search would show keystroke data from all candidate list items
        // but wont show identical keystroke data for user inputted.
        for (UINT index = 0; index < pCandidateList->Count(); index++)
        {
            CCandidateListItem *pLI = pCandidateList->GetAt(index);
            DWORD_PTR keystrokeBufferLen = 0;

            if (IsWildcard())
            {
                keystrokeBufferLen = wildcardIndex;
            }
            else
            {
                keystrokeBufferLen = _keystrokeBuffer.GetLength();
            }

            CStringRange newFindKeyCode;
            newFindKeyCode.Set(pLI->_FindKeyCode.Get() + keystrokeBufferLen, pLI->_FindKeyCode.GetLength() - keystrokeBufferLen);
            pLI->_FindKeyCode.Set(newFindKeyCode);
        }

        delete [] pwch;
    }
    else if (isWildcardSearch)
    {
        _pTableDictionaryEngine->CollectWordForWildcard(&_keystrokeBuffer, pCandidateList);
    }
    else
    {
        _pTableDictionaryEngine->CollectWord(&_keystrokeBuffer, pCandidateList);
    }

    for (UINT index = 0; index < pCandidateList->Count();)
    {
        CCandidateListItem *pLI = pCandidateList->GetAt(index);
        CStringRange startItemString;
        CStringRange endItemString;

        startItemString.Set(pLI->_ItemString.Get(), 1);
        endItemString.Set(pLI->_ItemString.Get() + pLI->_ItemString.GetLength() - 1, 1);

        index++;
    }
}
```