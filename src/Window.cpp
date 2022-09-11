
#include "mndWindow.h"
#include "WindowClass.h"

wchar_t Window::windowClassName[100]={L"WindowClass"} ;

LRESULT CALLBACK Protocol(HWND handel, unsigned int msg, WPARAM w, LPARAM l) {
    LONG_PTR ptr = GetWindowLongPtr(handel,GWLP_USERDATA);
    return ((Window*)ptr)->EvetProtocol(handel, msg, w, l);
}

LRESULT CALLBACK setProtocol(HWND handel, unsigned int msg, WPARAM w, LPARAM l) {
    if (msg == WM_CREATE) {
        
        SetWindowLongPtr(handel, GWLP_WNDPROC, (LONG_PTR)Protocol);
        SetWindowLongPtr(handel,GWLP_USERDATA,(LONG_PTR)((CREATESTRUCT*)l)->lpCreateParams);

        return DefWindowProc(handel, msg, w, l);
    }
    return DefWindowProc( handel, msg, w, l);
}

Window::Window( const wchar_t* titel1, int width, int height)
	: width(width), height(height),keyBoardEvent(),mouseEvent(), hInstance(GetModuleHandleA(0)), lastMousX(0), lastMouseY(0), lastState(0)
{
    cout(this->keyBoardEvent.a);

    wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = setProtocol;
	wc.lpszClassName = windowClassName;
	wc.style = CS_OWNDC;

	RegisterClassEx(&wc);

	RECT rect{
	    200,200,rect.left+width,rect.top+height
	};
    AdjustWindowRect(&rect,WS_CAPTION|WS_MAXIMIZEBOX|WS_SYSMENU, false);

	windowhandel = CreateWindowEx(WS_EX_ACCEPTFILES, windowClassName, titel, WS_SYSMENU  | WS_MINIMIZEBOX | WS_VISIBLE ,
		200, 200,
		rect.right-rect.left,
		rect.bottom - rect.top,
		0, 0, hInstance, this);

	ShowWindow(windowhandel, SW_SHOW);

    //creating graphics object
    pgfx = new Graphics(windowhandel,width,height);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplDX11_Init(pgfx->getdevice(), pgfx->getcontext());
    ImGui_ImplWin32_Init(windowhandel);

}

Window::~Window()
{
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();

    delete pgfx;
	UnregisterClass(windowClassName, hInstance);
	DestroyWindow(windowhandel);
}



wchar_t* Window::getName()
{
	return windowClassName;
}


int Window::getWidth() 
{
	return width;
}

int Window::getHeight() 
{
	return height;
}

void Window::setTitel(const wchar_t *name) {
    SetWindowText(windowhandel,name);
}

LRESULT CALLBACK Window::EvetProtocol(HWND handel, unsigned int msg, WPARAM wp, LPARAM lp) {


    if (ImGui_ImplWin32_WndProcHandler(handel,msg,wp,lp)) 
    {
        return DefWindowProc(handel, msg, wp, lp);
    }


    POINTS p;
    int delta;

    switch (msg)
    {
    case WM_CLOSE:
        
        PostQuitMessage(100);
        std::cout << "close message\n";
        break;

        //----------------------------------------------------------------
    // key board
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE) {
            PostQuitMessage(100);
            return 0;
        }

         keyBoardEvent.pushKeyDownMsg(wp);
        break;
    case WM_KEYUP:

        keyBoardEvent.pushKeyUpMsg(wp);
        break;
    case WM_CHAR:

        if (wp != VK_BACK)
            keyBoardEvent.pushCharMsg(wp);
        break;
        //------------------------------------------------------------------------
                     // mouse
    case WM_MOUSEMOVE:
        p = MAKEPOINTS(lp);

        if (p.x <= getWidth() and p.y <= getHeight() and p.x >= 0 and p.y >= 0) {
            mouseEvent.onMove(p.x, p.y);

            SetCapture(handel);
        }
        else if (wp & (MK_LBUTTON | MK_RBUTTON)) {
            mouseEvent.onMove(p.x, p.y);
        }
        else {
            ReleaseCapture();
        }
        break;

    
    case WM_LBUTTONDOWN:
        p = MAKEPOINTS(lp);
        mouseEvent.onLeftPress(p.x, p.y,wp);
        break;

    case WM_LBUTTONUP:
        p = MAKEPOINTS(lp);
        mouseEvent.onLeftRelease(p.x, p.y);
        break;

    case WM_RBUTTONDOWN:
        p = MAKEPOINTS(lp);
        mouseEvent.onRightPress(p.x, p.y,wp);
        break;

    case WM_RBUTTONUP:
        p = MAKEPOINTS(lp);
        mouseEvent.onRightRelease(p.x, p.y);
        break;
    case WM_MOUSEWHEEL:
        std::cout<<"wheel move\n";
        p = MAKEPOINTS(lp);
        delta = GET_WHEEL_DELTA_WPARAM(wp);
        mouseEvent.onWheelMove(p.x, p.y, delta);
        break;

  

    case WM_MBUTTONDOWN:
        p = MAKEPOINTS(lp);
        mouseEvent.onWheelPress(p.x, p.y,wp);
        break;

    case WM_MBUTTONUP:
        p = MAKEPOINTS(lp);
        mouseEvent.onWheelRelease(p.x, p.y);
        break;
        //--------------------------------------------------------------
    
    default:
        return DefWindowProc(handel, msg, wp, lp);

    }

    return DefWindowProc(handel, msg, wp, lp);

}

void Window::setClassName(const wchar_t* data) {
    wsprintf(windowClassName, data);
}
