#include <iostream>
#include <opencv2/opencv.hpp>

/**
 * @brief ShowHist Displays a histogram of a single channel uchar image (from lecture 3)
 * @param grayImage Input image. Must be type CV_8UC1
 * @author Thorbjoern Mosekjaer Iversen (Based on OpenCV histogram tutorial)
 */
void showHist(cv::Mat grayImage){
    CV_Assert(grayImage.type()==CV_8UC1);
    int histSize = 256;
    float range[] = { 0, 256 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;

    cv::Mat hist;
    cv::calcHist( &grayImage, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

    //Initialize histogram image
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

    // Normalize the result to [ 0, histImage.rows ]
    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    // Draw line between each (bin,value)
    for( int i = 1; i < histSize; i++ ){
        cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
                 cv::Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
                 cv::Scalar( 255, 0, 0), 2, 8, 0  );
    }

    // Display
    cv::namedWindow("Histogram", cv::WINDOW_AUTOSIZE );
    cv::imshow("Histogram", histImage );

    cv::waitKey(0);
}

/**
 * @brief countNumberOfObjects counts number of objects in a picture. Optimized for rice (lecture 4)
 * @param Input image as grayscale
 * @author Mathias Vesterled (Based on lecture 4)
 */

int countNumberOfObjects(cv::Mat input){
    //creates  empty copy of imput image
    cv::Mat output = input.clone();
    output.setTo(cv::Scalar(0, 0, 0));

    //Chosen "Otsu" due to the 2 normal distributions. Otsu finds the threshold value by itself
    //Not chosen adaptive thresholding as there is a uniform background, that makes a static threshold good
    cv::threshold(input, output, 0, 255, cv::THRESH_OTSU);

    // Show the result threshold
    cv::namedWindow("Result Threshold");
    cv::imshow("Result Threshold", output);
    cv::waitKey(0);

    // Applying dilation - Defining the kernel for dilation.
    // Set to 0, as this has the best effect for this picture - dilation is off!!!
    int dilation_size = 0;

    cv::Mat elementDilation = cv::getStructuringElement(cv::MORPH_RECT,
                                                        cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
                                                        cv::Point(dilation_size, dilation_size));


    cv::dilate(output, output, elementDilation);

/*
    // Show the result after dilation
    cv::namedWindow("Dilated Output");
    cv::imshow("Dilated Output", output);
    cv::waitKey(0);
*/
    // Perform erosion after dilation
    int erosion_size =3;
    cv::Mat elementErosion = cv::getStructuringElement(cv::MORPH_RECT,
                                                       cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
                                                       cv::Point(erosion_size, erosion_size));
    cv::erode(output, output, elementErosion);

    // Show the result after erosion
    cv::namedWindow("Eroded Output");
    cv::imshow("Eroded Output", output);
    cv::waitKey(0);

    // Now applying cv::connectedComponents to count number of rice (8-connectivity) (-1 to exclude background)
    cv::Mat rice;
    return cv::connectedComponents(output, rice, 8)-1;
}

int main()
{
    std::cout << "Binary Vision exam exercise" << std::endl;
    //Load image as grayscale
    std::string filename = "/home/matmat1000/Documents/rice.png";
    cv::Mat src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
    showHist(src);

    cv::namedWindow("Input image",cv::WINDOW_FULLSCREEN);
    cv::imshow("Input image", src);
    cv::waitKey(0);

    std::cout << "Number of objects: ";
    std::cout << countNumberOfObjects(src);
    std::cout << std::endl;

    return 0;
}
