#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/date_time/local_time/local_time.hpp>
#include <boost/format.hpp>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include <opencv2/opencv.hpp>
#include "image_guide.grpc.pb.h"
#include "glog/logging.h"
#include "helper/common.h"
#include "helper/base64.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using imageguide::Image;
using imageguide::Response;
using imageguide::ImageGuide;
using namespace helper::Common;

using namespace cv;
namespace bf = boost::filesystem;

class ImageGuideImpl final : public ImageGuide::Service {
public:
    explicit ImageGuideImpl() {
    }

    Status SendImage(ServerContext* context, ServerReader<Image>* reader, Response* response ) override {
        Image image;
        while(reader->Read(&image))
        {
            int id = image.id();
            LOG(INFO) << "Receive id: " << id;
            const std::string s = image.icon();
            std::vector<uchar> vec_data(s.begin(), s.end());
            
            Mat mat_image(imdecode(vec_data, 1));
            // LOG(INFO) << "Height: " << mat_image.rows << " Width: " << mat_image.cols;

            std::vector<int> compression_params = std::vector<int>(2);
            compression_params[0] = CV_IMWRITE_JPEG_QUALITY;
            compression_params[1] = 95;

            try{
                std::string now_date =  getNowByFormat("%Y%m%d");
                std::string image_folder = "./images/" + now_date;
                bf::path path = image_folder;
                if(!bf::exists(path))
                {
                    bool is_create = bf::create_directories(image_folder);
                    if(is_create)
                    {
                        std::string num = str(boost::format("%06d") % id);
                        std::string suffix = ".jpg";
                        std::string image_name = image_folder + "/" + "No." + num + suffix;
                        LOG(INFO) << "Writing image name: " << image_name;
                        imwrite(image_name, mat_image, compression_params);
                    }
                }else
                {
                    std::string num = str(boost::format("%06d") % id);
                    std::string suffix = ".jpg";
                    std::string image_name = image_folder + "/" + "No." + num + suffix;
                    LOG(INFO) << "Writing image name: " << image_name;
                    imwrite(image_name, mat_image, compression_params);
                }
            }catch(std::runtime_error& ex)
            {
                LOG(ERROR) << "Exception converting image to JPG format: " << ex.what();
                return  Status::CANCELLED;
            }
        }
        return Status::OK;
    }

};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ImageGuideImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    LOG(INFO) << "Start listening on " << server_address;
    server->Wait();
}

int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = "./log";

    RunServer();

    return 0;
}
