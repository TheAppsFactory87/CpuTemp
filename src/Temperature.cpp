#include "Temperature.h"
#ifdef _WIN32
    #include <comdef.h>
    #include <Wbemidl.h>
    #pragma comment(lib, "wbemuuid.lib")
#endif

double Temperature::readValue()
{
    double value = 0;
#ifdef _WIN32
    // Windows-specific implementation using WMI
    // Simplify error handling and ensure proper cleanup
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "COM initialization failed.\n";
        return -1;
    }

    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

    if (FAILED(hres)) {
        std::cerr << "Failed to create IWbemLocator object.\n";
        CoUninitialize();
        return -1;
    }

    IWbemServices *pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\WMI"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

    if (FAILED(hres)) {
        std::cerr << "Could not connect to WMI namespace.\n";
        pLoc->Release();
        CoUninitialize();
        return -1;
    }

    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT * FROM MSAcpi_ThermalZoneTemperature"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumerator);

    if (FAILED(hres)) {
        std::cerr << "Query failed.\n";
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return -1;
    }

    IWbemClassObject *pclsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (0 == uReturn) break;

        VARIANT vtProp;
        hr = pclsObj->Get(L"CurrentTemperature", 0, &vtProp, 0, 0);
        if (SUCCEEDED(hr)) {
            value = (vtProp.uintVal / 10.0) - 273.15; // Convert to Celsius
            VariantClear(&vtProp);
        }
        pclsObj->Release();
    }

    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

#else
    // Linux-specific implementation for Raspberry Pi
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (file.is_open()) {
        file >> value;
        value /= 1000; // Convert to Celsius
        file.close();
    } else {
        std::cerr << "Unable to read temperature from /sys/class/thermal/thermal_zone0/temp\n";
        return -1;
    }
#endif
    return value;
}