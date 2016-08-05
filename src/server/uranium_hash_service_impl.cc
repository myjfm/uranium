#include "uranium_hash_service_impl.h"

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.pb.h"

namespace uranium {

grpc::Status UraniumHashServiceImpl::HashGet(grpc::ServerContext* context,
                                             const api::HashGetRequest* request,
                                             api::HashGetResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashGetAll(
    grpc::ServerContext* context,
    const api::HashGetAllRequest* request,
    api::HashGetAllResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashGetAllFields(
    grpc::ServerContext* context,
    const api::HashGetAllFieldsRequest* request,
    api::HashGetAllFieldsResponse* response) {
  return grpc::Status::OK;
  }

grpc::Status UraniumHashServiceImpl::HashGetAllValues(
    grpc::ServerContext* context,
    const api::HashGetAllValuesRequest* request,
    api::HashGetAllValuesResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashLength(
    grpc::ServerContext* context,
    const api::HashLengthRequest* request,
    api::HashLengthResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashSet(
    grpc::ServerContext* context,
    const api::HashSetRequest* request,
    api::HashSetResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashSetX(
    grpc::ServerContext* context,
    const api::HashSetXRequest* request,
    api::HashSetXResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashRemove(
    grpc::ServerContext* context,
    const api::HashRemoveRequest* request,
    api::HashRemoveResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashExists(
    grpc::ServerContext* context,
    const api::HashExistsRequest* request,
    api::HashExistsResponse* response) {
  return grpc::Status::OK;
}

grpc::Status UraniumHashServiceImpl::HashRemoveAll(
    grpc::ServerContext* context,
    const api::HashRemoveAllRequest* request,
    api::HashRemoveAllResponse* response) {
  return grpc::Status::OK;
}

}  // namespace uranium
