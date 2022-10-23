#define _USE_MATH_DEFINES
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace cv;
using namespace std;


vector<vector<float>> rellenarMascara(int mascSize, float sigma) {

    int limite = (mascSize - 1)/ 2;
    float r, s,z, gaussResultado;
    float sum = 0.0;

    vector<vector<float>> mascara(mascSize, vector<float>(mascSize, 0));

    s = 2.0 * sigma * sigma;

    for (int x = -limite; x <= limite; x++) {
        for (int y = -limite; y <= limite; y++) {
            r = sqrt(x * x + y * y);
            z = (exp(-(r * r) / s)) / (M_PI * s);
            gaussResultado = (exp(-(r * r) / s)) / (M_PI * s);
            mascara[x + limite][y + limite] = gaussResultado;

            sum += gaussResultado;
            
            cout << gaussResultado << endl;
        }
    }

    for (int i = 0; i < mascSize; i++) {
        for (int j = 0; j < mascSize; j++) {
            mascara[i][j] /= sum;
        }
    }

    return mascara;
}

Mat matrizRelleno(int filas, int columnas, int mascSize)
{
    int diferenciaBordes = mascSize - 1;
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

Mat copiarImgARelleno(Mat bordes, Mat original, int mascSize)
{
    int diferenciaBordes = ((mascSize - 1) / 2);
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

float convolucionPixel(Mat matrizConBordes, vector<vector<float>> mascara, int mascSize, int x, int y) {

    int limites = (mascSize - 1) / 2;

    int filasImgBordes = matrizConBordes.rows;
    int columnasImgBordes = matrizConBordes.cols;

    float sumatoriaFiltro = 0.0;

    for (int i = -limites; i <= limites; i++) {
        for (int j = -limites; j <= limites; j++) {

            float valMascara = mascara[i + limites][j + limites];
            int coordY = y + j + limites;
            int coordX = x + i + limites;

            float valImagen = matrizConBordes.at<uchar>(coordY, coordX);

            sumatoriaFiltro += valMascara * valImagen;
        }
    }

    return sumatoriaFiltro;
}

Mat aplicarFiltroImagen(Mat imagenOriginal, Mat matrizConBordes, vector<vector<float>> mascara, int mascSize) {
    int filas = imagenOriginal.rows;
    int columnas = imagenOriginal.cols;

    Mat imagenFiltroAplicado(filas, columnas, CV_8UC1);
    
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++){
            uchar val = uchar(convolucionPixel(matrizConBordes, mascara, mascSize, i, j));
            imagenFiltroAplicado.at<uchar>(Point(i, j)) = val;
        }
    }

    return imagenFiltroAplicado;
}


 

int main()
{
    int mascSize = 0, filasImagen = 0, columnasImagen = 0;
    float sigma = 0.0;

    Mat image;
    char imageName[] = "lena.png";

    cout << "Digite el largo que tendra la mascara cuadrada para aplicar en la imagen: ";
    cin >> mascSize;
    cout << "Digite el valor de 'sigma' para usar en el filtro Gaussiano: ";
    cin >> sigma;

    if ((mascSize % 2) == 0)
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

    vector<vector<float>> mascara = rellenarMascara(mascSize, sigma);

    filasImagen = image.rows;
    columnasImagen = image.cols;

    Mat matrizConBordes = matrizRelleno(filasImagen, columnasImagen, mascSize);
    matrizConBordes = copiarImgARelleno(matrizConBordes, image, mascSize);

    Mat imagenFiltroAplicado = aplicarFiltroImagen(image, matrizConBordes, mascara, mascSize);

    namedWindow("Imagen original", WINDOW_AUTOSIZE);
    imshow("Imagen original", image);

    namedWindow("Imagen Bordes Extra", WINDOW_AUTOSIZE);
    imshow("Imagen Bordes Extra", matrizConBordes);

    namedWindow("Imagen con filtro", WINDOW_AUTOSIZE);
    imshow("Imagen con filtro", imagenFiltroAplicado);

    waitKey(0);

    return 1;
}