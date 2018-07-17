#include <windows.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atlstr.h>

HINSTANCE g_hInstance;

void PaintWindowSolidColor(HWND hWnd, COLORREF color)
{
    HBRUSH hBrush = ::CreateSolidBrush(color);
    HDC hdc = GetDC(hWnd);

    RECT rect = {};
    GetClientRect(hWnd, &rect);

    FillRect(hdc, &rect, hBrush);

    ReleaseDC(hWnd, hdc);
    ::DeleteObject(hBrush);
}


class ChildWindow : public CWindowImpl<ChildWindow>
{
private:

public:
    BEGIN_MSG_MAP(ChildWindow)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
    END_MSG_MAP()

    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
    {
        PaintWindowSolidColor(m_hWnd, RGB(100, 100, 0));
        ValidateRect(nullptr);
        return 0;
    }
};


class MainWindow : public CWindowImpl<MainWindow>
{
private:

    CWindow _childWindow;

public:

    BEGIN_MSG_MAP(MainWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_PAINT, OnPaint)
        MESSAGE_HANDLER(WM_SIZE, OnResize)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnResize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
};

LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    RECT rect;
    GetClientRect(&rect);
    rect.left += 10;
    rect.right -= 10;
    rect.top += 10;
    rect.bottom -= 10;
    _childWindow = CreateWindow(L"BUTTON", L"Resize me", WS_CHILD|WS_VISIBLE|BS_CENTER|BS_TEXT, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, m_hWnd, 0, g_hInstance, 0);
    
    return 0;
}

LRESULT MainWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    PaintWindowSolidColor(m_hWnd, RGB(255,255,0));
    ValidateRect(nullptr);
    return 0;

}

LRESULT MainWindow::OnResize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    RECT rect;
    GetClientRect(&rect);
    _childWindow.ResizeClient(rect.right - rect.left - 20, rect.bottom - rect.top - 20);
    return 0;
}

LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    PostQuitMessage(0);
    return 0;
}



int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    g_hInstance = hInstance;

    MainWindow mainWindow;
    RECT rect = {0, 0, 800, 500};
    mainWindow.Create(nullptr, rect, L"Adrian's homework", WS_OVERLAPPEDWINDOW|WS_VISIBLE| WS_CLIPCHILDREN);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}