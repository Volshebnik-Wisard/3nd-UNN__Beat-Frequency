// MFCApplication7Dlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#define _USE_MATH_DEFINES
#include "MFCApplication7.h"
#include "MFCApplication7Dlg.h"
#include "afxdialogex.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;

IMPLEMENT_DYNAMIC(Graphic, CStatic)

// Диалоговое окно CMFCApplication7Dlg



CMFCApplication7Dlg::CMFCApplication7Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION7_DIALOG, pParent)
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
	DDX_Control(pDX, IDC_CHECK1, Animation);
}

BEGIN_MESSAGE_MAP(CMFCApplication7Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication7Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK1, &CMFCApplication7Dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplication7Dlg

BOOL CMFCApplication7Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	AnimationThread.m_pfnThreadProc = AnimateThreadFunc;
	AnimationThread.m_pThreadParams = this;
	AnimationThread.CreateThread(CREATE_SUSPENDED);

	Gdiplus::RectF worldRect = { 0, -2, 2, 4 };
	Picture.worldRect = worldRect;



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
	Status s = GdiplusStartup(&work, &input, NULL);
}


void Graphic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//Определение размеров окна
	auto& window = lpDrawItemStruct->rcItem;
	Rect out_rect{ window.left, window.top, window.right - window.left, window.bottom - window.top };


	//Отрисовываем область для отрисовки
	Graphics wnd(lpDrawItemStruct->hDC);
	Bitmap bitmap(out_rect.Width, out_rect.Height, &wnd);
	Graphics graphics(&bitmap);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.Clear(Color(255, 255, 255));

	REAL otstupX = 23, otstupY = 23;


	//Окно отрисовки графика
	Rect rect_graph = out_rect;
	rect_graph.X += otstupX;
	rect_graph.Y += otstupY;
	rect_graph.Width -= otstupX * 2;
	rect_graph.Height -= otstupY * 2;

	double y_max = max(worldRect.Y, worldRect.Y + worldRect.Height);

	Matrix matrix;

	matrix.Translate(otstupX, otstupY);
	matrix.Scale((REAL)rect_graph.Width / worldRect.Width, -(REAL)rect_graph.Height / worldRect.Height);
	matrix.Translate(0, worldRect.Y);


	// Отрисовка сетки 
	Pen gridPen(Color(220, 220, 220), 0.005);

	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	Gdiplus::Font font(L"Arial", 8, FontStyleRegular);
	//Кол-во линий сетки
	int scaleX = 10;
	int scaleY = 5 * round(pow(10, log10(y_max) - floor(log10(y_max)))) * ((ceil(pow(10, log10(y_max) - floor(log10(y_max)))) == 1) ? 5 : 1);

	for (int var = 0; var <= scaleX; ++var)	//По Х
	{
		// Проекции - * / координату X
		PointF p1 = { worldRect.X + (REAL)var / scaleX * worldRect.Width, worldRect.Y };
		PointF p2 = { worldRect.X + (REAL)var / scaleX * worldRect.Width, worldRect.Y + worldRect.Height };
		CString podpis; podpis.Format(L"%.2f", p1.X * 0.63);

		matrix.TransformPoints(&p1);
		matrix.TransformPoints(&p2);

		graphics.DrawLine(&gridPen, p1, p2);

		RectF rect = { PointF{ p1.X - otstupX, p1.Y - 5 }, SizeF{ 2 * otstupX, otstupY } };
		graphics.DrawString(podpis.GetString(), podpis.GetLength(), &font, rect, &format, &SolidBrush(Color(0, 0, 0)));
	}

	for (int var = 0; var <= scaleY; ++var)	//По Y
	{
		PointF p1 = { worldRect.X, worldRect.Y + (REAL)var / scaleY * worldRect.Height };
		PointF p2 = { worldRect.X + worldRect.Width, worldRect.Y + (REAL)var / scaleY * worldRect.Height };
		CString podpis;
		podpis.Format(L"%.1f", p1.Y);

		matrix.TransformPoints(&p1);
		matrix.TransformPoints(&p2);

		graphics.DrawLine(&gridPen, p1, p2);


		RectF rect = { PointF{ 0, p1.Y - 30 }, SizeF{ otstupX, 30 * 2 } };
		graphics.DrawString(podpis.GetString(), podpis.GetLength(), &font, rect, &format, &SolidBrush(Color(0, 0, 0)));
	}


	// Отрисовка значений графика
	Pen graphPen(Color(0, 0, 255), 0.005);
	graphics.SetTransform(&matrix);
	if (!points.empty())
		for (size_t var = 0; var < points.size(); ++var)
		{
			if (counts[var] == 0) continue;
			Pen* graphPen1;
			graphPen1 = &graphPen;
			graphics.DrawCurve(graphPen1, points[var], counts[var]);
		}


	// Отрисовка осей 
	Pen ortPen(Color(0, 0, 0), 0.01);
	PointF xl{ worldRect.X, worldRect.Y }, xr{ worldRect.Width, worldRect.Y };
	PointF yb{ 0, worldRect.Y }, yt{ 0, worldRect.Y + worldRect.Height };
	graphics.DrawLine(&ortPen, xl, xr);
	graphics.DrawLine(&ortPen, yb, yt);


	// Отрисовка подписей
	graphics.ResetTransform();
	graphics.TranslateTransform(otstupX, otstupY);
	graphics.ResetTransform();

	wnd.DrawImage(&bitmap, out_rect);
}


double Graphic::Nearfloatpoint(std::vector<double> values)//Ближайшее число с плавающей точкой
{
	double maxF = *(std::max_element(values.begin(), values.end()));
	double minF = *(std::min_element(values.begin(), values.end()));
	double valF = (maxF > -minF) ? maxF : -minF;
	return pow(10, floor(log10(valF))) * ceil(pow(10, log10(valF) - floor(log10(valF))));
}

std::pair<PointF*, size_t> Graphic::DefiningValues(std::vector<double> values, RectF worldRect)
{
	PointF* point = new PointF[values.size()];
	for (size_t i = 0; i < values.size(); i++)
	{
		point[i].X = worldRect.X + i * worldRect.Width / values.size();
		point[i].Y = values[i];
	}

	return std::pair<PointF*, size_t>{point, values.size()};
	delete[] point;
}


//заполняем вектора значениями
void Graphic::SetValuesPoints(std::vector<std::vector<double>> values)
{
	for (auto& i : points)
	{
		delete[] i;
	}
	points.clear(); points.resize(values.size());
	counts.clear(); counts.resize(values.size());

	for (auto it = values.begin(); it < values.end(); it++)
	{
		auto a = DefiningValues(*it, this->worldRect);
		points[it - values.begin()] = a.first;
		counts[it - values.begin()] = a.second;
	}
}


Graphic::~Graphic()
{
	GdiplusShutdown(work);
}


UINT CMFCApplication7Dlg::AnimateThreadFunc(LPVOID pv)
{
	auto p = (CMFCApplication7Dlg*)pv;

	while (true)
	{
		p->OnBnClickedButton1();
		Sleep(75);
	}
	return 0;
}


void CMFCApplication7Dlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	double w1, dw;
	static double phase = 0;
	if (!Animation.GetCheck()) phase = 0;

	CString buffer;
	GetDlgItemText(IDC_EDIT1, buffer); w1 = wcstof(buffer, NULL);
	GetDlgItemText(IDC_EDIT2, buffer); dw = wcstof(buffer, NULL);


	double cor = 1.885 / 3;
	double fd = 512;
	std::vector<double> signal(1024);
	for (size_t i = 0; i < 1024; i++)
	{
		signal[i] = cos(w1 * cor * i / fd + phase) + cos((w1 * cor + dw * cor) * i / fd);

	}

	Gdiplus::RectF worldRect = { 0, -2, 2, 4 };
	Picture.SetValuesPoints({ signal });

	DRAWITEMSTRUCT Draw = {};

	Draw.CtlID = IDC_PICTURE;
	Draw.hwndItem = Picture.GetSafeHwnd();
	Draw.hDC = Picture.GetDC()->GetSafeHdc();
	GetDlgItem(IDC_PICTURE)->GetClientRect(&Draw.rcItem);

	Picture.DrawItem(&Draw);
	ReleaseDC(CDC::FromHandle(Draw.hDC));

	if (Animation.GetCheck()) {
		phase += 0.1; phase = fmod(phase, 2 * 3.1415);
	}

}


void CMFCApplication7Dlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (Animation.GetCheck())
	{
		AnimationThread.ResumeThread();

		GetDlgItem(IDC_EDIT1)->EnableWindow(0);
		GetDlgItem(IDC_EDIT2)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(0);
	}
	else
	{
		AnimationThread.SuspendThread();

		GetDlgItem(IDC_EDIT1)->EnableWindow(1);
		GetDlgItem(IDC_EDIT2)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(1);
	}
}

void CMFCApplication7Dlg::OnOK()
{

}















//	sin(2 * M_PI * w1 / 10 * i / fd) + sin(2 * M_PI * (w1 / 10 - dw / 10) * i / fd + phase);