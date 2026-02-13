
// MFCApplication7Dlg.h: файл заголовка
//

#pragma once
#include <vector>
#include <string>
#include <gdiplus.h>
#include <algorithm>

class Graphic : public CStatic
{
	DECLARE_DYNAMIC(Graphic)
protected:
	DECLARE_MESSAGE_MAP()

public:
	Graphic();
	ULONG_PTR work;
	std::vector<Gdiplus::PointF*> points; std::vector<size_t> counts;
	Gdiplus::RectF worldRect = { 0, -1, 2, 2 }; //иначе не перерисуется
	static double Nearfloatpoint(std::vector<double> values);
	static std::pair<Gdiplus::PointF*, size_t> DefiningValues(std::vector<double>, Gdiplus::RectF worldRect);
	void SetValuesPoints(std::vector<std::vector<double>> values);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~Graphic();
};



// Диалоговое окно CMFCApplication7Dlg
class CMFCApplication7Dlg : public CDialogEx
{
	// Создание
public:
	CMFCApplication7Dlg(CWnd* pParent = nullptr);	// стандартный конструктор

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION7_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


	// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Graphic Picture;
	double Omega, dOmega;
	CButton Animation;
	CWinThread AnimationThread;
	static UINT AnimateThreadFunc(LPVOID pv);
	virtual void OnOK();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck1();
};
