#include <iostream>
#include <opencv2/opencv.hpp>



int main()
{

    cv::Mat input = cv::imread("/home/matmat1000/Documents/chessboard.jpg", cv::IMREAD_COLOR);
    cv::namedWindow("chessboard before", cv::WINDOW_NORMAL); // Make window resizable
    cv::resizeWindow("chessboard before", 600, 400); // Resize window to specific size
    cv::imshow("chessboard before", input);
    cv::waitKey(0);

    //Real world transformation of coordiantes
    // Image points (corresponding to real-world points)
    std::vector<cv::Point2f> imagePoints;
    imagePoints.push_back(cv::Point2f(58, 1157)); // bottom left
    imagePoints.push_back(cv::Point2f(308, 172)); // top left
    imagePoints.push_back(cv::Point2f(1603, 169)); // top right
    imagePoints.push_back(cv::Point2f(1872, 1141)); // bottom right

    // Real world points
    std::vector<cv::Point2f> realWorldPoints;
    realWorldPoints.push_back(cv::Point2f(0.0f, 1000.0f));     // (0, 0)
    realWorldPoints.push_back(cv::Point2f(0.0f, 0.0f));    // (2.5, 57.5)
    realWorldPoints.push_back(cv::Point2f(1000.0f, 0.0f));    // (80, 0)
    realWorldPoints.push_back(cv::Point2f(1000.0f, 1000.0f));   // (67.5, 57.5)

    // Compute the homography matrix
    cv::Mat mHomoMat = cv::getPerspectiveTransform(imagePoints, realWorldPoints);

    // Apply the perspective transformation
    cv::Mat output;
    cv::warpPerspective(input, output, mHomoMat, cv::Size(1000,1000)); // Use input size or desired output size

    // Display the transformed image
    cv::namedWindow("chessboard after", cv::WINDOW_NORMAL); // Make window resizable
    cv::resizeWindow("chessboard after", 600, 400);       // Resize window to specific size
    cv::imshow("chessboard after", output);
    cv::waitKey(0); // Wait for a key press


    //LEGO TEST

    cv::Mat lego = cv::imread("/home/matmat1000/Documents/lego.JPEG", cv::IMREAD_COLOR);

    // Show the original LEGO image
    cv::namedWindow("lego before", cv::WINDOW_NORMAL); // Make window resizable
    cv::resizeWindow("lego before", 600, 400); // Resize window to specific size
    cv::imshow("lego before", lego);
    cv::waitKey(0); // Wait for a key press

    // Define image points (coordinates in the image)
    std::vector<cv::Point2f> imagePointsLego;
    imagePointsLego.push_back(cv::Point2f(1212, 900));   // Top-left corner
    imagePointsLego.push_back(cv::Point2f(2716, 610));   // Top-right corner
    imagePointsLego.push_back(cv::Point2f(1212, 2018));  // Bottom-left corner
    imagePointsLego.push_back(cv::Point2f(2639, 2642));  // Bottom-right corner

    // Define real-world points (desired output coordinates: 1000x600)
    std::vector<cv::Point2f> realWorldPointsLego;
    realWorldPointsLego.push_back(cv::Point2f(0.0f, 0.0f));        // Top-left corner
    realWorldPointsLego.push_back(cv::Point2f(1000.0f, 0.0f));     // Top-right corner
    realWorldPointsLego.push_back(cv::Point2f(0.0f, 600.0f));      // Bottom-left corner
    realWorldPointsLego.push_back(cv::Point2f(1000.0f, 600.0f));   // Bottom-right corner

    // Compute the homography matrix
    cv::Mat mHomoMatLego = cv::getPerspectiveTransform(imagePointsLego, realWorldPointsLego);

    // Apply the perspective transformation
    cv::Mat outputLego;
    cv::warpPerspective(lego, outputLego, mHomoMatLego, cv::Size(1000, 600)); // Output size 1000x600

    // Display the transformed image
    cv::namedWindow("lego after", cv::WINDOW_NORMAL); // Make window resizable
    cv::resizeWindow("lego after", 600, 400);       // Resize window to specific size
    cv::imshow("lego after", outputLego);
    cv::waitKey(0); // Wait for a key press


    return 0;
}
