#define _USE_MATH_DEFINES

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

double* fillGraussianMask(double sigma, int n) {
    int i = 0, limit = n / 2;
    double r, s, z;

    double* values;
    values = new double[n * n];

    s = 2.0 * sigma * sigma;

    for (int x = -limit; x <= limit; x++) {
        for (int y = -limit; y <= limit; y++) {
            r = sqrt(x * x + y * y);
            z = (exp(-(r * r) / s)) / (M_PI * s);

            values[i] = (exp(-(r * r) / s)) / (M_PI * s);

            i++;
        }
    }

    return values;
}

Mat createBorderedImage(int rows, int cols, int n) {
    int borderDifference = n - 1;

    Mat matrix(rows + borderDifference, cols + borderDifference, CV_8UC1);

    for (int i = 0; i < rows + borderDifference; i++) {
        for (int j = 0; j < cols + borderDifference; j++) {
            matrix.at<uchar>(Point(j, i)) = uchar(0);
        }
    }

    return matrix;
}

Mat adaptBorderedImage(Mat bordered, Mat original, int n) {
    int borderDifference = ((n - 1) / 2);

    for (int i = borderDifference; i < bordered.rows - borderDifference; i++) {
        for (int j = borderDifference; j < bordered.cols - borderDifference; j++) {
            bordered.at<uchar>(Point(j, i)) = original.at<uchar>(Point(j - borderDifference, i - borderDifference));
        }
    }

    return bordered;
}

int main() {
    Mat image;
    char imageName[] = "lena.png";
    int n = 0, imageRows = 0, imageCols = 0;

    double sigma = 0.0;

    cout << "Digite el largo de la mascara: ";
    cin >> n;
    cout << "Digite el valor de 'sigma': ";
    cin >> sigma;

    if ((n % 2) == 0) {
        cout << "La mascara debe ser de longitud impar";
        exit(1);
    }

    image = imread(imageName);
    if (!image.data) {
        cout << "Error al cargar la imagen: " << imageName << endl;
        exit(1);
    }

    cvtColor(image, image, COLOR_BGR2GRAY);

    double* maskArray = new double[n * n];
    maskArray = fillGraussianMask(sigma, n);

    cout << "Valores calculados para la mascara de " << n << " * " << n << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << maskArray[i + j];
            cout << "\t";
        }
        cout << "\n";
    }

    Mat borderedImage = createBorderedImage(image.rows, image.cols, n);
    borderedImage = adaptBorderedImage(borderedImage, image, n);

    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original image", image);

    namedWindow("Bordered image", WINDOW_AUTOSIZE);
    imshow("Bordered image", borderedImage);

    waitKey(0);
    return 1;
}