#include <iostream>
#include <opencv2/opencv.hpp>


/**
 * @brief Applies linear filter to an image
 * @param Input image, filter kernel (3x3), output image
 * @author Mathias Vesterled (Based on lecture 2)
 */
void linearFilter(cv::Mat input, cv::Mat kernel, cv::Mat &output){

    //nulpadding tilføjes rundt om biillede
    cv::Mat paddedInput;
    int padSize = 1;
    cv::copyMakeBorder(input, paddedInput, padSize, padSize, padSize, padSize, cv::BORDER_CONSTANT, cv::Scalar(0));

    //laver output billedet til samme størrelse som input blot med nuller
    output = cv::Mat::zeros(input.size(), input.type());

    //forloop der kører inputbillede igennem (uden nulpadding)
    for (int i = 0; i < input.rows; ++i){
        for (int j = 0; j < input.cols; ++j){
            //laver værdi til beregning og løber kernel igennem (+padsize for at kompensere for nuller)
            float pixelValue = 0;
            for (int k = -1; k <= 1; ++k){
                for (int l = -1; l <= 1; ++l){
                    pixelValue += paddedInput.at<uchar>(i+padSize-k, j+padSize-l)*kernel.at<float>(k+1, l+1);
                }
            }
            //sætter pixel i output-billede
            output.at<uchar>(i, j) = cv::saturate_cast<uchar>(pixelValue);
        }
    }


}

int main()
{
    std::cout << "Convolution opgave med openCV" << std::endl;

    //Load image as grayscale
    std::string filename = "/home/matmat1000/Documents/test_gaussian.png";
    cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);

    cv::namedWindow("src");
    cv::imshow("src",src);
    cv::waitKey(0);

    CV_Assert(src.type() == CV_8UC1);
    //Create uniform 3x3 kernel (CV_32FC1 = float med 1 kanal)
    cv::Mat kernel(3,3, CV_32FC1, cv::Scalar(1.0/9.0));
    CV_Assert(kernel.type() == CV_32FC1); //kontrol af kernel

    //Apply linear filter
    cv::Mat output;
    linearFilter(src, kernel, output);

    cv::namedWindow("Linear filter output");
    cv::imshow("Linear filter output",output);
    cv::waitKey(0);

    //Test with custom kernel on impulse image
    cv::Mat impulse = cv::Mat::zeros(5,5,CV_8UC1);
    impulse.at<uchar>(2,2) = 1;
    cv::Mat customKernel = (cv::Mat_<float>(3,3) << 1,2,3,4,5,6,7,8,9);

    linearFilter(impulse,customKernel,output);
    for(int i=0; i<output.rows; i++){
        for(int j=0; j<output.cols; j++){
            std::cout << (int) output.at<uchar>(i,j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
