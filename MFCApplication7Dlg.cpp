
// MFCApplication7Dlg.cpp : файл реализации
//

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "MFCApplication7.h"
#include "MFCApplication7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace Gdiplus;
IMPLEMENT_DYNAMIC(Graphic, CStatic)

// диалоговое окно CMFCApplication7Dlg



CMFCApplication7Dlg::CMFCApplication7Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMFCApplication7Dlg::IDD, pParent)
	, Omega(100)
	, dOmega(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication7Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Omega);
	DDX_Text(pDX, IDC_EDIT2, dOmega);
	DDX_Control(pDX, IDC_PICTURE, Picture);
}

BEGIN_MESSAGE_MAP(CMFCApplication7Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_DRAW, &CMFCApplication7Dlg::OnBnClickedDraw)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_ANIMATION, &CMFCApplication7Dlg::OnBnClickedAnimation)
END_MESSAGE_MAP()


// обработчики сообщений CMFCApplication7Dlg

BOOL CMFCApplication7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	fi = 0.;
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCApplication7Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplication7Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_MESSAGE_MAP(Graphic, CStatic)
END_MESSAGE_MAP()

Graphic::Graphic()
{
	GdiplusStartupInput input;
	Status s;
	s = GdiplusStartup(&work, &input, NULL);
	animation_switch = true;
	max_y = 0.; //Для размеров окна
	min_y = 0.;
	max_x = 0.;
	min_x = 0.;
}

void Graphic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  Добавьте исходный код для отображения указанного элемента
	if (animation_switch)
	{
		animation_switch = false;
	}
	else
	{
		Graphics wnd(lpDrawItemStruct->hDC);
		Bitmap buffer(lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, &wnd);
		Graphics draw_in_buffer(&buffer);
		draw_in_buffer.SetSmoothingMode(SmoothingModeAntiAlias);
		draw_in_buffer.Clear(Color(255, 255, 255, 255));

		double otstup = 30.; //Отступ

		RectF rect_graph(otstup, otstup, //Окно отрисовки графика
			lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left - 1.5 * otstup,
			lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top - otstup
		);

		draw_in_buffer.TranslateTransform(otstup, (lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top) / 2);
		double Xscale = rect_graph.Width / (max_x - min_x); //Мастабы для X и Y
		double Yscale = rect_graph.Height / (max_y - min_y);
		draw_in_buffer.ScaleTransform(Xscale, -Yscale);

		Pen pen_ortho(Color(255, 0, 0, 0), 0.01); //Оси

		draw_in_buffer.DrawLine(&pen_ortho, PointF(min_x, min_y), PointF(min_x, max_y)); //Отрисавка оси Y
		draw_in_buffer.DrawLine(&pen_ortho, PointF(min_x, min_y), PointF(max_x, min_y)); //Отрисавка оси X

		Pen pen_network(Color(255, 220, 220, 220), 0.001); //Сетка

		int num = 10; //Кол-во штрихов сетки по X и Y
		double step_ver_network = (max_x - min_x) / num; //шаг по X
		double step_hor_network = (max_y - min_y) / num; //шаг по Y

		double x = min_x;
		for (int i = 0; i < num; i++) //Отрисывка сетки
		{
			draw_in_buffer.DrawLine(&pen_network, PointF(min_x + (i + 1) * step_ver_network, min_y), PointF(min_x + (i + 1) * step_ver_network, max_y));
			draw_in_buffer.DrawLine(&pen_network, PointF(min_x, min_y + (i + 1) * step_hor_network), PointF(max_x, min_y + (i + 1) * step_hor_network));
		}

		Pen pen_graph(Color(255, 0, 0, 255), 0.001); //График
		int iter = 1;
		for (x = min_x + step_x; x < max_x; x += step_x)
		{
			draw_in_buffer.DrawLine(&pen_graph, PointF((x - step_x), Funcion[iter - 1]), PointF(x, Funcion[iter]));
			iter++;
		}
		draw_in_buffer.ScaleTransform(1 / Xscale, -1 / Yscale);


		FontFamily my_font_family(L"Arial"); //Шрифт
		Gdiplus::Font my_font(&my_font_family, 10, FontStyleRegular, UnitPixel);
		SolidBrush brush_font(Color(255, 0, 0, 0));

		x = min_x; //Сброс
		double y = min_y;
		wchar_t podpis_ortho[10]; //Массив значений
		for (int i = 0; i < num + 1; i++) // Заполнение и отображение массива значений
		{
			swprintf_s(podpis_ortho, L"%.2f", x);
			draw_in_buffer.DrawString(podpis_ortho, -1, &my_font, PointF(x * Xscale - 10., rect_graph.Height / 2), &brush_font);
			x += step_ver_network;

			swprintf_s(podpis_ortho, L"%.1f", y);
			draw_in_buffer.DrawString(podpis_ortho, -1, &my_font, PointF(min_x - 25., -y * Yscale - 8.), &brush_font);
			y += step_hor_network;
		}

		wnd.DrawImage(&buffer, 0, 0, 0, 0, lpDrawItemStruct->rcItem.right, lpDrawItemStruct->rcItem.bottom, UnitPixel);
	}
}

Graphic::~Graphic()
{
	GdiplusShutdown(work);
}


std::vector<double> CMFCApplication7Dlg::Func_bieniya(double omega, double delta_omega, double& maxF, double& minF, double& max_x, double& min_x, double& step)
{
	step = 0.001; //Шаг дискретизации
	int iter = 0;
	min_x = 0.;
	max_x = 4 * M_PI / delta_omega;
	std::vector<double> func;
	for (double x = 0.; x < max_x; x += step)
	{
		func.push_back(cos(omega * x + fi) + cos((omega + delta_omega) * x)); //Сигнал
		if (maxF < func[iter]) maxF = func[iter]; //Определение максимума функции
		if (minF > func[iter]) minF = func[iter];
		iter++;
	}
	return func;
}

void CMFCApplication7Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	UpdateData(TRUE);
	fi += 0.1; //Скорость изменения фазы графика
	Picture.Funcion = Func_bieniya(Omega, dOmega, Picture.max_y, Picture.min_y, Picture.max_x, Picture.min_x, Picture.step_x);
	Picture.Invalidate(FALSE);
}


void CMFCApplication7Dlg::OnBnClickedAnimation()
{
	// TODO: добавьте свой код обработчика уведомлений
	animation = !animation;
	if (animation) timer = SetTimer(1, 100, NULL);
	else KillTimer(timer);
}


void CMFCApplication7Dlg::OnBnClickedDraw()
{
	// TODO: добавьте свой код обработчика уведомлений
	UpdateData(TRUE);
	Picture.Funcion = Func_bieniya(Omega, dOmega, Picture.max_y, Picture.min_y, Picture.max_x, Picture.min_x, Picture.step_x);
	Picture.Invalidate(FALSE);
}

