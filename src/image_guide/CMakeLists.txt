FILE(GLOB IMAGE_GUIDE_SERVER_SRC "*.h" "*.pb.*" image_guide_server.cc)
FILE(GLOB IMAGE_GUIDE_CLIENT_SRC "*.h" "*.pb.*" image_guide_client.cc)
# LIST(REMOVE_ITEM IMAGE_GUIDE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/image_guide_client.cc)

SET(PROTOS_PATH ${PROJECT_SOURCE_DIR}/protos)
SET(PROTOS_SOURCE_FILE ${PROJECT_SOURCE_DIR}/protos/image_guide.proto)
SET(GRPC_CPP_PLUGIN_PATH /usr/local/bin/grpc_cpp_plugin)

SET(OPCV opencv_core opencv_highgui opencv_features2d opencv_flann opencv_imgproc opencv_imgcodecs)
SET(BOOST boost_filesystem boost_date_time boost_system)
SET(GRPC grpc++_unsecure grpc gpr protobuf)
SET(SYSTEM unwind glog pthread dl)

INCLUDE_DIRECTORIES(/usr/local/include ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib /usr/local/lib)
ADD_EXECUTABlE(image_guide_server ${IMAGE_GUIDE_SERVER_SRC})
TARGET_LINK_LIBRARIES(image_guide_server ${SYSTEM} ${GRPC} ${OPCV} helper ${BOOST})
# ADD_EXECUTABlE(image_guide_client ${IMAGE_GUIDE_CLIENT_SRC})
# TARGET_LINK_LIBRARIES(image_guide_client unwind grpc++_unsecure grpc gpr protobuf glog pthread dl ${OPCV} helper)

SET(PROTOS_FILE "grpc")  
ADD_CUSTOM_COMMAND(OUTPUT  ${PROTOS_FILE}  
    COMMAND echo "Removing old grpc intermediate files.."
    COMMAND rm -rf ${PROJECT_SOURCE_DIR}/src/image_guide/*.pb.*
    COMMAND echo "Generating ${PROTOS_FILE} file..."  
    COMMAND protoc -I ${PROTOS_PATH} --grpc_out=${PROJECT_SOURCE_DIR}/src/image_guide --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH}  ${PROTOS_SOURCE_FILE}
    COMMAND protoc -I ${PROTOS_PATH} --cpp_out=${PROJECT_SOURCE_DIR}/src/image_guide  ${PROTOS_SOURCE_FILE}
    COMMENT  "This is a test"  
    )  
ADD_CUSTOM_TARGET(GRPC ALL DEPENDS ${PROTOS_FILE})  
