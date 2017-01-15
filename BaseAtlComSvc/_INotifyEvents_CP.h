#pragma once

using namespace ATL;

template <class T>
class CProxy_INotifyEvents : public IConnectionPointImpl<T, &__uuidof( _INotifyEvents ), CComDynamicUnkArray>
{
public:
    HRESULT Fire_NotifyValueUpdated(ULONG lValue , VARIANT_BOOL* Cancel)
    {
        HRESULT hr = S_OK;
        T * pThis = static_cast<T *>(this);
        int cConnections = m_vec.GetSize();

        for (int iConnection = 0; iConnection < cConnections; iConnection++)
        {
            pThis->Lock();
            CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
            pThis->Unlock();

            IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

            if (pConnection)
            {
                CComVariant avarParams[2];
                avarParams[1] = lValue;
                avarParams[1].vt = VT_UI4;
                avarParams[0].byref = Cancel;
                avarParams[0].vt = VT_BOOL | VT_BYREF;
                DISPPARAMS params = { avarParams, NULL, 2, 0 };
                hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, NULL, NULL, NULL);
            }
        }

        return hr;
    }
};
