## 3.40 显示属性信息

显示属性信息对象，为应用程序提供显示属性信息。

输入法需要实现ITfDisplayAttributeInfo接口，提供显示属性信息对象。

## 3.40.1 显示属性信息对象

Interface						|Description
-|-
[ITfDisplayAttributeInfo][1]	|显示属性信息对象，为应用程序提供显示属性信息。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfDisplayAttributeInfo.md

```C++
STDAPI CDisplayAttributeInfo::GetAttributeInfo(_Out_ TF_DISPLAYATTRIBUTE *ptfDisplayAttr)
{
    if (ptfDisplayAttr == nullptr)
    {
        return E_INVALIDARG;
    }

    // return the default display attribute.
    *ptfDisplayAttr = *_pDisplayAttribute;

    return S_OK;
}
```