#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"


using namespace cv;
using std::cout;
using std::endl;

const char* keys =
    "{ help h |                  | Print help message. }"
    "{ input1 | box.png          | Path to input image 1. }"
    "{ input2 | box_in_scene.png | Path to input image 2. }";

int main(int argc, char* argv[])
{
    //INDLÆSNING AF BILLEDER
    std::string imagePath = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lek8_Feature/box.png";
    Mat img1 = imread(imagePath, IMREAD_GRAYSCALE);
    std::string imagePath2 = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lek8_Feature/box_in_scene.png";
    Mat img2 = imread(imagePath2, IMREAD_GRAYSCALE);
    if (img1.empty() || img2.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }

    //DETECTERING AF KEYPOINTS
    //-- Step 1: Detect keypoints and compute descriptors using AKAZE
    Ptr<AKAZE> detector = AKAZE::create();
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

    //-- Draw keypoints on pic 1
    detector->detect(img1, keypoints1);
    Mat img_keypoints;
    drawKeypoints(img1, keypoints1, img_keypoints);

    //-- Show detected (drawn) keypoints on pic 1
    imshow("AKAZE Keypoints", img_keypoints);

    waitKey();

    //MATCHER KEYPOINTS
    //-- Step 2: Matching descriptor vectors with a brute-force matcher
    // AKAZE uses binary descriptors, so NORM_HAMMING is preferred
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
    std::vector<DMatch> matches;
    matcher->match(descriptors1, descriptors2, matches);

    //-- Draw matches
    Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2, matches, img_matches);

    //-- Show detected matches
    imshow("Matches", img_matches);

    waitKey();

    //FLANN MATCH - Finder de gode matches
    Ptr<DescriptorMatcher> matcherFlann = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
    std::vector<std::vector<DMatch>> knn_matches;
    matcherFlann->knnMatch(descriptors1, descriptors2, knn_matches, 2);

    // Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    // Draw matches
    Mat img_matchesFlann;
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matchesFlann, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    imshow("Good Matches", img_matchesFlann);
    waitKey();

    // FIND OBJEKT
    Ptr<AKAZE> detector2 = AKAZE::create();
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    Mat descriptors_object, descriptors_scene;
    detector2->detectAndCompute(img1, noArray(), keypoints_object, descriptors_object);
    detector2->detectAndCompute(img2, noArray(), keypoints_scene, descriptors_scene);

    // Step 2: Matching descriptor vectors using brute-force matcher with Hamming norm
    Ptr<DescriptorMatcher> matcher2 = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);
    std::vector<std::vector<DMatch>> knn_matches2;
    matcher2->knnMatch(descriptors_object, descriptors_scene, knn_matches2, 2);

    // Filter matches using the Lowe's ratio test
    const float ratio_thresh2 = 0.75f;
    std::vector<DMatch> good_matches2;
    for (size_t i = 0; i < knn_matches2.size(); i++)
    {
        if (knn_matches2[i][0].distance < ratio_thresh2 * knn_matches2[i][1].distance)
        {
            good_matches2.push_back(knn_matches2[i][0]);
        }
    }

    // Draw matches
    Mat img_matches2;
    drawMatches(img1, keypoints_object, img2, keypoints_scene, good_matches2, img_matches2,
                Scalar::all(-1), Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for (size_t i = 0; i < good_matches2.size(); i++)
    {
        obj.push_back(keypoints_object[good_matches2[i].queryIdx].pt);
        scene.push_back(keypoints_scene[good_matches2[i].trainIdx].pt);
    }

    // Compute homography if we have enough matches
    if (good_matches2.size() >= 4)
    {
        Mat H = findHomography(obj, scene, RANSAC);

        // Get the corners from the image_1 (the object to be "detected")
        std::vector<Point2f> obj_corners(4);
        obj_corners[0] = Point2f(0, 0);
        obj_corners[1] = Point2f((float)img1.cols, 0);
        obj_corners[2] = Point2f((float)img1.cols, (float)img1.rows);
        obj_corners[3] = Point2f(0, (float)img1.rows);
        std::vector<Point2f> scene_corners(4);

        perspectiveTransform(obj_corners, scene_corners, H);

        // Draw lines between the corners (the mapped object in the scene - image_2)
        line(img_matches2, scene_corners[0] + Point2f((float)img1.cols, 0),
             scene_corners[1] + Point2f((float)img1.cols, 0), Scalar(0, 255, 0), 4);
        line(img_matches2, scene_corners[1] + Point2f((float)img1.cols, 0),
             scene_corners[2] + Point2f((float)img1.cols, 0), Scalar(0, 255, 0), 4);
        line(img_matches2, scene_corners[2] + Point2f((float)img1.cols, 0),
             scene_corners[3] + Point2f((float)img1.cols, 0), Scalar(0, 255, 0), 4);
        line(img_matches2, scene_corners[3] + Point2f((float)img1.cols, 0),
             scene_corners[0] + Point2f((float)img1.cols, 0), Scalar(0, 255, 0), 4);
    }

    imshow("Good Matches & Object detection", img_matches2);
    waitKey();


    return 0;
}

