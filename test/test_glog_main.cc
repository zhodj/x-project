#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include "glog/logging.h"

#define ROOT_DIR "/home/fangfang/work/x-project/test/log/"

class Glog_test : public ::testing::Test {

};

int main(int argc, char *argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir = ROOT_DIR;

    FLAGS_stderrthreshold = google::GLOG_INFO;
    FLAGS_colorlogtostderr = true;
    FLAGS_severitysinglelog = true;

    FLAGS_logbufsecs = 0;
    FLAGS_max_log_size = 100;
    FLAGS_stop_logging_if_full_disk = true;
    google::SetLogDestination(google::GLOG_FATAL, "/home/fangfang/work/x-project/test/log/fatal_");
    google::SetLogDestination(google::GLOG_ERROR, "/home/fangfang/work/x-project/test/log/error_");
    google::SetLogDestination(google::GLOG_WARNING, "/home/fangfang/work/x-project/test/log/warning_");
    google::SetLogDestination(google::GLOG_INFO, "/home/fangfang/work/x-project/test/log/info_");

    for(int i = 1; i <= 100; i++)
    {
        LOG_IF(INFO, i == 100) << "LOG_IF(INFO, i == 100) google::COUNTER=" << google::COUNTER << " i=" << i;
        LOG_EVERY_N(INFO, 10) << "LOG_EVERY_N(INFO, 10) google::COUNTER=" << google::COUNTER << " i=" << i;
        LOG_IF_EVERY_N(WARNING, (i>50), 10) << "LOG_IF_EVERY_N(INFO, (i>50), 10) google::COUNTER=" << google::COUNTER << " i=" << i;
        LOG_FIRST_N(ERROR, 5) << "LOG_FIRST_N(INFO, 5) google::COUNTER=" << google::COUNTER << " i=" << i;
    }
    google::ShutdownGoogleLogging();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
