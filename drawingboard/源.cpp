#include <windows.h>
#include<windowsx.h>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<map>
#include<utility>
#define MAX 1000
#pragma warning(disable:4996)

using namespace std;
HDC hdc;            //�豸�����ľ��
PAINTSTRUCT ps;
static int cnt = 0;
class Shape {
protected:
    int x1;
    int y1;
    int x2;
    int y2;
public:
    
    virtual void getdata(int a, int b, int c, int d) = 0;
    virtual void draw() = 0;
    virtual void move(int m, int n, int s, int t) = 0;
    virtual void save(FILE* p) = 0;
};

class Line :public Shape
{
public:
    float k;
    float b;
    float distance;
    void getdata(int a, int b, int c, int d);
    void draw();
    void move(int m, int n, int s, int t);
    void save(FILE* p);
};
void Line::getdata(int a, int b, int c, int d)
{
    x1 = a;
    y1 = b;
    x2 = c;
    y2 = d;
    /*k = (float)(y2 - y1) / (float)(x2 - x1);
    b = y1 - k * x1;*/
}
void Line::draw()
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}
void Line::move(int m, int n, int s, int t)
{
    if (x2 != x1)//б�ʴ���
    {
        k = (float)(y2 - y1) / (float)(x2 - x1);
        b = y1 - k * x1;
        distance = ((k * m + b - n) / sqrt(k * k + 1));
        if (((m <= x2 && m >= x1) || (m >= x2 && m <= x1)) && (fabs(distance) <= 2)) {
            x1 = x1 + s - m;
            y1 = y1 + t - n;
            x2 = x2 + s - m;
            y2 = y2 + t - n;
        }
    }
    else//б�ʲ�����
    {
        if (((n <= y2 && n >= y1) || (n >= y2 && n <= y1)) && (m <= x1 + 2 && m >= x1 - 2)) {

            x1 = x1 + s - m;
            y1 = y1 + t - n;
            x2 = x2 + s - m;
            y2 = y2 + t - n;
        }
    }
}
void Line::save(FILE* p)
{
    fprintf(p, "%d ", 1);
    fprintf(p, "%d ", x1);
    fprintf(p, "%d ", y1);
    fprintf(p, "%d ", x2);
    fprintf(p, "%d\n", y2);
}

class Rect :public Shape
{
public:
    void getdata(int a, int b, int c, int d);
    void draw();
    void move(int m, int n, int s, int t);
    void save(FILE* p);
};
void Rect::getdata(int a, int b, int c, int d)
{
    x1 = a;
    y1 = b;
    x2 = c;
    y2 = d;
}
void Rect::draw()
{
    Rectangle(hdc, x1, y1, x2, y2);
}
void Rect::move(int m, int n, int s, int t)
{
    if (((m <= x2 && m >= x1) || (m >= x2 && m <= x1)) &&
        ((n <= y2 && n >= y1) || (n >= y2 && n <= y1)))
    {
        x1 = x1 + s - m;
        y1 = y1 + t - n;
        x2 = x2 + s - m;
        y2 = y2 + t - n;
    }
}
void Rect::save(FILE* p)
{
    fprintf(p, "%d ", 2);
    fprintf(p, "%d ", x1);
    fprintf(p, "%d ", y1);
    fprintf(p, "%d ", x2);
    fprintf(p, "%d\n", y2);
}

class Circle :public Shape
{
public:
    float dcircle;
    void getdata(int a, int b, int c, int d);
    void draw();
    void move(int m, int n, int s, int t);
    void save(FILE* p);
};
void Circle::getdata(int a, int b, int c, int d)
{
    x1 = a;
    y1 = b;
    x2 = c;
    y2 = d;
    dcircle = (int)sqrt(((float)x1 - (float)x2) * ((float)x1 - (float)x2) + ((float)y1 - (float)y2) * ((float)y1 - (float)y2));
}
void Circle::draw()
{
    Ellipse(hdc,
        (x1 + x2 - (int)dcircle) / 2,
        (y1 + y2 + (int)dcircle) / 2,
        (x1 + x2 + (int)dcircle) / 2,
        (y1 + y2 - (int)dcircle) / 2);
}
void Circle::move(int m, int n, int s, int t)
{
    if (sqrt(((float)m - (float)x1 / 2 - (float)x2 / 2) * ((float)m - (float)x1 / 2 - (float)x2 / 2) + ((float)n - (float)y1 / 2 - (float)y2 / 2) * ((float)n - (float)y1 / 2 - (float)y2 / 2)) <= dcircle / 2) {
        x1 = x1 + s - m;
        y1 = y1 + t - n;
        x2 = x2 + s - m;
        y2 = y2 + t - n;
    }
}
void Circle::save(FILE* p)
{
    fprintf(p, "%d ", 3);
    fprintf(p, "%d ", x1);
    fprintf(p, "%d ", y1);
    fprintf(p, "%d ", x2);
    fprintf(p, "%d\n", y2);
}

class Shapefactory
{
public:
    virtual Shape* createshape() = 0;
};

class Linefactory:public Shapefactory
{
public:
    Shape* createshape();
};
Shape* Linefactory::createshape()
{
    return new Line;
}

class Rectfactory:public Shapefactory
{
public:
    Shape* createshape();
};
Shape* Rectfactory::createshape()
{
    return new Rect;
}

class Circlefactory :public Shapefactory
{
public:
    Shape* createshape();
};
Shape* Circlefactory::createshape()
{
    return new Circle;
}
//ָ��ͼ�ι�����ָ��
Shapefactory* pLf = new Linefactory;
Shapefactory* pRf = new Rectfactory;
Shapefactory* pCf = new Circlefactory;
//����ӳ���ϵ
map<int, Shapefactory*>Map;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    
    static TCHAR szAppName[] = TEXT("����");    //�����������
    HWND hwnd;                                      //���
    MSG msg;                                        //��Ϣ�ṹ
    WNDCLASS wndclass;                              //������ṹ
    //������ṹ��������
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    //ע��
    if (!RegisterClass(&wndclass))
    {
        MessageBox(NULL, TEXT("���������Ҫ�� Windows NT ����ִ�У�"), szAppName, MB_ICONERROR);
        return 0;
    }
    //����createwindow���ô��ھ�������
    hwnd = CreateWindow(szAppName,//��������
        TEXT("�ҵĻ���"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);
    //��Ϣѭ�������ģ�
    while (GetMessage(&msg, NULL, 0, 0))//GetMessage����ָ�뽫��Ϣ������Ϣ�ṹ
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //HDC hdc;            //�豸�����ľ��
    //PAINTSTRUCT ps;
    static int xs, ys;
    static int xe, ye;
    static int xmove, ymove;
    static int mode;
    RECT* rect = NULL;
    FILE* fp;
    HBRUSH hBrush, hOldBrush;
    HPEN hPen, hOldPen;
    /*static int cnt = 0;*/
    static Shape* pshape[MAX];
    static int shp;
    static int xt1;
    static int yt1;
    static int xt2;
    static int yt2;
    
    switch (message)
    {
        //���ڵĳ�ʼ����Ϣ
    case WM_CREATE: {
        Map.insert(pair<int, Shapefactory*>(1, pLf));
        Map.insert(pair<int, Shapefactory*>(2, pRf));
        Map.insert(pair<int, Shapefactory*>(3, pCf));
        CreateWindow(
            TEXT("Button"),//��������
            TEXT("ֱ��"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            0, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)1,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);
        //Line linebutton(0,0,0,0);
        //linebutton.crtbt(hwnd,message,wParam,lParam);

        CreateWindow(
            TEXT("Button"),//��������
            TEXT("����"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            100, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)2,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);

        CreateWindow(
            TEXT("Button"),//��������
            TEXT("Բ"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            200, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)3,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);

        CreateWindow(
            TEXT("Button"),//��������
            TEXT("�϶�"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            300, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)19,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);

        CreateWindow(
            TEXT("Button"),//��������
            TEXT("����"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            500, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)20,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);

        CreateWindow(
            TEXT("Button"),//��������
            TEXT("��ȡ"), //д����
            WS_VISIBLE | WS_CHILD, //��ʼ�ɼ���WS_CHILD������Ϊ���˵��Ӵ��ڣ�һ��Ҫ����
            600, 0, 100, 50, //x,y,x����ߴ磬y����ߴ�
            hwnd, //���Ǹ����ھ��
            (HMENU)21,
            ((LPCREATESTRUCT)lParam)->hInstance, //Ӧ�ó���ʵ�����
            NULL);
    }
    case WM_COMMAND:
    {
        mode = LOWORD(wParam);
        switch (mode) {
        case 20:
            fp = fopen("record.text", "w");
            fprintf(fp, "%d\n", cnt);
            for (int i = 0; i < cnt; i++)
            {
                pshape[i]->save(fp);
            }
            fclose(fp);
            return 0;

        case 21:
            mode = 0;
            fp = fopen("record.text", "r");
            if (fp == NULL)
                MessageBox(hwnd, TEXT("���ļ�ʧ��"), TEXT("����"), MB_OK);
            else {
                fscanf(fp, "%d", &cnt);

                for (int i = 0; i < cnt; i++)
                {
                    fscanf(fp, "%d", &shp);
                    fscanf(fp, "%d %d %d %d", &xt1, &yt1, &xt2, &yt2);
                    pshape[i] = Map[shp]->createshape();
                    pshape[i]->getdata(xt1, yt1, xt2, yt2);
                }
                
                fclose(fp);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        default:
            return 0;
        }
    }
    case WM_LBUTTONDOWN:
        xs = GET_X_LPARAM(lParam);
        ys = GET_Y_LPARAM(lParam);
        return 0;

    case WM_LBUTTONUP: {

        xe = GET_X_LPARAM(lParam);
        ye = GET_Y_LPARAM(lParam);
        if (mode != 0)
        {
            if (mode != 19)
            {
                pshape[cnt] = Map[mode]->createshape();
                pshape[cnt]->getdata(xs, ys, xe, ye);
                cnt++;
            }
            else
            {
                for (int i = 0; i < cnt; i++)
                {
                            pshape[i]->move(xs, ys, xe, ye);
                }
            }
        }
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    }
    case WM_PAINT: {
        hdc = BeginPaint(hwnd, &ps);
        hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        hPen = CreatePen(PS_INSIDEFRAME, 5, 0);
        hOldPen = (HPEN)SelectObject(hdc, hPen);

        for (int i = 0; i < cnt; i++)
        {
            pshape[i]->draw();
        }

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        for (int i = 0; i < cnt; i++)
        {
            delete pshape[i];
            pshape[i] = NULL;
        }
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}