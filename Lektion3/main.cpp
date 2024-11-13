#include <stdio.h>
#include <opencv2/opencv.hpp>

/**
 * @brief ShowHist Displays a histogram of a single channel uchar image
 * @param grayImage Input image. Must be type CV_8UC1
 */

void showHist(cv::Mat grayImage){
    //kontrollerer at det er gråtone og initialiserer værdier til histogram
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

int main(int argc, char** argv )
{
    // //////////////////OPGAVE 1 - Histogram ////////////
    cv::Mat imageHist;
    imageHist = cv::imread( "/home/matmat1000/Documents/imgs/legoHouse.jpg", 1 );

    if ( !imageHist.data )
    {
        printf("No image data \n");
        return -1;
    }

    cv::Mat image_grey;
    cv::cvtColor(imageHist, image_grey, cv::COLOR_BGR2GRAY);

    cv::namedWindow("Display Image for hist", cv::WINDOW_AUTOSIZE );
    cv::imshow("Display Image for hist", image_grey);
    cv::waitKey(0);

    showHist( image_grey );


    // //////////////////OPGAVE 2 (b)-  histogram equalization på gråtone///////
    cv::Mat image = cv::imread("/home/matmat1000/Documents/imgs/TrinityCampanile3.jpg", cv::IMREAD_GRAYSCALE);
    cv::imshow("Input image", image);
    cv::waitKey(0);

    cv::MatND histogram;
    const int* channel_numbers = { 0 };
    float channel_range[] = { 0.0, 255.0 };
    const float* channel_ranges = channel_range;
    int number_bins = 256;
    for (int i=0; i < image.channels(); i++)
        cv::calcHist( &image, 1, channel_numbers, cv::Mat(), histogram, 1, &number_bins, &channel_ranges );

    std::cout << "histogram: " << histogram.rows << ", " << histogram.cols << std::endl;
    std::cout << "histogram.depth(): " << histogram.depth()  << std::endl;

    cv::Mat lookUpTable(1,256,CV_8U);
    uchar* p = lookUpTable.ptr();

    // Create a lookup table to map the luminances
    // h[x] = histogram of luminance values image f(i,j).
    int pixels_so_far = 0;
    int num_pixels = image.rows * image.cols;
    int output = 0;
    for(int i=0; i<255; i++){
        pixels_so_far = pixels_so_far + histogram.at<float>(i,0);
        output = (255* pixels_so_far) / num_pixels;
        p[i] = output;
        //std::cout << "p[" << i << "]: " << output << std::endl;
    }

    /*    Apply the lookup table LUT(x) to the image:
           for  every pixel f(i,j)
               f’(i,j) = LUT[ f(i,j) ]     */
    cv::Mat eqImage(image.rows,image.cols,CV_8UC1);
    bool useOpenCVLUT = true;
    if(useOpenCVLUT){
        cv::LUT(image,lookUpTable,eqImage);
    }else{
        for(int i=0; i<image.rows; i++){
            for(int j=0; j<image.cols; j++){
                int value = image.at<uchar>(i,j);
                eqImage.at<uchar>(i,j) = lookUpTable.at<uchar>(0,value);
            }
        }
    }

    cv::imshow("Equalized image", eqImage);
    cv::waitKey(0);

    /* Visualize difference between my own and OpenCV's implementation */
    cv::Mat cvEqImage;
    cv::equalizeHist(image, cvEqImage);
    cv::imshow("OpenCV Equalized image", cvEqImage);
    cv::waitKey(0);

    // //////////////////OPGAVE 2 (a)-  histogram equalization på farvebillede (HSV)/////

    // Læs det originale farvebillede
    cv::Mat colorImage = cv::imread("/home/matmat1000/Documents/imgs/TrinityCampanile3.jpg", cv::IMREAD_COLOR);
    if (!colorImage.data) {
        std::cout << "No image data \n";
        return -1;
    }

    // Vis det originale billede
    cv::imshow("Original Color Image", colorImage);
    cv::waitKey(0);

    // Konverter billedet fra BGR til HSV farverum
    cv::Mat hsvImage;
    cv::cvtColor(colorImage, hsvImage, cv::COLOR_BGR2HSV);

    // Split billedet i dets 3 kanaler (H, S, V)
    std::vector<cv::Mat> hsvChannels;
    cv::split(hsvImage, hsvChannels);

    // Anvend histogramudjævning på V-kanalen (value/lysintensiteten)
    cv::equalizeHist(hsvChannels[2], hsvChannels[2]);

    // Merge de justerede H-, S-, og V-kanaler
    cv::merge(hsvChannels, hsvImage);

    // Konverter tilbage fra HSV til BGR farverum
    cv::Mat equalizedColorImage;
    cv::cvtColor(hsvImage, equalizedColorImage, cv::COLOR_HSV2BGR);

    // Vis det udjævnede billede
    cv::imshow("Histogram Equalized Color Image (HSV)", equalizedColorImage);
    cv::waitKey(0);

    return 0;
}
