#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>

// Function to find the dominant angle using the strongest frequency components
float detectDominantOrientation(const cv::Mat& spectrum, int rows, int cols) {
    // Find the maximum value and its location in the spectrum
    cv::Mat normalizedSpectrum;
    cv::normalize(spectrum, normalizedSpectrum, 0, 255, cv::NORM_MINMAX);
    normalizedSpectrum.convertTo(normalizedSpectrum, CV_8U);

    cv::Point maxLoc;
    cv::minMaxLoc(spectrum, nullptr, nullptr, nullptr, &maxLoc);

    // Coordinates relative to the center (DC component)
    int centerX = cols / 2;
    int centerY = rows / 2;
    int u = maxLoc.x - centerX; // Frequency in horizontal direction
    int v = maxLoc.y - centerY; // Frequency in vertical direction

    // Compute the angle using the formula θ = atan2(v * C, u * R)
    float angle = std::atan2(v * cols, u * rows) * -180.0f / CV_PI; // Convert to degrees
    return angle;
}

void rotateImage(const cv::Mat& inputImage, float angle, cv::Mat& outputImage) {
    // Compute rotation matrix
    cv::Point2f center(inputImage.cols / 2.0F, inputImage.rows / 2.0F);
    cv::Mat rotationMatrix = cv::getRotationMatrix2D(center, angle, 1.0);

    // Rotate the image
    cv::warpAffine(inputImage, outputImage, rotationMatrix, inputImage.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
}

int main()
{
    // Load the input image in grayscale mode
    std::string imagePath = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lektion10_fourier/rotatedText.png";
    cv::Mat I = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);

    // Display the input image
    //cv::imshow("Picture", I);
    //cv::waitKey();

    // Prepare for DFT (Discrete Fourier Transform) by padding the image
    cv::Mat padded;
    int m = cv::getOptimalDFTSize(I.rows); // Optimal number of rows for DFT
    int n = cv::getOptimalDFTSize(I.cols); // Optimal number of columns for DFT

    // Add zero padding to the image to achieve optimal DFT size
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

    // Create a complex matrix with two channels: one for the image (real part) and another for zeros (imaginary part)
    cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
    cv::Mat complexI;
    merge(planes, 2, complexI); // Merge the two planes into a single complex matrix

    // Perform the Discrete Fourier Transform (DFT)
    dft(complexI, complexI);

    // Split the complex matrix into real and imaginary parts
    split(complexI, planes); // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

    // Compute the magnitude of the Fourier Transform: sqrt(Re^2 + Im^2)
    magnitude(planes[0], planes[1], planes[0]); // planes[0] now contains the magnitude
    cv::Mat magI = planes[0];

    // Switch to a logarithmic scale for better visualization: log(1 + magnitude)
    magI += cv::Scalar::all(1); // Add 1 to avoid log(0)
    log(magI, magI);

    // Crop the spectrum if the dimensions are odd (to ensure even dimensions)
    magI = magI(cv::Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // Rearrange the quadrants of the Fourier image to place the origin at the center
    int cx = magI.cols / 2; // Center x-coordinate
    int cy = magI.rows / 2; // Center y-coordinate

    // Define quadrants as Regions of Interest (ROIs)
    cv::Mat q0(magI, cv::Rect(0, 0, cx, cy));   // Top-Left quadrant
    cv::Mat q1(magI, cv::Rect(cx, 0, cx, cy));  // Top-Right quadrant
    cv::Mat q2(magI, cv::Rect(0, cy, cx, cy));  // Bottom-Left quadrant
    cv::Mat q3(magI, cv::Rect(cx, cy, cx, cy)); // Bottom-Right quadrant

    // Swap the quadrants to center the low frequencies in the Fourier spectrum
    cv::Mat tmp;
    q0.copyTo(tmp); // Swap Top-Left and Bottom-Right
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp); // Swap Top-Right and Bottom-Left
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // Normalize the magnitude matrix for better visualization (scales values to [0, 1])
    normalize(magI, magI, 0, 1, cv::NORM_MINMAX);

    // Display the original input image
    imshow("Input Image", I);

    // Display the Fourier spectrum (magnitude of the DFT)
    imshow("Spectrum Magnitude", magI);
    cv::Mat magI_normalized;
    normalize(magI, magI_normalized, 0, 1, cv::NORM_MINMAX); // Normalize to range [0, 1]
    magI_normalized.convertTo(magI_normalized, CV_8U, 255.0); // Convert to 8-bit range [0, 255]
    cv::imwrite("/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lektion10_fourier/spectrum.png", magI_normalized);

    // Wait for a key press to close the windows
    cv::waitKey();

    // Detect the dominant orientation
    float dominantAngle = detectDominantOrientation(magI, I.rows, I.cols);
    std::cout << "Dominant Angle (degrees): " << dominantAngle << std::endl;

    // Rotate the image
    cv::Mat rotatedImage;
    rotateImage(I, dominantAngle, rotatedImage);

    // Display the results
    cv::imshow("Input Image", I);
    cv::imshow("Spectrum Magnitude", magI);
    cv::imshow("Rotated Image", rotatedImage);
    cv::waitKey();

    return 0;
}
