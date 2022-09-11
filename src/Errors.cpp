#include <mndErrors.h>

bool mndCHECK(HRESULT hr){


	switch (hr) {

	case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
		cout("DXGI_ERROR_NOT_CURRENTLY_AVAILABLE");
		cout("The resource or request is not currently available, but it might become available later.");
		cout("you call it in a Session 0 process");

		break;

	case E_INVALIDARG:
		cout("E_INVALIDARG");

		cout("you set the pAdapter parameter to a non-NULL value and the DriverType parameter to the D3D_DRIVER_TYPE_HARDWARE value");
		break;

	case  DXGI_ERROR_SDK_COMPONENT_MISSING:
		cout("DXGI_ERROR_SDK_COMPONENT_MISSING");
		cout("you specify D3D11_CREATE_DEVICE_DEBUG in Flags and the incorrect version of the debug layer is installed on your computer.\n"
			" Install the latest Windows SDK to get the correct version");
		cout("The operation depends on an SDK component that is missing or mismatched.");
		break;

	case  DXGI_ERROR_ACCESS_DENIED:
		cout("DXGI_ERROR_ACCESS_DENIED");

		cout("You tried to use a resource to which you did not have the required access privileges.\n"
			" This error is most typically caused when you write to a shared resource with read-only access.");
		break;

	case DXGI_ERROR_ACCESS_LOST:
		cout("DXGI_ERROR_ACCESS_LOST");

		cout("The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.");
		break;

	case DXGI_ERROR_ALREADY_EXISTS:
		cout("DXGI_ERROR_ALREADY_EXISTS");

		cout("The desired element already exists. This is returned by DXGIDeclareAdapterRemovalSupport if it is not the first time that the function is called");
		break;
	case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
		cout("DXGI_ERROR_CANNOT_PROTECT_CONTENT");

		cout("DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.");
		break;
	case DXGI_ERROR_DEVICE_HUNG:
		cout("DXGI_ERROR_DEVICE_HUNG");

		cout("The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.");
		break;
	case DXGI_ERROR_DEVICE_REMOVED:
		cout("DXGI_ERROR_DEVICE_REMOVED");
		cout("he video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason()\n");
		

		break;
	case DXGI_ERROR_DEVICE_RESET:
		cout("DXGI_ERROR_DEVICE_RESET");

		cout("The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device");
		break;
	case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
		cout("DXGI_ERROR_DRIVER_INTERNAL_ERROR");

		cout("The driver encountered a problem and was put into the device removed state");
		break;
	case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
		cout("DXGI_ERROR_FRAME_STATISTICS_DISJOINT");

		cout("An event (for example, a power cycle) interrupted the gathering of presentation statistics.");
		break;
	case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
		cout("DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE");

		cout("The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.");
		break;

	case DXGI_ERROR_INVALID_CALL:
		cout("DXGI_ERROR_INVALID_CALL");

		cout("The application provided invalid parameter data; this must be debugged and fixed before the application is released.");
		break;

	case DXGI_ERROR_MORE_DATA:
		cout("DXGI_ERROR_MORE_DATA");

		cout("The buffer supplied by the application is not big enough to hold the requested data.");
		break;

	case DXGI_ERROR_NAME_ALREADY_EXISTS:
		cout("DXGI_ERROR_NAME_ALREADY_EXISTS");

		cout("The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle() is already associated with some other resource.");
		break;

	case DXGI_ERROR_NONEXCLUSIVE:
		cout("DXGI_ERROR_NONEXCLUSIVE");

		cout("A global counter resource is in use, and the Direct3D device can't currently use the counter resource.");
		break;

	case DXGI_ERROR_NOT_FOUND:
		cout("DXGI_ERROR_NOT_FOUND");

		cout("When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface. When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.");
		break;

	case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:
		cout("DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED");

		cout("DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED");
		break;
	case DXGI_ERROR_REMOTE_OUTOFMEMORY:
		cout("DXGI_ERROR_REMOTE_OUTOFMEMORY");

		cout("DXGI_ERROR_REMOTE_OUTOFMEMORY");
		break;
	case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:

		cout("DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE\n"
			"The DXGI output (monitor) to which the swap chain content was restricted is now disconnected or changed.");
		break;

	case DXGI_ERROR_SESSION_DISCONNECTED:
		cout("DXGI_ERROR_SESSION_DISCONNECTED");

		cout("The Remote Desktop Services session is currently disconnected.");
		break;
	case DXGI_ERROR_UNSUPPORTED:
		cout("DXGI_ERROR_UNSUPPORTED\n"
			"The requested functionality is not supported by the device or the driver.");
		break;
	case DXGI_ERROR_WAIT_TIMEOUT:
		cout("DXGI_ERROR_WAIT_TIMEOUT\n"
			"The time-out interval elapsed before the next desktop frame was available.");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		cout("DXGI_ERROR_WAS_STILL_DRAWING\n"
			"The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.");
		break;
	case D3D11_ERROR_FILE_NOT_FOUND:
		cout("D3D11_ERROR_FILE_NOT_FOUND");
		break;

	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		cout("D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS");
		cout("There are too many unique instances of a particular type of state object.");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		cout("D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS");
		cout("There are too many unique instances of a particular type of view object.");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		cout("D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD");
		cout("The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.");
		break;
	case E_FAIL:
		cout("E_FAIL");
		cout("Attempted to create a device with the debug layer enabled and the layer is not installed.");
		break;

	case E_OUTOFMEMORY:
		cout("E_OUTOFMEMORY");
		cout("Direct3D could not allocate sufficient memory to complete the call.");
		break;
	case E_NOTIMPL:
		cout("E_NOTIMPL");
		cout("The method call isn't implemented with the passed parameter combination.");
		break;
	case S_FALSE:
		cout("S_FALSE");
		cout("Alternate success value, indicating a successful but nonstandard completion (the precise meaning depends on context).");
		break;
	

	default:
		return true;

	};

	return false;
}

