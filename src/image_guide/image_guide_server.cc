#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/security/server_credentials.h>
#include "image_guide.grpc.pb.h"
#include "glog/logging.h"
#include "helper/xmlparser.h"

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

class ImageGuideImpl final : public ImageGuide::Service {
public:
    explicit ImageGuideImpl() {
    }

    Status SendImage(ServerContext* context, ServerReader<Image>* reader, Response* response ) override {
        Image image;
        while(reader->Read(&image))
        {
            const std::string s = image.icon();
            const void* data = s.c_str();
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
