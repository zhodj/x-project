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

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include <opencv2/opencv.hpp>
#include "image_guide.grpc.pb.h"
#include "glog/logging.h"
#include "helper/xmlparser.h"
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
using namespace boost::posix_time;

using namespace cv;

class ImageGuideImpl final : public ImageGuide::Service {
public:
    explicit ImageGuideImpl() {
    }

    Status SendImage(ServerContext* context, ServerReader<Image>* reader, Response* response ) override {
        Image image;
        while(reader->Read(&image))
        {
            int id = image.id();
            const std::string s = image.icon();
            std::vector<uchar> vec_data(s.begin(), s.end());
            
            std::cout << "Vector length: " << vec_data.size() << std::endl;
            Mat mat_image(imdecode(vec_data, 1));
            std::cout << "Height: " << mat_image.rows << " Width: " << mat_image.cols << std::endl;

            std::vector<int> compression_params = std::vector<int>(2);
            compression_params[0] = CV_IMWRITE_JPEG_QUALITY;
            compression_params[1] = 95;

            try{
                std::string now = to_simple_string(second_clock::local_time());
                std::string image_folder = "./images/" + now;
                bool is_create = boost::filesystem::create_directories(image_folder);
                if(is_create)
                {
                    std::string image_name = "number" + ".jpg";
                    imwrite(image_name, mat_image, compression_params);
                }else
                {
                }
            }catch(std::runtime_error& ex)
            {
                std::cout << "Exception converting image to JPG format: " << ex.what() << std::endl;
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

    // Expect only arg: --db_path=path/to/image_guide_db.json.
    RunServer();

    return 0;
}
