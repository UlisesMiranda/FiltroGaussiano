#define _USE_MATH_DEFINES
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace cv;
using namespace std;

double* rellenarMascara(double sigma, int n);

double * rellenarMascara(double sigma, int n) {

    int i = 0, limite = n / 2;
    double r, s,z;

    double* valores;
    valores = new double[n * n];

    s = 2.0 * sigma * sigma;

  
    for (int x = -limite; x <= limite; x++) {
        for (int y = -limite; y <= limite; y++) {
            r = sqrt(x * x + y * y);
            z = (exp(-(r * r) / s)) / (M_PI * s);
            valores[i] = (exp(-(r * r) / s)) / (M_PI * s);
            i++;
        }
    }

    return valores;
}

Mat matrizRelleno(int filas, int columnas, int n)
{
    int diferenciaBordes = n - 1;
    Mat matriz(filas + diferenciaBordes, columnas + diferenciaBordes, CV_8UC1);

    for (int i = 0; i < filas + diferenciaBordes; i++)
    {
        for (int j = 0; j < columnas + diferenciaBordes; j++)
        {
            matriz.at<uchar>(Point(j, i)) = uchar(0);
        }
    }

    return matriz;
}

Mat copiarImgARelleno(Mat bordes, Mat original, int n)
{
    int diferenciaBordes = ((n - 1) / 2);
    int filas = bordes.rows;
    int columnas = bordes.cols;

    for (int i = diferenciaBordes; i < filas - diferenciaBordes; i++)
    {
        for (int j = diferenciaBordes; j < columnas - diferenciaBordes; j++)
        {
            bordes.at<uchar>(Point(j, i)) = original.at<uchar>(Point(j - diferenciaBordes, i - diferenciaBordes));
        }
    }

    return bordes;
}

int main()
{
    int n = 0, filasImagen = 0, columnasImagen = 0;
    double sigma = 0.0;

    Mat image;
    char imageName[] = "lena.png";

    cout << "Digite el largo que tendra la mascara cuadrada para aplicar en la imagen: ";
    cin >> n;
    cout << "Digite el valor de 'sigma' para usar en el filtro Gaussiano: ";
    cin >> sigma;

    if ((n % 2) == 0)
    {
        cout << "La mascara debe ser de longitud impar";
        exit(1);
    }

    image = imread(imageName);
    // Error reading image validation
    if (!image.data)
    {
        cout << "Error al cargar la imagen: " << imageName << endl;
        exit(1);
    }

    cvtColor(image, image, COLOR_BGR2GRAY);

    double* mascara = new double[n * n];
    mascara = rellenarMascara(sigma, n);

    for (int i = 0; i < n * n; i++) {
        cout << mascara[i] << endl;
    }

    filasImagen = image.rows;
    columnasImagen = image.cols;

    Mat matrizConBordes = matrizRelleno(filasImagen, columnasImagen, n);
    matrizConBordes = copiarImgARelleno(matrizConBordes, image, n);

    namedWindow("NTSC Grayscale Image", WINDOW_AUTOSIZE);
    imshow("NTSC Grayscale Image", image);

    namedWindow("Filtro Obtenido", WINDOW_AUTOSIZE);
    imshow("Filtro Obtenido", matrizConBordes);

    waitKey(0);

    return 1;
}