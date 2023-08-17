## 3.4 注册输入法

注册TSF输入法分为三步。
RegisterServer()函数在注册表中注册COM组件。
RegisterProfiles()函数注册文字服务及配置。
RegisterCategories()函数注册输入法类别。

## 3.4.1 注册COM组件

执行RegisterServer()函数后，会在注册表创建以下键及键值：

```C++
HKEY_CLASSES_ROOT\CLSID\{D2291A80-84D8-4641-9AB2-BDD1472C846B}\InProcServer32
```

## 3.4.2 注册文本服务和配置文件

Interface							|Description
-|-
[ITfInputProcessorProfileMgr][1]	|文本服务语言配置管理器，管理键盘布局和文本服务，建议使用此接口注册输入法。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfInputProcessorProfileMgr.md

执行RegisterProfiles()函数后，会在注册表创建以下键及键值：

```C++
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\CTF\TIP\{D2291A80-84D8-4641-9AB2-BDD1472C846B}\LanguageProfile\0x00000804\{83955C0E-2C09-47A5-BCF3-F2B98E11EE8B}
```

## 3.4.3 注册输入法类别

Interface			|Description
-|-
[ITfCategoryMgr][2]	|类别管理器，为输入法注册类别。

[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfCategoryMgr.md

执行RegisterCategories()函数后，会在注册表创建以下键及键值：

```C++
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\CTF\TIP\{D2291A80-84D8-4641-9AB2-BDD1472C846B}\Category
```