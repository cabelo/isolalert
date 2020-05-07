/*

Author : Alessandro de Oliveira Faria (A.K.A. Cabelo)

wget https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/yolov3.cfg
wget https://pjreddie.com/media/files/yolov3.weights
wget https://raw.githubusercontent.com/pjreddie/darknet/master/data/coco.names

Build with oneAPI: 
/usr/bin/c++ -I/opt/intel/inteloneapi/vpl/latest/include  -o isolalert.cpp.o  -c isolalert.cpp
/usr/bin/c++ isolalert.cpp.o -lcurl  /opt/intel/inteloneapi/vpl/latest/lib/libopencv_world.so -o isolalert 

*/

#include <fstream>
#include <sstream>
#include <iostream>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "common.hpp"
#include <string>  

#include "curl/curl.h"
#include <iostream>

int _alert = 0;
int _count = 0;
std::string keys =
    "{ help  h     | | Print help message. }"
    "{ image       |<none>| Path to input image file.  }"
    "{ device      |  0 | camera device number. }"
    "{ classes     | coco.names | Optional path to a text file with names of classes to label detected objects. }"
    "{ scale       | 0.00392 | Optional path to a text file with names of classes to label detected objects. }"
    "{ thr         | .5 | Confidence threshold. }"
    "{ width       | 416 | Confidence threshold. }"
    "{ height      | 416 | Confidence threshold. }"
    "{ model       | yolov3.weights | Model file yolo. }"
    "{ config      | yolov3.cfg | Config file of yolo. }"
    "{ nms         | .4 | Non-maximum suppression threshold. }";


using namespace cv;
using namespace dnn;
using namespace std;

float confThreshold, nmsThreshold;
std::vector<std::string> classes;

cv::Mat curlImg(const char *img_url, int timeout=10);
size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata);
void postprocess(Mat& frame, const std::vector<Mat>& out, Net& net);

void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, Scalar cor);

std::vector<String> getOutputsNames(const Net& net);
std::string buildURL(string base);

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser = CommandLineParser(argc, argv, keys);
    parser.about("Use this script to run object detection deep learning networks using OpenCV.");
    if (argc == 1 || parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String imageFile = parser.get<String>("image");
    confThreshold = parser.get<float>("thr");
    nmsThreshold = parser.get<float>("nms");
    float scale = parser.get<float>("scale");
    int inpWidth = parser.get<int>("width");
    int inpHeight = parser.get<int>("height");
    std::string modelPath = parser.get<String>("model");
    std::string configPath = parser.get<String>("config");

    // Open file with classes names.
    if (parser.has("classes"))
    {
        std::string file = parser.get<String>("classes");
        std::ifstream ifs(file.c_str());
        if (!ifs.is_open())
            CV_Error(Error::StsError, "File " + file + " not found");
        std::string line;
        while (std::getline(ifs, line))
        {
            classes.push_back(line);
        }
    }

    // Load a model.
    Net net = readNet(modelPath, configPath);
    std::vector<String> outNames = net.getUnconnectedOutLayersNames();

    // Create a window
    static const std::string kWinName = "Deep learning object detection in OpenCV";
    //namedWindow(kWinName, WINDOW_NORMAL);
    //namedWindow(kWinName,WINDOW_AUTOSIZE);

    cv::Mat ovl = imread("i2ai.png");
    // Process frames.
    while(true)
    {
         _count++; 
         std::string _tmp; 
        _tmp = to_string(_count);
        Mat blob;
        Mat frame;
        cv::Mat image= curlImg(buildURL(imageFile).c_str());   
        //frame = imread(imageFile);
        if(_count==25) _count = 0;

        if (image.empty())
        {
            cout<<"Error in URL!"<<endl;
            continue; ;
        }
        resize(image, frame, Size(800,450));  

        // Create a 4D blob from a frame.
        Size inpSize(inpWidth > 0 ? inpWidth : frame.cols, inpHeight > 0 ? inpHeight : frame.rows);
        blobFromImage(frame, blob, scale, inpSize, Scalar(), false, false);

        net.setInput(blob);
        std::vector<Mat> outs;
        net.forward(outs, outNames);
        postprocess(frame, outs, net);
        cv::addWeighted( frame, 1, ovl,1,  0, frame );
        putText(frame, "Alessandro de Oliveira Faria - cabelo@opensuse.org", Point(10, 430), FONT_HERSHEY_SIMPLEX, 0.50, Scalar(255,255,255));
        imshow(kWinName, frame);
        std::string _save;
        _save = "/srv/www/htdocs/cam01/"+_tmp+".jpg";
        imwrite(_save,frame ); 
	     if(waitKey(6000)!=-1) break;
    }
    return 0;
}

size_t write_data(char *ptr, size_t size, size_t nmemb, void *userdata)
{   
    vector<uchar> *stream = (vector<uchar>*)userdata;
    size_t count = size * nmemb;
    stream->insert(stream->end(), ptr, ptr + count);
    return count;
}       
        
cv::Mat curlImg(const char *img_url, int timeout)
{       
    vector<uchar> stream;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, img_url); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &stream); 
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout); 
    CURLcode res = curl_easy_perform(curl); 
    curl_easy_cleanup(curl); 
    return imdecode(stream, -1); 
}       


std::string buildURL(string base)
{
   std::string count;
   std::string url;
   std::string ext = ".jpg?";
   std::string extF;
   count = to_string(_count);
   extF = ext + to_string( (long int)getTickCount());
   url = base+count+extF;
   return url;
}


float Distance( Point pt1, Point pt2 )
{
    int dx = pt2.x - pt1.x;
    int dy = pt2.y - pt1.y;
    return sqrt( (float)(dx*dx + dy*dy));
}

void postprocess(Mat& frame, const std::vector<Mat>& outs, Net& net)
{
    static std::vector<int> outLayers = net.getUnconnectedOutLayers();
    static std::string outLayerType = net.getLayer(outLayers[0])->type;

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;
    if (outLayerType == "Region")
    {
        for (size_t i = 0; i < outs.size(); ++i)
        {
            // Network produces output blob with a shape NxC where N is a number of
            // detected objects and C is a number of classes + 4 where the first 4
            // numbers are [center_x, center_y, width, height]
            float* data = (float*)outs[i].data;
            for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
            {
                Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
                Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold)
                {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back((float)confidence);
                    boxes.push_back(Rect(left, top, width, height));
                }
            }
        }
    }
    else
        CV_Error(Error::StsNotImplemented, "Unknown output layer type: " + outLayerType);

    std::vector<int> indices;
    std::vector<int> alert;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    _alert = 0;
    for (size_t i = 0; i < indices.size(); ++i)
    {
         alert.push_back (0);
    }

    for (size_t i = 0; i < indices.size(); ++i)
    {
        int  _idx1, _idx2;
        _idx1 = indices[i];
        Rect _box1, _box2;
        _box1 = boxes[_idx1];

        for (size_t t = (i+1); t < indices.size(); ++t)
        {
            _idx2 = indices[t];
            _box2 = boxes[_idx2];
            double _dist = Distance(Point(_box1.x,_box1.y),Point(_box2.x,_box2.y));
            if((_dist/2) < _box2.height)
            {
				    if((classes[classIds[_idx1]] == "person") && (classes[classIds[_idx2]] == "person"))
                {
                    alert[i] = 1;
                    alert[t] = 1;
                    _alert = 1;
                }
            }
        }
    }


    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        Scalar cor = (alert[i]==1?Scalar( 0,0,255 ):Scalar( 255,0,0 ));
        drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame, cor);
    }
}

void drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame, Scalar cor)
{
    if(classes[classId] != "person") cor = Scalar( 255,0,0 );
    if(_alert ==1) putText(frame,"ALERT" , Point(10, 50), FONT_HERSHEY_DUPLEX, 1, Scalar(0,0,255));
    if(cor == Scalar(0,0,255)) line(frame, Point(left+5, top), Point(40, 50),Scalar(0,0,255));

    rectangle(frame, Point(left, top), Point(right, bottom), cor ,2);

    std::string label = format("%.2f", conf);
    if (!classes.empty())
    {
        CV_Assert(classId < (int)classes.size());
        label = classes[classId] + ": " + label;
        label = classes[classId] ;
    }

    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.50, 1, &baseLine);
    top = max(top, labelSize.height);
    putText(frame, label, Point(left, top-5), FONT_HERSHEY_SIMPLEX, 0.50, Scalar(255,255,255));
}

