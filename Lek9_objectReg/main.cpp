#include "opencv2/objdetect.hpp" // Inkluderer biblioteket for objektdetektion, fx Haar-cascader.
#include "opencv2/highgui.hpp"   // Inkluderer biblioteket til GUI-vinduer og billedhåndtering.
#include "opencv2/imgproc.hpp"   // Inkluderer biblioteket til billedbehandling, fx konvertering til gråtone.
#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame); // Deklarerer en funktion til at detektere ansigter og vise resultatet.

CascadeClassifier face_cascade;  // Objekt til at indlæse og bruge Haar-cascade for ansigtsdetektion.
CascadeClassifier eyes_cascade;  // Objekt til at indlæse og bruge Haar-cascade for øjendetektion.

int main(int argc, const char** argv)
{
    // Sti til Haar-cascade-filen for ansigtsdetektion.
    String face_cascade_name = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lek9_objectReg/haarcascade_frontalface_alt.xml";

    // Sti til Haar-cascade-filen for øjendetektion.
    String eyes_cascade_name = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lek9_objectReg/haarcascade_eye_tree_eyeglasses.xml";

    // Sti til input-billedet, der skal analyseres.
    String input_image_path = "/home/matmat1000/C++/Machine_Vision/Git/Computer-Vision/Lek9_objectReg/people2.png";

    //-- 1. Indlæs Haar-cascade-filerne
    if (!face_cascade.load(face_cascade_name))  // Forsøger at indlæse Haar-cascade til ansigter.
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    }
    if (!eyes_cascade.load(eyes_cascade_name))  // Forsøger at indlæse Haar-cascade til øjne.
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    }

    //-- 2. Læs input-billedet
    Mat frame = imread(input_image_path);  // Indlæser billedet fra den angivne sti.
    if (frame.empty())  // Tjekker, om billedet blev indlæst korrekt.
    {
        cout << "--(!)Error loading image\n";
        return -1;
    }

    //-- 3. Anvend Haar-cascade til billedet
    detectAndDisplay(frame);  // Kald funktionen til at detektere ansigter og vise resultatet.

    waitKey(0);
    return 0;
}

void detectAndDisplay(Mat frame)
{
    Mat frame_gray;  // Opretter en variabel til det gråtone-konverterede billede.
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);  // Konverter billedet til gråtone.
    equalizeHist(frame_gray, frame_gray);  // Forbedrer kontrasten i gråtonebilledet.

    //-- Detekter ansigter
    std::vector<Rect> faces;  // Vektor til at gemme koordinater for detekterede ansigter.
    face_cascade.detectMultiScale(frame_gray, faces);  // Detekter ansigter i gråtonebilledet.
    for (size_t i = 0; i < faces.size(); i++)  // Iterer over hvert detekteret ansigt.
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);  // Beregn centerpunkt for ansigtet.
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);  // Tegn en ellipse omkring ansigtet.
        Mat faceROI = frame_gray(faces[i]);  // Opret en Region of Interest (ROI) for ansigtet i gråtonebilledet.

        //-- I hvert ansigt, detekter øjne
        std::vector<Rect> eyes;  // Vektor til at gemme koordinater for detekterede øjne.
        eyes_cascade.detectMultiScale(faceROI, eyes);  // Detekter øjne inden for ansigtets ROI.
        for (size_t j = 0; j < eyes.size(); j++)  // Iterer over hvert detekteret øje.
        {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);  // Beregn centerpunkt for øjet.
            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);  // Beregn radius for cirklen omkring øjet.
            circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);  // Tegn en cirkel omkring øjet.
        }
    }

    //-- Vis resultatet
    imshow("Capture - Face detection", frame);  // Vis det oprindelige billede med detekterede ansigter og øjne markeret.
}
