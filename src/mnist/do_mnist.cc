#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <dirent.h>
#include <string.h>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace boost::algorithm;

char handle_filename(string& filename);
void read_source_files(string& pathname, char* img_src, char* label_src)
{
    int length = 0, length_label = 0;
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
    char * p = img_src;
    char * q = label_src;

    int magic_number = 0x00000803;
    int magic_label_number = 0x00000801;

    memcpy(p, &magic_number, sizeof(int));
    p = p + sizeof(int);
    length = length + sizeof(int);


    memcpy(q, &magic_label_number, sizeof(int));
    q = q + sizeof(int);
    length_label = length_label + sizeof(int);

    int image_numbers = 60000;
    memcpy(p, &image_numbers, sizeof(int));
    p = p + sizeof(int);
    length = length + sizeof(int);

    memcpy(q, &image_numbers, sizeof(int));
    q = q + sizeof(int);
    length_label = length_label + sizeof(int);

    int row_numbers = 28;
    memcpy(p, &row_numbers, sizeof(int));
    p = p + sizeof(int);
    length = length + sizeof(int);
    int col_numbers = 28;
    memcpy(p, &col_numbers, sizeof(int));
    p = p + sizeof(int);
    length = length + sizeof(int);

    for(size_t i = 0; i < files.size(); ++i)
    {
        string filepath = pathname + "/" + files[i];
        char label =  handle_filename(files[i]);
        ifstream binfile;
        binfile.open(filepath.c_str(), ifstream::in|ifstream::binary);
        while(binfile.good())
        {
            char c = binfile.get();
            memcpy(p, &c, sizeof(char));
            p = p + sizeof(char);
            length = length + sizeof(char);
        }
        binfile.close();

        memcpy(q, &label, sizeof(char));
        q = q + sizeof(char);
        length_label = length_label + sizeof(char);
    }
    closedir(dir);

    ofstream outfile("train-images-idx3-ubyte", ofstream::binary);
    copy(img_src, img_src + length, ostream_iterator<char>(outfile));
    outfile.close();


    ofstream outlabel("train-labels-idx1-ubyte", ofstream::binary);
    copy(label_src, label_src + length_label, ostream_iterator<char>(outlabel));
    outlabel.close();
    
}

char handle_filename(string& filename)
{
    vector<string> strVec;
    split(strVec, filename, is_any_of("."));
    string name_prefix = strVec[0];
    strVec.clear();
    split(strVec, name_prefix, is_any_of("_"));
    string number = strVec[0];
    char i = std::stoi(number);

    return i;
}

int main()
{
    string pathname = "/home/fangfang/Workspace/scripts/train_numbers/";
    char * img_src = new char[1024 * 1024 * 60];
    char * label_src = new char[1024 * 1024 * 60];
    read_source_files(pathname, img_src, label_src);
    delete[] img_src;
    delete[] label_src;
}
