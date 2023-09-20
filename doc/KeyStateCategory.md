## 3.21 按键分类

按键分类，按照按键分类调用相应处理函数。

## 3.21.1 当前工作状态

按照当前工作状态，创建相应的扩展类。

```C++
CKeyStateCategory* CKeyStateCategoryFactory::MakeKeyStateCategory(KEYSTROKE_CATEGORY keyCategory, _In_ CSampleIME *pTextService)
{
    CKeyStateCategory* pKeyState = nullptr;

    switch (keyCategory)
    {
    case CATEGORY_NONE:
        pKeyState = new (std::nothrow) CKeyStateNull(pTextService);
        break;

    case CATEGORY_COMPOSING:
        pKeyState = new (std::nothrow) CKeyStateComposing(pTextService);
        break;

    case CATEGORY_CANDIDATE:
        pKeyState = new (std::nothrow) CKeyStateCandidate(pTextService);
        break;

    case CATEGORY_PHRASE:
        pKeyState = new (std::nothrow) CKeyStatePhrase(pTextService);
        break;

    default:
        pKeyState = new (std::nothrow) CKeyStateNull(pTextService);
        break;
    }
    return pKeyState;
}
```