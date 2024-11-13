#include <iostream>
#include <opencv2/opencv.hpp>



int main(int argc, char** argv )
{
    std::cout << "Lektion 1 - Billedetest med openCV" << std::endl;

    //afslutter, hvis der ikke er præcist et argument givet
    if ( argc != 2 )
    {
        std::cout << "You must provide the path to the image as an argument,"
                     " or the number of arguments is incorrect" << std::endl;
        return -1;
    }

    //laver image objekt og læser ind fra argument
    cv::Mat image;
    image = cv::imread("/home/matmat1000/Documents/imgs/legoHouse.jpg", 1);

    //tjekker om billededata er valid
    if ( !image.data )
    {
        std::cout << "No image data found" << std::endl;
        return -1;
    }

    //Loader vindue
    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    //Viser billede i vinduet
    cv::imshow("Display Image", image);

    //udskriver billededata
    std::cout << "Depth: " << image.depth() << " = CV_8U" << std::endl;
    std::cout << "Channels: " << image.channels() << std::endl;
    std::cout << "Type: " << image.type() << std::endl;
    std::cout << "Cols: " << image.cols << std::endl;
    std::cout << "Rows: " << image.rows << std::endl;

    //laver et nyt billede der bruger data fra image (byttet rækker og kolonner
    cv::Mat image2(image.cols, image.rows, image.type());

    for (int i = 0; i < image.rows; ++i){
        for  (int j  = 0; j < image.cols; ++j){
            //Transpose ved at bytte pixels individuelt
    //image2.at<cv::Vec3b>(j,i)[0] = image.at<cv::Vec3b>(i,j)[0];//blue
    //image2.at<cv::Vec3b>(j,i)[1] = image.at<cv::Vec3b>(i,j)[1]; //green
    //image2.at<cv::Vec3b>(j,i)[2] = image.at<cv::Vec3b>(i,j)[2]; //red
            //korrekt rotering
    image2.at<cv::Vec3b>(j, image.rows - 1 - i) = image.at<cv::Vec3b>(i, j);
    }
    }

    //Loader vindue
    cv::namedWindow("Display Image after rotation", cv::WINDOW_AUTOSIZE );
    //Viser billede i vinduet
    cv::imshow("Display Image after rotation", image2);

    cv::waitKey(0);

    //Indlæsning af billede ske
    cv::Mat src;
    src = cv::imread("/home/matmat1000/Documents/imgs/BabyFood-Sample1.jpg", 1);

    //konverting til HLS Color Space: (Hue-Luminance-Saturation)
    cv::Mat src_hls;
    cv::cvtColor(src, src_hls, cv::COLOR_BGR2HLS);

    //Splitter i BGR og HLS Channels: (Blue-Green-Red) og (Hue-Luminance-Saturation)
    std::vector<cv::Mat> bgrChannels, hlsChannels;
    cv::split(src,bgrChannels);
    cv::split(src_hls,hlsChannels);

    //Laver vinduer til alle visninger:
    int flags = cv::WINDOW_AUTOSIZE;
    cv::namedWindow("src",flags);
    cv::namedWindow("Blue",flags);      //Blue
    cv::namedWindow("Green",flags);     //Green
    cv::namedWindow("Red",flags);       //Red
    cv::namedWindow("Hue",flags);       //Hue
    cv::namedWindow("Luminance",flags); //Luminance/Lightness
    cv::namedWindow("Saturation",flags);//Saturation

    //Viser alle kanalerne i de lavede vinduer
    cv::imshow("Blue",bgrChannels[0]);          //Blue
    cv::imshow("Green",bgrChannels[1]);         //Green
    cv::imshow("Red",bgrChannels[2]);           //Red
    cv::imshow("Hue",hlsChannels[0]);           //Hue
    cv::imshow("Luminance",hlsChannels[1]);     //Luminance/Lightness
    cv::imshow("Saturation",hlsChannels[2]);    //Saturation
    cv::waitKey(0);

    /* Note hat the following is hard coded with segmentation values
     for the image BabyFood-Sample1.jpg */

    /*Kigger om alle værdier i HLS er over 150 (Hue-Luminance-Saturation)
     * Hvis de er det, så gemmes de i det endelige billede spoon_hls */
    cv::Mat spoon_hls = cv::Mat::zeros(src_hls.rows, src_hls.cols, src_hls.type());
    for(int i=0; i<src_hls.rows; i++){
    for(int j=0; j<src_hls.cols; j++){
    cv::Vec3b pixel = src_hls.at<cv::Vec3b>(i,j);
    if(pixel[0] > 150 && pixel[1] < 150 && pixel[2] > 150){ //værdier kan ændres
        spoon_hls.at<cv::Vec3b>(i,j) = pixel;
    }
    }
    }

    //Konverterer tilbage til BGR (Blue-Grren-Red) og viser billede:
    cv::Mat spoon;
    cv::cvtColor(spoon_hls, spoon, cv::COLOR_HLS2BGR);
    cv::namedWindow("spoon");
    cv::imshow("spoon",spoon);
    cv::waitKey(0);

    return 0;
}
