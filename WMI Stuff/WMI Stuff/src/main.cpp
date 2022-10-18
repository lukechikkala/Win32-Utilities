#include <iostream>
#include <WbemIdl.h>
#include <windows.h>
#include <comdef.h>


#pragma comment( lib, "wbemuuid.lib" )

//#include "include/Lib_InitializeCOM_LC.cpp"
//#include "include/Lib_SetupWBEM_LC.cpp"

void InitializeCOM_LC()
{
    // ----------------------------------------------------------------------------------------------------
    HRESULT h_Result_LC;
    h_Result_LC = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to initialize COM Lib." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
    }

    // ----------------------------------------------------------------------------------------------------
    h_Result_LC = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to initialize security." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
        CoUninitialize();
    }
}

void SetupWBEM_LC(IWbemLocator*& pLoc_LC, IWbemServices*& pSvc_LC)
{
    HRESULT h_Result_LC;

    h_Result_LC = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc_LC);

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to create IWbemLocator object." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
        CoUninitialize();
    }

    h_Result_LC = pLoc_LC->ConnectServer(
        _bstr_t(L"ROOT\\microsoft\\windows\\storage"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc_LC
    );

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to connect." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
        pLoc_LC->Release();
        CoUninitialize();
    }

    h_Result_LC = CoSetProxyBlanket(
        pSvc_LC,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to set proxy blanket." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
        pSvc_LC->Release();
        pLoc_LC->Release();
        CoUninitialize();
    }
}

int main()
{
    HRESULT h_Result_LC;
    IWbemLocator *wb_Locator_LC = NULL;
    IWbemServices *wb_Services_LC = NULL;

    std::cout << " Hello World " << std::endl;

    InitializeCOM_LC();
    std::cout << " COM has been initialized " << std::endl;

    SetupWBEM_LC( wb_Locator_LC, wb_Services_LC );
    std::cout << " SetupWBEM has been initialized" << std::endl;

    IEnumWbemClassObject* storageEmulator_LC = NULL;

    h_Result_LC = wb_Services_LC->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM MSFT_PhysicalDisk"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &storageEmulator_LC
    );

    if (FAILED(h_Result_LC))
    {
        std::cout << " Failed to set proxy blanket." << std::endl;
        std::cout << " Error Code: 0x" << std::hex << h_Result_LC << std::endl;
        wb_Services_LC->Release();
        wb_Locator_LC->Release();
        CoUninitialize();
    }

    IWbemClassObject* storageWbemObject_LC = NULL;
    ULONG uReturn = 0;
    while (storageEmulator_LC)
    {
        HRESULT hr_LC = storageEmulator_LC->Next(WBEM_INFINITE, 1, & storageWbemObject_LC, & uReturn);
        if (0 == uReturn)
        {
            break;
        }
    }

    VARIANT deviceID;
    VARIANT busType;
    VARIANT healthStatus;
    VARIANT spindleSpeed;
    VARIANT mediaType;

    storageWbemObject_LC->Get(L"DeviceID"     , 0, &deviceID     , 0, 0);
    storageWbemObject_LC->Get(L"BusType"      , 0, &busType      , 0, 0);
    storageWbemObject_LC->Get(L"HealthStatus" , 0, &healthStatus , 0, 0);
    storageWbemObject_LC->Get(L"SpindleSpeed" , 0, &spindleSpeed , 0, 0);
    storageWbemObject_LC->Get(L"MediaType"    , 0, &mediaType    , 0, 0);

    std::cin.get();
}