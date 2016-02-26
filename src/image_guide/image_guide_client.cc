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

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using imageguide::Image;
using imageguide::ImageGuide;
using imageguide::Response;

class imageGuideClient {
public:
    imageGuideClient(std::shared_ptr<Channel> channel)
        : stub_(ImageGuide::NewStub(channel)) {
        }

    void sendImage() {
        Image image;
        Response response;       
        ClientContext context;    
        const int kPoints = 10;   
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        std::default_random_engine generator(seed);
        std::uniform_int_distribution<int> delay_distribution( 500, 1500);

        std::unique_ptr<ClientWriter<Image> > writer( stub_->SendImage(&context, &response));
        if (!writer->Write(image)) {
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

private:
    std::unique_ptr<ImageGuide::Stub> stub_;
};

int main(int argc, char** argv) {
    imageGuideClient guide(
                           grpc::CreateChannel("localhost:50051",
                                               grpc::InsecureChannelCredentials())
                          );

    std::cout << "-------------- SendImage--------------" << std::endl;
    guide.sendImage();

    return 0;
}
