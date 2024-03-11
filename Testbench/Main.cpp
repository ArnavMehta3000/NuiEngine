#include <Core/EntryPoint.h>
#include <Core/Utils/Log.h>

NUI_MAIN()
{
	NUI_LOG(Debug, Main, "Debug Log!");
	NUI_LOG(Info, Main, "Info Log!");
	NUI_LOG(Warn, Main, "Warn Log!");
	NUI_LOG(Error, Main, "Error Log!");
	//NUI_LOG(Fatal, Main, "Fatal Log!");

	HRESULT hr = E_FAIL;
	NUI_ASSERT(SUCCEEDED(hr), "Number is not 3");
	return 0;
}