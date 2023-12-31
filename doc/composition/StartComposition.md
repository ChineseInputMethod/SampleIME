## 3.35 开始合成

开始合成，在上下文中创建一个合成。

## 3.35.1 输出汉字的第一种方式

将StartComposition.cpp文件的第48行注释掉，取消第47行和52行的注释。
将CStartCompositionEditSession::DoEditSession()函数修改为下图所示。

![输出汉字的第一种方式](输出汉字的第一种方式.png)

在字处理软件中激活本工程输入法，按下编码键，字符串“输出汉字的第一种方式”会被插入到当前输入焦点。
这是输出汉字的第一种方式。

## 3.35.2 输出汉字的第二种方式

取消53行到59行的注释，将CStartCompositionEditSession::DoEditSession()函数修改为下图所示。

![输出汉字的第二种方式](输出汉字的第二种方式.png)

在字处理软件中激活本工程输入法，按下编码键，字符串“输出汉字的第二种方式”会被插入到当前输入焦点。
这是输出汉字的第二种方式。

可以看到第54行到58行代码，与70行到75行代码相似。这是TSF输入法输出汉字与第二种输出汉字方式的相似之处。
不同的是，TSF输入法调用ITfContextComposition::StartComposition()方法，在上下文中创建了一个ITfComposition合成。

## 3.35.3 什么是合成

类似于页面布局，元素被放在标签内，用标记语言进行排版。铅字印刷时代对文字也需要进行一定的排版。
或是因为字体的不同，或是因为磨损的原因，需要对字模在xyz轴上进行调整。如果是以字母为单位，效率不高，效果也不好。
所以一般都是以单词为单位，调整页面布局。

早期的字处理软件，类似于Markdown这样的语法，有可见字符，也有控制字符。控制字符控制可见字符的显示。
这种模式非常方便字母文字的输入。但是输入东亚文字则有一点不便。（作者只了解一点日韩文字）

ucdos时代，页面底部有个控制条，可以在控制条里输入编码，选择想要输出的汉字。这种方式显然不适合字母文字的录入方式。字母文字在编辑区直接可以输出字母。
日韩文字就夹在这两种模式之间。

在Windows 95系统中，提出了合成概念。与现在TSF合成概念不同的是，win95中的合成，包含了输入编码的多种状态。
具体的讲，就是输入文字的编码时，此时是输入状态。结束输入编码后，自动或手动转换为想要输入的文字，此时是转换状态。
如果输入的是系统为收录的自造词，还需要管理已转换编码和未转换编码。

作者当时并不懂这些，很长时间都搞不懂，怎么输出一个汉字。现在才明白，如果从日韩输入法的角度看，就好理解了。
因为日韩文字一般不需要候选列表，罗马字拼写与文字是强一一对应关系。所以在编辑区直接编辑其不同状态，是非常便捷直观的。

而汉字输入法中的形码输入法，其编码与文字的对应关系是附会的。在编辑区出现汉字的输入编码就显得有点突兀。
另一方面，汉字输入法中的拼音输入法，存在着无法完全避免的同音字，所以在编码之后拖着一个候选窗口。

当时，作者很多都想不明白的事，现在都明白了其背后的原因。例如智能ABC输入完编码，需要按下空格才出现其他候选字词。
这应该和注音输入法和日韩输入法有关，将候选窗口隐藏了起来。
例如形码输入法，追求一个顶屏功能（输入完全码，如果没有重码字，自动上屏）。这有意或者无意的受了日韩输入法的影响。

什么是合成？

就是在上下文中，将当前正在输入的内容，添加一个属性，表示这段内容是待确定的。
当这段内容完成了从编码到文字的转换，就从这段内容中删除这个待确定属性。这段内容正式被添加到上下文中（其实一直在上下文中，只不过去掉了这个待确定属性）。

这段内容，在TSF术语里就是ITfRange。这个待确定属性就是ITfComposition。
TSF输入法调用ITfContextComposition::StartComposition()方法，在上下文中创建了一个ITfComposition合成。
就是在上下文中，将当前正在输入的内容，添加一个属性，表示这段内容是待确定的。

## 3.35.4 开始合成

Interface					|Description
-|-
[ITfInsertAtSelection][1]	|在选定位置插入内容，用于在上下文中插入文本或嵌入对象。
[ITfContextComposition][2]	|上下文合成，用于创建ITfComposition合成。

[1]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfInsertAtSelection.md
[2]: https://github.com/ChineseInputMethod/Interface/blob/master/TSFmanager/ITfContextComposition.md

当用户按下编码键，如果输入状态不是处于合成状态，那么创建一个编辑会话。在编辑会话中创建一个合成。

首先，获取在选定位置插入内容接口；
然后，调用ITfInsertAtSelection::InsertTextAtSelection()方法，通过插入一个空字符，获得ITfRange文本范围；
接着，获取上下文合成接口；
然后，调用ITfContextComposition::StartComposition()方法，将所选文本范围标记为GUID_PROP_COMPOSING属性；
最后，调用ITfContext::SetSelection()方法，将标记后的文本范围设置为上下文中的选定内容。

```C++
STDAPI CStartCompositionEditSession::DoEditSession(TfEditCookie ec)
{
    ITfInsertAtSelection* pInsertAtSelection = nullptr;
    ITfRange* pRangeInsert = nullptr;
    ITfContextComposition* pContextComposition = nullptr;
    ITfComposition* pComposition = nullptr;

    if (FAILED(_pContext->QueryInterface(IID_ITfInsertAtSelection, (void **)&pInsertAtSelection)))
    {
        goto Exit;
    }

   if (FAILED(pInsertAtSelection->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, NULL, 0, &pRangeInsert)))
    {
        goto Exit;
    }

    if (FAILED(_pContext->QueryInterface(IID_ITfContextComposition, (void **)&pContextComposition)))
    {
        goto Exit;
    }

    if (SUCCEEDED(pContextComposition->StartComposition(ec, pRangeInsert, _pTextService, &pComposition)) && (nullptr != pComposition))
    {
        _pTextService->_SetComposition(pComposition);

        // set selection to the adjusted range
        TF_SELECTION tfSelection;
        tfSelection.range = pRangeInsert;
        tfSelection.style.ase = TF_AE_NONE;
        tfSelection.style.fInterimChar = FALSE;

        _pContext->SetSelection(ec, 1, &tfSelection);
        _pTextService->_SaveCompositionContext(_pContext);
    }

...
```