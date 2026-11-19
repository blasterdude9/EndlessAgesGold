// dpguids.cpp - defines the DirectPlay8 GUID constants (CLSID_/IID_).
// The legacy dxguid.lib no longer carries DirectPlay8 GUIDs (DirectPlay was
// removed from Windows/DX SDKs). INITGUID makes the vendored headers' DEFINE_GUID
// lines emit the symbols so modules referencing DirectPlay link.
// NOTE: this only satisfies the LINKER. DirectPlay has no runtime on modern
// Windows; the networking transport must still be replaced (DirectPlay -> UDP).
#define INITGUID
#include <windows.h>
#include <initguid.h>
#include "dplay8.h"
#include "dpaddr.h"
#include "dplobby8.h"
