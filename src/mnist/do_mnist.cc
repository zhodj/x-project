#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <dirent.h>
#include <string.h>
#include <boost/algorithm/string.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace boost::algorithm;
using namespace cv;

unsigned char handle_filename(string& filename);

void intToBytes(vector<unsigned char> & arrayofByte, int paramInt)
{
   unsigned char c[4] = {0};
   for(int i = 0; i < 4; i++) 
   {
       c[i] = (paramInt >> (i * 8));
   }
   arrayofByte.push_back(c[3]);
   arrayofByte.push_back(c[2]);
   arrayofByte.push_back(c[1]);
   arrayofByte.push_back(c[0]);
}

void charToBytes(vector<unsigned char> & arrayofByte, unsigned char paramChar)
{
   unsigned char c[1] = {0};
   for(int i = 0; i < 1; i++) 
   {
       c[i] = (paramChar >> (i * 8));
   }
   arrayofByte.push_back(c[1]);
}

void read_source_files(string& pathname)
{
    int length = 0, length_label = 0;
    int len = 0;
    struct dirent *ptr;
    DIR *dir = NULL;
    dir = opendir(pathname.c_str());
    vector<string> files;
    while((ptr = readdir(dir)) != NULL)
    {
        if(ptr->d_name[0] == '.')
            continue;
        else
            files.push_back(ptr->d_name);
    }
    vector<unsigned char> train_bytes;
    vector<unsigned char> label_bytes;

    int magic_number = 0x00000803;
    int magic_label_number = 0x00000801;

    intToBytes(train_bytes, magic_number);
    length = length + sizeof(int);

    intToBytes(label_bytes, magic_label_number);
    length_label = length_label + sizeof(int);


    int image_numbers = 10000;
    intToBytes(train_bytes, image_numbers);
    length = length + sizeof(int);

    intToBytes(label_bytes, image_numbers);
    length_label = length_label + sizeof(int);

    int row_numbers = 28;
    intToBytes(train_bytes, row_numbers);
    length = length + sizeof(int);
    int col_numbers = 28;
    intToBytes(train_bytes, col_numbers);
    length = length + sizeof(int);

    for(size_t i = 0; i < files.size(); ++i)
    {
        string filepath = pathname + "/" + files[i];
        unsigned char label =  handle_filename(files[i]);
        Mat image;
        image = imread(filepath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if(!image.data)
        {
            cout << "Could not open or find the image" << endl;
            return ;
        }else
        {
            for(int i = 0; i < image.rows; i++)
                for(int j = 0; j < image.cols; j++)
                {
                    charToBytes(train_bytes, image.at<uchar>(i,j));
                    length = length + sizeof(unsigned char);
                }
        }

        charToBytes(label_bytes, label);
        length_label = length_label + sizeof(unsigned char);
    }
    closedir(dir);

    ofstream outfile("t10k-images-idx3-ubyte", ofstream::binary);
    copy(train_bytes.begin(),  train_bytes.end(), ostream_iterator<char>(outfile));
    outfile.close();

    ofstream outlabel("t10k-labels-idx1-ubyte", ofstream::binary);
    copy(label_bytes.begin(), label_bytes.end(), ostream_iterator<char>(outlabel));
    outlabel.close();

    cout << "length: " << length << endl << "length_label: " << length_label << endl;
    
}

unsigned char handle_filename(string& filename)
{
    vector<string> strVec;
    split(strVec, filename, is_any_of("."));
    string name_prefix = strVec[0];
    strVec.clear();
    split(strVec, name_prefix, is_any_of("_"));
    string number = strVec[0];
    unsigned char i = std::stoi(number);

    return i;
}

int main()
{
    string pathname = "/home/zhoudingjun/workstation/scripts/python/test_numbers/";
    read_source_files(pathname);
}
