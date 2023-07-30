## 3.3 COM导出函数

Server.cpp提供了TSF输入法作为COM组件的标准实现。

## 3.3.1 注册输入法

注册COM组件时，DllRegisterServer()函数被调用。在此函数中实现了注册输入法。

```C++
STDAPI DllRegisterServer(void)
{
    if ((!RegisterServer()) || (!RegisterProfiles()) || (!RegisterCategories()))
    {
        DllUnregisterServer();
        return E_FAIL;
    }
    return S_OK;
}
```

## 3.3.2 创建输入法实例

创建COM对象时，DllGetClassObject()函数被调用。通过调用CSampleIME::CreateInstance()，创建输入法实例。

```C++
void BuildGlobalObjects(void)
{
    classFactoryObjects[0] = new (std::nothrow) CClassFactory(Global::SampleIMECLSID, CSampleIME::CreateInstance);
}
```