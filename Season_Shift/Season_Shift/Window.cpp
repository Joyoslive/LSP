#include "pch.h"
#include "Window.h"
#include "Input.h"
#include "Logger.h"

bool firstActivate = false;

Window::Window(HINSTANCE hInst, const std::wstring title, UINT clientWidth, UINT clientHeight) : 
    m_hInst(hInst),
    m_title(title),
    m_clientWidth(clientWidth),
    m_clientHeight(clientHeight),
    m_style(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
    m_isClosed(true),
    m_exStyle(0),
    m_firstActivate(false)
{
    // Register window class
    WNDCLASS wc = { };
    wc.lpfnWndProc = Window::WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = title.c_str();
    RegisterClass(&wc);

    // create
    m_hwnd = CreateWindowEx(
        m_exStyle,                              // ex styles (optional add)
        title.c_str(),                  // class name
        title.c_str(),                  // title 
        m_style,                         // win style

        // pos and size
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,    // re-size to desired cw & ch later

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        (void*)this // Additional application data : re-route winproc
    );

    assert(m_hwnd);
    m_isClosed = false;

    //mouse_.getXTMouse()->SetWindow(m_hwnd);
    
    resizeToWindowToFitClient();
    ShowWindow(m_hwnd, SW_SHOWDEFAULT);

    // Init ImGUI
    IMGUI_CHECKVERSION();
    auto ctx = ImGui::CreateContext();
    ImGui_ImplWin32_Init(m_hwnd);
    ImGui::StyleColorsDark();
};

Window::~Window()
{

};

void Window::resizeToWindowToFitClient()
{

    RECT rect = { 0 };
    rect.right = m_clientWidth;
    rect.bottom = m_clientHeight;

    // We pass a rect that has our desired client area and receive corrected window rect (incl. border)
    if (!AdjustWindowRectEx(&rect, m_style, 0, m_exStyle))
        assert(false);

    long windowWidth = rect.right - rect.left;
    long windowHeight = rect.bottom - rect.top;

    // cx, cy are width and height!
    if (!SetWindowPos(m_hwnd, HWND_TOP, 0, 0, windowWidth, windowHeight, SWP_NOREPOSITION))
        assert(false);

}

HWND& Window::getHWND()
{
    return m_hwnd;
}

float Window::getAspectRatio() const
{
    return (float)m_clientWidth / (float)m_clientHeight;
}

UINT Window::getClientWidth() const
{
    return m_clientWidth;
}

UINT Window::getClientHeight() const
{
    return m_clientHeight;
}

bool Window::isClosed() const
{
    return m_isClosed;
}

void Window::Quit()
{
    PostMessage(m_hwnd, WM_CLOSE, 0, 0);
}

void Window::setOnResizeCallback(std::function<void(UINT width, UINT height)> func)
{
    m_onResize = func;
}

void Window::setFullScreenCallback(std::function<void(bool fullScreen)> set, std::function<bool()> get)
{
    m_setFullScreen = set;
    m_getFullScreenState = get;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::handleProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGui_ImplWin32_WndProcHandler(m_hwnd, uMsg, wParam, lParam);
    static bool s_fullScreen = false;
    switch (uMsg)
    {
	case WM_CLOSE:
    {
        m_isClosed = true;
        Input::getInput().lockMouse(2);
        DestroyWindow(m_hwnd);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        m_isClosed = true;
        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(m_hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(m_hwnd, &ps);
        break;
    }

    case WM_ACTIVATEAPP:
    {
        DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
        DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);

        if (m_getFullScreenState == NULL)
        {
            break;
        }
        if (!wParam)
        {
            s_fullScreen = m_getFullScreenState();
        }
        else
        {
            m_setFullScreen(s_fullScreen);
        }
        break;
    }

    case WM_WINDOWPOSCHANGED:
    {
        if (m_onResize == nullptr) break;
        if (m_isClosed) break;

        RECT winRect;
        GetClientRect(m_hwnd, &winRect);

        UINT rx = winRect.right - winRect.left;
        UINT ry = winRect.bottom - winRect.top;

        m_onResize(rx, ry);

        break;
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
        break;
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
        break;
    }
    case WM_ACTIVATE:
    {
        if (!m_firstActivate)
        {
            m_firstActivate = true;
            break;
        }
        if (wParam == WA_INACTIVE)
        {
            // Changes to absolute WITHOUT changing internal 'code'.
            // This handles the case where the player alt+tabs without opening the menu
            // Internal state --> relative but we purposefully set the mode to absolute
            // So when we get back into the game, it returns back to relative.
            Input::getInput().setModeAbsolute();
            break;
        }
        if (wParam == WA_ACTIVE)
        {
            // If alt+tabbed with relative --> get back in relative
            if (Input::getInput().getLatestCode() == 2)
                Input::getInput().lockMouse(2);
            else // If alt+tabbed with absolute --> get back in absolute
                Input::getInput().lockMouse(1);

            break;
        }
        break;
    }
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
    {
        DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
        break;
    }

    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // WindowProc needs to be a static function for class initialization! (We pass function ptr)
    // But remember that we want our action handling to happen in functions of classes (non static members)

    // Since a window has a few bytes that we can use to store data in (LongPtr) then we can use this to
    // store the ptr to the active window object of ours on class creation! (non-static)
    static Window* currWin;
    if (uMsg == WM_CREATE)
    {
        // First time creation --> Store active window in extra window memory
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* currWin = reinterpret_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)currWin);

        // Re-route directly to main win class
        return currWin->handleProc(uMsg, wParam, lParam);
    }
    else
    {
        currWin = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (currWin == nullptr)
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    else
        return currWin->handleProc(uMsg, wParam, lParam);
}   