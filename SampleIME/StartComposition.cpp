// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "Private.h"
#include "Globals.h"
#include "EditSession.h"
#include "SampleIME.h"

//+---------------------------------------------------------------------------
//
// CStartCompositinoEditSession
//
//----------------------------------------------------------------------------

class CStartCompositionEditSession : public CEditSessionBase
{
public:
    CStartCompositionEditSession(_In_ CSampleIME *pTextService, _In_ ITfContext *pContext) : CEditSessionBase(pTextService, pContext)
    {
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec);
};

//+---------------------------------------------------------------------------
//
// ITfEditSession::DoEditSession
//
//----------------------------------------------------------------------------

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
//     if (FAILED(pInsertAtSelection->InsertTextAtSelection(ec, 0, L"输出汉字的第一种方式\n", wcslen(L"输出汉字的第一种方式\n"), &pRangeInsert)))
   if (FAILED(pInsertAtSelection->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, NULL, 0, &pRangeInsert)))
    {
        goto Exit;
    }
//     pRangeInsert->Collapse(ec, TF_ANCHOR_END);
     /*{
         TF_SELECTION tfSelection;
         tfSelection.range = pRangeInsert;
         tfSelection.range->SetText(ec, 0, L"输出汉字的第二种方式\n", wcslen(L"输出汉字的第二种方式\n"));
         tfSelection.range->Collapse(ec, TF_ANCHOR_END);
         _pContext->SetSelection(ec, 1, &tfSelection);
     }*/
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
        /*{
            const ULONG cchMax = 256;
            ULONG pcch = 0;
            WCHAR pchText[cchMax];
            tfSelection.range->GetText(
                ec,
                TF_TF_MOVESTART,//TF_TF_IGNOREEND
                pchText,
                cchMax,
                &pcch
            );
            pchText[pcch] = '\0';
        }*/
        _pContext->SetSelection(ec, 1, &tfSelection);
        _pTextService->_SaveCompositionContext(_pContext);
    }

Exit:
    if (nullptr != pContextComposition)
    {
        pContextComposition->Release();
    }

    if (nullptr != pRangeInsert)
    {
        pRangeInsert->Release();
    }

    if (nullptr != pInsertAtSelection)
    {
        pInsertAtSelection->Release();
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////
//
// CSampleIME class
//
//////////////////////////////////////////////////////////////////////

//+---------------------------------------------------------------------------
//开始合成
// _StartComposition
//
// this starts the new (std::nothrow) composition at the selection of the current 
// focus context.
//----------------------------------------------------------------------------

void CSampleIME::_StartComposition(_In_ ITfContext *pContext)
{
    CStartCompositionEditSession* pStartCompositionEditSession = new (std::nothrow) CStartCompositionEditSession(this, pContext);

    if (nullptr != pStartCompositionEditSession)
    {
        HRESULT hr = S_OK;
        pContext->RequestEditSession(_tfClientId, pStartCompositionEditSession, TF_ES_SYNC | TF_ES_READWRITE, &hr);

        pStartCompositionEditSession->Release();
    }
}

//+---------------------------------------------------------------------------
//
// _SaveCompositionContext
//
// this saves the context _pComposition belongs to; we need this to clear
// text attribute in case composition has not been terminated on 
// deactivation
//----------------------------------------------------------------------------

void CSampleIME::_SaveCompositionContext(_In_ ITfContext *pContext)
{
    assert(_pContext == nullptr);

    pContext->AddRef();
    _pContext = pContext;
} 
