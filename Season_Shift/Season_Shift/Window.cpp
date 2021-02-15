#include "Window.h"
#include "Input.h"
#include "Logger.h"

Window::Window(HINSTANCE hInst, const std::wstring title, UINT clientWidth, UINT clientHeight) : 
    m_hInst(hInst),
    m_title(title),
    m_clientWidth(clientWidth),
    m_clientHeight(clientHeight),
    m_style(WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX),
    m_isClosed(true),
    m_exStyle(0)
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

void Window::setOnResizeCallback(std::function<void(UINT width, UINT height)> func)
{
    m_onResize = func;
}
LRESULT Window::handleProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        m_isClosed = true;
        break;
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
       DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
       DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
       break;

    case WM_WINDOWPOSCHANGED:
    {
        if (m_onResize == nullptr) break;

        RECT winRect;
        GetClientRect(m_hwnd, &winRect);

        UINT rx = winRect.right - winRect.left;
        UINT ry = winRect.bottom - winRect.top;

        Logger::getLogger().setFile("WindowLogger.txt");
        Logger::getLogger().addLog(std::to_string(rx) + "x" + std::to_string(ry) + "\n");
        Logger::getLogger().debugLog(std::to_string(rx) + "x" + std::to_string(ry) + "\n");

        m_onResize(rx, ry);

        Logger::getLogger().dumpLogs();

        break;
    }

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
        break;
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
        DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
       break;

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