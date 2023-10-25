## 3.39 显示属性提供者

显示属性提供者，由TSF管理器用来枚举和获取单个显示属性信息对象。

如果输入法调用了ITfProperty::SetValue()方法，那么TSF管理器会向输入法查询ITfDisplayAttributeProvider接口。

## 3.39.1 提供显示属性

Interface							|Description
-|-
[ITfDisplayAttributeProvider][1]	|显示属性提供者，由TSF管理器用来枚举和获取单个显示属性信息对象。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TextService/ITfDisplayAttributeProvider.md

如果输入法调用了ITfProperty::SetValue()方法，那么TSF管理器会调用ITfDisplayAttributeProvider::GetDisplayAttributeInfo()方法。

在ITfDisplayAttributeProvider::GetDisplayAttributeInfo()方法中，输入法向TSF管理器提供ITfDisplayAttributeInfo显示属性信息组件对象。

```C++
STDAPI CSampleIME::GetDisplayAttributeInfo(__RPC__in REFGUID guidInfo, __RPC__deref_out_opt ITfDisplayAttributeInfo **ppInfo)
{
    if (ppInfo == nullptr)
    {
        return E_INVALIDARG;
    }

    *ppInfo = nullptr;

    // Which display attribute GUID?
    if (IsEqualGUID(guidInfo, Global::SampleIMEGuidDisplayAttributeInput))
    {
        *ppInfo = new (std::nothrow) CDisplayAttributeInfoInput();
        if ((*ppInfo) == nullptr)
        {
            return E_OUTOFMEMORY;
        }
    }
    else if (IsEqualGUID(guidInfo, Global::SampleIMEGuidDisplayAttributeConverted))
    {
        *ppInfo = new (std::nothrow) CDisplayAttributeInfoConverted();
        if ((*ppInfo) == nullptr)
        {
            return E_OUTOFMEMORY;
        }
    }
    else
    {
        return E_INVALIDARG;
    }


    return S_OK;
}
```