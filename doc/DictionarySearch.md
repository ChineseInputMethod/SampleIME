## 3.24 词典搜索

词典搜索，在词典中搜索编码为输入编码的汉字。

## 3.24.1 搜索词典

本工程将整个码表文件映射为了一维数组。<br>
首先，获取数组的长度和首地址。

```C++
BOOL CDictionarySearch::FindWorker(BOOL isTextSearch, _Out_ CDictionaryResult **ppdret, BOOL isWildcardSearch)
{
    DWORD_PTR dwTotalBufLen = GetBufferInWCharLength();         // in char
    if (dwTotalBufLen == 0)	{	...	}
    
    const WCHAR *pwch = GetBufferInWChar();
    DWORD_PTR indexTrace = 0;     // in char
    *ppdret = nullptr;
    BOOL isFound = FALSE;
    DWORD_PTR bufLenOneLine = 0;
...
```

然后从当前地址，读取剩余长度，至回车符。<br>
例如，第一次调用，当前地址就是首地址，剩余长度就是数组长度，从当前位置直到首个回车符为一行。<br>
第二次调用indexTrace += bufLenOneLine;dwTotalBufLen -= bufLenOneLine;<br>
也就是&pwch[indexTrace]偏移了bufLenOneLine个字符，从&pwch[indexTrace]开始，读取dwTotalBufLen长度，直到遇到首个回车符。<br>
（因为还要处理换行符，在FindNextLine:部分，还要多偏移一个字符）

```C++
...
TryAgain:
    bufLenOneLine = GetOneLine(&pwch[indexTrace], dwTotalBufLen);
...
```

接着调用第四个参数使用默认实参的ParseLine(&pwch[indexTrace], bufLenOneLine, &keyword)函数，获得文件中一行码表中的编码部分。

```C++
...
    if (bufLenOneLine == 0)	{	...	}
    else
    {
        CParserStringRange keyword;
        DWORD_PTR bufLen = 0;
        LPWSTR pText = nullptr;

        if (!ParseLine(&pwch[indexTrace], bufLenOneLine, &keyword))	{	...	}
...
```

然后进行通配符比较。如果通配符比较返回FALSE，则继续查找下一行码表。

```C++
...
        if (!isTextSearch)
        {
            // Compare Dictionary key code and input key code
            if (!isWildcardSearch)	{	...	}
            else
            {
                // Wildcard search
                if (!CStringRange::WildcardCompare(_locale, _pSearchKeyCode, &keyword))
                {
                    if (bufLen)	{	...	}
                    goto FindNextLine;
                }
            }
        }
        else	{	...	}

        if (bufLen)	{	...	}
...
```

如果通配符比较返回TRUE，则调用使用实参初始化第四个参数的ParseLine(&pwch[indexTrace], bufLenOneLine, &keyword, &valueStrings)函数。

```C++
...
        // Prepare return's CDictionaryResult
        *ppdret = new (std::nothrow) CDictionaryResult();
        if (!*ppdret)	{	...	}

        CSampleImeArray<CParserStringRange> valueStrings;
        if (!ParseLine(&pwch[indexTrace], bufLenOneLine, &keyword, &valueStrings))	{	...	}
...
```

然后将查找结果保存到搜索结果中。

```C++
...
        (*ppdret)->_FindKeyCode = keyword;
        (*ppdret)->_SearchKeyCode = *_pSearchKeyCode;

        for (UINT i = 0; i < valueStrings.Count(); i++)
        {
            CStringRange* findPhrase = (*ppdret)->_FindPhraseList.Append();
            if (findPhrase)
            {
                *findPhrase = *valueStrings.GetAt(i);
            }
        }

        // Seek to next line
        isFound = TRUE;
    }
...
```

如果找到了搜索结果，则退出。否则继续搜索。

```C++
...
FindNextLine:
    dwTotalBufLen -= bufLenOneLine;
    if (dwTotalBufLen == 0)
    {
        indexTrace += bufLenOneLine;
        _charIndex += indexTrace;

        if (!isFound && *ppdret)
        {
            delete *ppdret;
            *ppdret = nullptr;
        }
        return (isFound ? TRUE : FALSE);        // End of file
    }

    indexTrace += bufLenOneLine;
    if (pwch[indexTrace] == L'\r' || pwch[indexTrace] == L'\n' || pwch[indexTrace] == L'\0')
    {
        bufLenOneLine = 1;
        goto FindNextLine;
    }

    if (isFound)
    {
        _charIndex += indexTrace;
        return TRUE;
    }

    goto TryAgain;
}
```

本工程的候选列表默认搜索方式，在输入法中被称为省码或者简码。<br>
例如，输入编码“wom”，用户不用输全最后一个字的全部拼音，就可以显示候选词，被称为省码。<br>
这一方式与字处理软件的自动补全形式相同，但输入法的处理更加复杂。（本工程未表现这种复杂性）

响应用户操作提供候选字词，是输入法的核心内容。判断一个输入法优秀与否，其最具决定性的因素就是：<br>
用户是否能“XX”的输入TA想要输入的汉字。

这里的XX，不同的发明人，不同的用户，可能有不同的定义。也许舒适比较能描述这一技术特点。