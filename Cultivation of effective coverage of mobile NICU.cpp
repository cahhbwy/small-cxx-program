#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#define R 6371
#define PI 3.14159
using namespace std;
#define random(x) (rand() % x)
typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    long biWidth;
    long biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    long biXPelsPerMeter;
    long biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

int ReadBmp(const char *szFileName);
int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b);

BITMAPINFOHEADER bih;
BYTE *Buffer = NULL;
long LineByteWidth;
class Point
{
private:
    double E;
    double N;

public:
    Point();
    Point(double _E, double _N)
    {
        E = _E;
        N = _N;
    }
    static double dis(const Point &a, const Point &b)
    {
        return R * acos(sin(a.N * PI / 180) * sin(b.N * PI / 180) + cos(a.N * PI / 180) * cos(b.N * PI / 180) * cos((a.E - b.E) * PI / 180));
    }
};

class Circle
{
public:
    Point O;
    double r;

    Circle();
    Circle(double _E, double _N, double _r) : O(_E, _N)
    {
        r = _r;
    }
    bool isInCircle(const Point &p) const
    {
        if (Point::dis(p, O) < r)
            return true;
        return false;
    }
};

bool isInCircles(const Point p, const vector<Circle> &circles)
{
    for (int i = 0; i < circles.size(); i++)
    {
        if (circles[i].isInCircle(p))
            return true;
    }

    return false;
}

vector<Circle> readCircles(const char *inFileName)
{
    vector<Circle> circles;
    ifstream fin(inFileName);
    string name;
    double E, N, r;
    while (fin >> name >> E >> N >> r)
        circles.push_back(Circle(E, N, r));
    return circles;
}

bool in_guangdong(int x, int y)
{
    BYTE r, g, b;
    if (GetDIBColor(x, y, &r, &g, &b) == 1)
    {
        if ((int)r == 255 && (int)g == 255 && (int)b == 255)
            return true;
    }
    return false;
}
int return_true_y(double y)
{
    int result = -154.87 * y + 4120.48;
    return result;
}
int return_true_x(double x)
{
    int result = 115.87 * x - 12247.59;
    return result;
}

int main(void)
{
    int x, y;
    BYTE r, g, b;
    int n;
    char szfilename[255] = "./1.bmp";
    int times = 0;

    if (ReadBmp(szfilename) == 0)
    {
        printf("failure to read file %s", szfilename);
        return 1;
    }

    printf("Width: %ld\n", bih.biWidth);
    printf("Height: %ld\n", bih.biHeight);
    printf("BitCount: %d\n\n", (int)bih.biBitCount);
    int high_x, high_y;
    bool find_high = false;
    for (int i = 130; i <= 200; i++) //纬度
    {
        for (int j = 500; j <= 1000; j++) //经度
        {
            if (GetDIBColor(j, i, &r, &g, &b) == 1)
            {
                if ((int)r == 255 && (int)g == 255 && (int)b == 255)
                {
                    high_x = j;
                    high_y = i;
                    find_high = true;
                    break;
                }
            }
        }
        if (find_high == true)
            break;
    }
    cout << high_x << " " << high_y << endl;
    int low_x, low_y;
    bool find_low = false;
    for (int i = 1000; i >= 950; i--) //纬度
    {
        for (int j = 450; j <= 500; j++) //经度
        {
            if (GetDIBColor(j, i, &r, &g, &b) == 1)
            {
                if ((int)r == 255 && (int)g == 255 && (int)b == 255)
                {
                    low_x = j;
                    low_y = i;
                    find_low = true;
                    break;
                }
            }
        }
        if (find_low == true)
            break;
    }
    cout << low_x << " " << low_y << endl;
    char szfilename1[255] = "./data.txt";
    vector<Circle> circles = readCircles(szfilename1);
    //for(int i = 0; i < circles.size(); i++)
    //cout<<circles[i].r<<" ";
    //cout<<endl;
    srand((int)time(0));
    int in_guangdong_count = 0;
    while (times <= 100000)
    {
        int x = random(821);
        double temp_x = (x % 821 + 10900.0) / 100.0;
        int y = random(560);
        double temp_y = (y % 561 + 2000.0) / 100.0;
        int true_x = return_true_x(temp_x);
        int true_y = return_true_y(temp_y);
        if (GetDIBColor(true_x, true_y, &r, &g, &b) == 1)
        {
            if (in_guangdong(true_x, true_y) == 1)
            {
                bool is_in_circle = isInCircles(Point(temp_x, temp_y), circles);
                if (is_in_circle == true)
                    in_guangdong_count++;
                times++;
            }
        }
    }
    cout << fixed << setprecision(3) << in_guangdong_count / 100001.0 << endl;
    free(Buffer);

    return 0;
}

int ReadBmp(const char *szFileName)
{
    FILE *file;
    WORD bfh[7];
    long dpixeladd;

    if (NULL == (file = fopen(szFileName, "rb")))
    {
        return 0;
    }
    printf("%s\n", szFileName);

    fread(&bfh, sizeof(WORD), 7, file);
    if (bfh[0] != (WORD)(((WORD)'B') | ('M' << 8)))
    {
        fclose(file);
        return 0;
    }

    fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);

    if (bih.biBitCount < 24)
    {
        fclose(file);
        return 0;
    }

    dpixeladd = bih.biBitCount / 8;
    LineByteWidth = bih.biWidth * (dpixeladd);
    if ((LineByteWidth % 4) != 0)
        LineByteWidth += 4 - (LineByteWidth % 4);

    if ((Buffer = (BYTE *)malloc(sizeof(BYTE) * LineByteWidth * bih.biHeight)) != NULL)
    {
        fread(Buffer, LineByteWidth * bih.biHeight, 1, file);

        fclose(file);
        return 1;
    }

    fclose(file);
    return 0;
}

int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b)
{
    int dpixeladd;
    BYTE *ptr;
    if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)
    {
        return 0;
    }

    dpixeladd = bih.biBitCount / 8;
    ptr = Buffer + X * dpixeladd + (bih.biHeight - 1 - Y) * LineByteWidth;

    *b = *ptr;
    *g = *(ptr + 1);
    *r = *(ptr + 2);

    return 1;
}
