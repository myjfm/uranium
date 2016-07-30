#ifndef URANIUM_SERVER_URANIUM_SCHEMALESS_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_SCHEMALESS_SERVICE_IMPL_H_

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class UraniumSchemalessServiceImpl
    : public api::UraniumSchemalessService::Service {
 public:
  UraniumSchemalessServiceImpl() {}
  virtual ~UraniumSchemalessServiceImpl() {}
  Status Init() { return Status::OK(); }

  virtual grpc::Status KVGet(grpc::ServerContext* context,
                             const api::KVGetRequest* request,
                             api::KVGetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status KVSet(grpc::ServerContext* context,
                             const api::KVSetRequest* request,
                             api::KVSetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status KVRemove(grpc::ServerContext* context,
                                const api::KVRemoveRequest* request,
                                api::KVRemoveResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListLPush(grpc::ServerContext* context,
                                 const api::ListLPushRequest* request,
                                 api::ListLPushResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListLPop(grpc::ServerContext* context,
                                const api::ListLPopRequest* request,
                                api::ListLPopResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListRPush(grpc::ServerContext* context,
                                 const api::ListRPushRequest* request,
                                 api::ListRPushResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListRPop(grpc::ServerContext* context,
                                const api::ListRPopRequest* request,
                                api::ListRPopResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListLPushX(grpc::ServerContext* context,
                                  const api::ListLPushXRequest* request,
                                 api::ListLPushXResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListRPushX(grpc::ServerContext* context,
                                  const api::ListRPushXRequest* request,
                                  api::ListRPushXResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListIndex(grpc::ServerContext* context,
                                 const api::ListIndexRequest* request,
                                 api::ListIndexResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListSet(grpc::ServerContext* context,
                               const api::ListSetRequest* request,
                               api::ListSetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListRange(grpc::ServerContext* context,
                                 const api::ListRangeRequest* request,
                                 api::ListRangeResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListLength(grpc::ServerContext* context,
                                  const api::ListLengthRequest* request,
                                  api::ListLengthResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status ListRemoveAll(
      grpc::ServerContext* context,
      const api::ListRemoveAllRequest* request,
      api::ListRemoveAllResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGet(grpc::ServerContext* context,
                               const api::HashGetRequest* request,
                               api::HashGetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAll(grpc::ServerContext* context,
                                  const api::HashGetAllRequest* request,
                                  api::HashGetAllResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAllFields(
      grpc::ServerContext* context,
      const api::HashGetAllFieldsRequest* request,
      api::HashGetAllFieldsResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashGetAllValues(
      grpc::ServerContext* context,
      const api::HashGetAllValuesRequest* request,
      api::HashGetAllValuesResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashLength(grpc::ServerContext* context,
                                  const api::HashLengthRequest* request,
                                  api::HashLengthResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashSet(grpc::ServerContext* context,
                               const api::HashSetRequest* request,
                               api::HashSetResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashSetX(grpc::ServerContext* context,
                                const api::HashSetXRequest* request,
                                api::HashSetXResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashRemove(grpc::ServerContext* context,
                                  const api::HashRemoveRequest* request,
                                  api::HashRemoveResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashExists(grpc::ServerContext* context,
                                  const api::HashExistsRequest* request,
                                  api::HashExistsResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status HashRemoveAll(
      grpc::ServerContext* context,
      const api::HashRemoveAllRequest* request,
      api::HashRemoveAllResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetAdd(grpc::ServerContext* context,
                              const api::SetAddRequest* request,
                              api::SetAddResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetLength(grpc::ServerContext* context,
                                 const api::SetLengthRequest* request,
                                 api::SetLengthResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetIsMember(
      grpc::ServerContext* context,
      const api::SetIsMemberRequest* request,
      api::SetIsMemberResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetGetAll(grpc::ServerContext* context,
                                 const api::SetGetAllRequest* request,
                                 api::SetGetAllResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetRemove(grpc::ServerContext* context,
                                 const api::SetRemoveRequest* request,
                                 api::SetRemoveResponse* response) override {
    return grpc::Status::OK;
  }

  virtual grpc::Status SetRemoveAll(
      grpc::ServerContext* context,
      const api::SetRemoveAllRequest* request,
      api::SetRemoveAllResponse* response) override {
    return grpc::Status::OK;
  }
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUMLESS_SERVICE_IMPL_H_

