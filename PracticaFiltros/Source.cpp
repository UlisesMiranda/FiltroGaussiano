#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat createMask(int n)
{
    Mat mask(n, n, CV_8UC1);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            mask.at<uchar>(Point(j, i)) = uchar(0);
        }
    }
    return mask;
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
    float sigma = 0;

    Mat image, mask;
    char imageName[] = "lena.png";

    cout << "Digite el largo que tendra la mascara cuadrada para aplicar en la imagen: ";
    cin >> n;
    cout << "Digite el valor de 'sigma' para usar en el filtro Gaussiano: ";
    cin >> sigma;
    /*cout << "Ingresa el /*nombre de la imagen a usar: ";
    cin >> imageName;*/

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

    mask = createMask(n);
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