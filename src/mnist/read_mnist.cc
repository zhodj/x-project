#include <iostream>
#include <fstream>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int reverse_int(int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1 = i & 255;
    ch2 = (i >> 8) & 255;
    ch3 = (i >> 16) & 255;
    ch4 = (i >> 24) & 255;
    return ((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}
void read_mnist_files(string& filename, vector<cv::Mat>& vec)
{
    ifstream file(filename.c_str(), ios::binary);
    if (!file.fail()) {  
        int magic_number = 0;  
        int number_of_images = 0;  
        // int n_rows = 0;  
        // int n_cols = 0;  
        file.read((char*) &magic_number, sizeof(int));  
        magic_number = reverse_int(magic_number);  
#if 1
            cout << magic_number << endl;
        file.read((char*) &number_of_images,sizeof(int));  
        number_of_images = reverse_int(number_of_images);  
            cout << number_of_images << endl;
#else
        file.read((char*) &n_rows, sizeof(int));  
        n_rows = reverse_int(n_rows);  
        file.read((char*) &n_cols, sizeof(int));  
        n_cols = reverse_int(n_cols);  

        for(int i = 0; i < number_of_images; ++i) {  
            cv::Mat tp = cv::Mat::zeros(n_rows, n_cols, CV_8UC1);  
            for(int r = 0; r < n_rows; ++r) {  
                for(int c = 0; c < n_cols; ++c) {  
                    unsigned char temp = 0;  
                    file.read((char*) &temp, sizeof(temp));  
                    tp.at<uchar>(r, c) = (int) temp;  
                }  
            }  
            vec.push_back(tp);  
        }  
#endif
    }  
}

void read_mnist_label(string& filename, vector<int> &vec)
{
    ifstream file(filename.c_str(), ios::binary);
    if(file.is_open())
    {
        int magic_number = 0;
        int number_of_images = 0;

        file.read((char*)&magic_number, sizeof(magic_number));
        magic_number = reverse_int(magic_number);
#if 1
        cout << magic_number << endl;
        file.read((char*)&number_of_images, sizeof(number_of_images));
        number_of_images = reverse_int(number_of_images);
        cout << number_of_images << endl;

#else
        for(int i = 0;i < number_of_images;++i)
        {
            unsigned char temp = 0;
            file.read((char*)&temp, sizeof(temp));
            vec[i] = (int)temp;
        }
#endif
    }
}

string get_image_name(int number, int arr[])
{
    string str1, str2;

    for (int i = 0;i < 10;i++) {
        if (number == i) {
            arr[i]++;
            char ch1[10];
            sprintf(ch1, "%d", arr[i]);
            str1 = std::string(ch1);

            if (arr[i] < 10) {
                str1 = "0000" + str1;
            }else if (arr[i] < 100) {
                str1 = "000" + str1;
            }else if (arr[i] < 1000) {
                str1 = "00" + str1;
            }else if (arr[i] < 10000) {
                str1 = "0" + str1;
            }

            break;
        }
    }

    char ch2[10];
    sprintf(ch2, "%d", number);
    str2 = std::string(ch2);

    str2 = str2 + "_" + str1;

    return str2;
}

int main()
{
    string filename_images = "/home/fangfang/Workspace/mnist/source/train-images-idx3-ubyte";
    vector<cv::Mat> vec_images;
    read_mnist_files(filename_images, vec_images);

    string filename_labels = "/home/fangfang/Workspace/mnist/source/train-labels-idx1-ubyte";
    int number_of_images = 60000;
    vector<int> vec_labels(number_of_images);
    read_mnist_label(filename_labels, vec_labels);

    if(vec_images.size() != vec_labels.size())
    {
        cout << "images size: " << vec_images.size() << " labels size: " << vec_labels.size() << endl;
        return -1;
    }
    int count[10];
    for(int i = 0; i < 10; i++)
        count[i] = 0;

    string save_images_path = "/home/fangfang/Workspace/mnist/train/";
    for(unsigned int i = 0; i < vec_images.size(); i++)
    {
        int number = vec_labels[i];
        string image_name = get_image_name(number, count);
        image_name = save_images_path + image_name + ".jpg";
        cv::imwrite(image_name, vec_images[i]);
    }
    return 0;


}
