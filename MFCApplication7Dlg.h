
// MFCApplication7Dlg.h : файл заголовка
//

#pragma once
#include <gdiplus.h>
#include <vector>

class Graphic : public CStatic
{
	DECLARE_DYNAMIC(Graphic)

protected:
	DECLARE_MESSAGE_MAP()

public:
	Graphic();
	ULONG_PTR work;
	std::vector<double> Funcion;
	bool animation_switch;
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	double max_x, max_y, min_x, min_y, step_x;
	virtual ~Graphic();
};

// диалоговое окно CMFCApplication7Dlg
class CMFCApplication7Dlg : public CDialogEx
{
	// Создание
public:
	CMFCApplication7Dlg(CWnd* pParent = NULL);	// стандартный конструктор

	// Данные диалогового окна
	enum { IDD = IDD_MFCAPPLICATION7_DIALOG };

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
	std::vector<double> Func_bieniya(double omega, double delta_omega, double& max_f, double& min_f, double& max_x, double& min_x, double& step);
	double Omega, dOmega, fi;
	bool animation;
	UINT timer;
	afx_msg void OnBnClickedAnimation();
	afx_msg void OnBnClickedDraw();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
