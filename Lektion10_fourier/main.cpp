#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"

int main()
{
    std::cout << "Hello World!" << std::endl;
    std::cout << "Neek" << std::endl;

    std::string imagePath = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lektion10_fourier/rotatedText.png";
    cv::Mat img1 = cv::imread(imagePath);

    cv::imshow("Picture", img1);
    cv::waitKey();

    return 0;
}
