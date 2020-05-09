#include <windows.h>
#include <windowsx.h>

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    SetDlgItemInt(hwnd, edt1, 0, TRUE);
    SetDlgItemInt(hwnd, edt2, 0, TRUE);
    SetDlgItemInt(hwnd, edt3, 100, TRUE);
    SetDlgItemInt(hwnd, edt4, 50, TRUE);
    SetDlgItemText(hwnd, edt5, TEXT("Text"));

    ComboBox_AddString(GetDlgItem(hwnd, cmb1), TEXT("DT_TOP"));
    ComboBox_AddString(GetDlgItem(hwnd, cmb1), TEXT("DT_VCENTER"));
    ComboBox_AddString(GetDlgItem(hwnd, cmb1), TEXT("DT_BOTTOM"));
    ComboBox_SetCurSel(GetDlgItem(hwnd, cmb1), 0);

    CheckDlgButton(hwnd, chx1, BST_CHECKED);

    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    case edt1:
    case edt2:
    case edt3:
    case edt4:
    case edt5:
    case chx1:
    case cmb1:
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }
}

void OnDraw(HWND hwnd, HDC hdc)
{
    LOGFONT lf;
    ZeroMemory(&lf, sizeof(lf));
    lf.lfHeight = -20;
    lstrcpy(lf.lfFaceName, TEXT("Tahoma"));
    HFONT hFont = CreateFontIndirect(&lf);

    LONG left = GetDlgItemInt(hwnd, edt1, NULL, TRUE);
    LONG top = GetDlgItemInt(hwnd, edt2, NULL, TRUE);
    LONG right = GetDlgItemInt(hwnd, edt3, NULL, TRUE);
    LONG bottom = GetDlgItemInt(hwnd, edt4, NULL, TRUE);

    RECT rc;
    SetRect(&rc, left, top, right, bottom);

    TCHAR szText[256];
    GetDlgItemText(hwnd, edt5, szText, 256);

    BOOL fCalcRect = (IsDlgButtonChecked(hwnd, chx1) == BST_CHECKED);

    UINT uFormat = DT_SINGLELINE | DT_LEFT;
    if (fCalcRect)
        uFormat |= DT_CALCRECT;

    INT iItem = ComboBox_GetCurSel(GetDlgItem(hwnd, cmb1));
    switch (iItem)
    {
    case 0:
        uFormat |= DT_TOP;
        break;
    case 1:
        uFormat |= DT_VCENTER;
        break;
    case 2:
        uFormat |= DT_BOTTOM;
        break;
    }

    SetWindowOrgEx(hdc, -50, -50, NULL);

    HGDIOBJ hbrOld = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, left, top, right, bottom);
    SelectObject(hdc, hbrOld);

    SIZE siz;
    HGDIOBJ hFontOld = SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, szText, -1, &rc, uFormat);
    GetTextExtentPoint32(hdc, szText, lstrlen(szText), &siz);
    SelectObject(hdc, hFontOld);

    HPEN hPen = CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
    HGDIOBJ hPenOld = SelectObject(hdc, hPen);
    hbrOld = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    SelectObject(hdc, hbrOld);
    SelectObject(hdc, hPenOld);
    DeleteObject(hPen);

    SetWindowOrgEx(hdc, 0, 0, NULL);

    wsprintf(szText, TEXT("rc:(%ld, %ld)-(%ld, %ld) ==> rc:(%ld, %ld)-(%ld, %ld), siz:(%ld, %ld)"),
        left, top, right, bottom,
        rc.left, rc.top, rc.right, rc.bottom, siz.cx, siz.cy);
    TextOut(hdc, 0, 0, szText, lstrlen(szText));

    DeleteObject(hFont);
}

void OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    if (HDC hdc = BeginPaint(hwnd, &ps))
    {
        OnDraw(hwnd, hdc);
        EndPaint(hwnd, &ps);
    }
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);
    return 0;
}
