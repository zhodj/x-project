#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpc++/channel.h>
#include <grpc++/client_context.h>
#include <grpc++/create_channel.h>
#include <grpc++/security/credentials.h>
#include "image_guide.grpc.pb.h"
#include "helper/common.h"

#include <opencv2/opencv.hpp>

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using imageguide::Image;
using imageguide::ImageGuide;
using imageguide::Response;
using namespace helper::Common;
using namespace cv;
using namespace std;

class imageGuideClient {
public:
    imageGuideClient(std::shared_ptr<Channel> channel)
        : stub_(ImageGuide::NewStub(channel)) {
        }

    void sendImage() {
        Response response;
        ClientContext context;
        const int kPoints = 10;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> delay_distribution( 500, 1500);

        std::unique_ptr<ClientWriter<Image> > writer( stub_->SendImage(&context, &response));
        if (!writer->Write(this->image)) {
            // Broken stream.
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds( delay_distribution(generator)));
        writer->WritesDone();
        Status status = writer->Finish();
        if (status.ok()) {
            std::cout << "Finished send image " << std::endl;
        } else {
            std::cout << "Send Image rpc failed." << std::endl;
        }
    }

    int readImage(const char* image_file)
    {
        Mat image = imread(image_file, IMREAD_COLOR);
        if(!image.data)
        {
            std::cout << "Could not open or find the image" << std::endl;
            return -1;
        }
        std::vector<uchar> buff;
        this->matToBytes(image, buff);
        std::string s(buff.begin(), buff.end());
        this->image.set_icon(s);
        return 0;

    }

    void matToBytes(Mat image, vector<uchar>& buff)
    {
        std::cout << "Height: " << image.rows << " Width: " << image.cols << std::endl;
        vector<int> param = vector<int>(2);
        param[0] = CV_IMWRITE_JPEG_QUALITY;
        param[1] = 95; // defualt(95) 0-100
        imencode(".jpg", image, buff, param);
        std::cout << "Coded file size(jpg): " << buff.size() << std::endl;
    }

private:
    std::unique_ptr<ImageGuide::Stub> stub_;
    Image image;
};

int main(int argc, char** argv) {
    imageGuideClient guide( grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials())); 
    std::cout << "-------------- SendImage--------------" << std::endl;
    guide.readImage(argv[1]);
    guide.sendImage();

    return 0;
}
