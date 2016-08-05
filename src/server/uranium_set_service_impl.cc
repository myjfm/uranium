#include "uranium_set_service_impl.h"
#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

grpc::Status UraniumSetServiceImpl::SetAdd(grpc::ServerContext* context,
                                           const api::SetAddRequest* request,
                                           api::SetAddResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetLength(
    grpc::ServerContext* context,
    const api::SetLengthRequest* request,
    api::SetLengthResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetIsMember(
    grpc::ServerContext* context,
    const api::SetIsMemberRequest* request,
    api::SetIsMemberResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetGetAll(
    grpc::ServerContext* context,
    const api::SetGetAllRequest* request,
    api::SetGetAllResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetRemove(
    grpc::ServerContext* context,
    const api::SetRemoveRequest* request,
    api::SetRemoveResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumSetServiceImpl::SetRemoveAll(
    grpc::ServerContext* context,
    const api::SetRemoveAllRequest* request,
    api::SetRemoveAllResponse* response) {
  return grpc::Status::OK;
}

}  // namespace uranium
