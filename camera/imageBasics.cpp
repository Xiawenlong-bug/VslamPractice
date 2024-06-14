#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
    Mat image;
    image = imread("../../camera/ubuntu.png");

    if(image.data==nullptr){
        cerr<<"Error"<<endl;
        return -1;
    }

    cout<<"rows: "<<image.rows<<endl;
    cout<<"cols: "<<image.cols<<endl;
    cout<<"channels: "<<image.channels()<<endl;

    namedWindow("Display Image", WINDOW_AUTOSIZE);
    imshow("Display Image", image);

    chrono::steady_clock::time_point t1=chrono::steady_clock::now();

    //访问x,y处坐标的像素
    for (size_t y = 0;y<image.rows;y++){
        unsigned char * row_ptr=image.ptr<unsigned char>(y);
        for(size_t x = 0;x <image.cols;x++ ){
            unsigned char *data_ptr=&row_ptr[x*image.channels()];
            for (int c = 0; c != image.channels();c++) unsigned char data=data_ptr[c];
        }
    }

    chrono::steady_clock::time_point t2=chrono::steady_clock::now();
    chrono::duration<double> time_used=chrono::duration_cast<chrono::duration<double>>(t2-t1);
    cout<<"time cost: "<<time_used.count()<<"s"<<endl;
    //这样是深拷贝，对image_another的修改不会对image造成修改
    Mat image_another = image.clone();
    waitKey(0);
    return 0;


}