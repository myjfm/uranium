// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
//
#ifndef URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

class TableManager;

class UraniumListServiceImpl
    : public api::UraniumListService::Service {
 public:
  UraniumListServiceImpl() {}
  virtual ~UraniumListServiceImpl() {}

  UraniumListServiceImpl(const UraniumListServiceImpl&) = delete;
  UraniumListServiceImpl& operator=(UraniumListServiceImpl&) = delete;

  Status Init(std::shared_ptr<TableManager> table_manager) {
    assert(table_manager.get());
    table_manager_ = table_manager;
    return Status::OK();
  }

  virtual grpc::Status ListLPush(grpc::ServerContext* context,
                                 const api::ListLPushRequest* request,
                                 api::ListLPushResponse* response) override;

  virtual grpc::Status ListLPop(grpc::ServerContext* context,
                                const api::ListLPopRequest* request,
                                api::ListLPopResponse* response) override;

  virtual grpc::Status ListRPush(grpc::ServerContext* context,
                                 const api::ListRPushRequest* request,
                                 api::ListRPushResponse* response) override;

  virtual grpc::Status ListRPop(grpc::ServerContext* context,
                                const api::ListRPopRequest* request,
                                api::ListRPopResponse* response) override;

  virtual grpc::Status ListLPushX(grpc::ServerContext* context,
                                  const api::ListLPushXRequest* request,
                                 api::ListLPushXResponse* response) override;

  virtual grpc::Status ListRPushX(grpc::ServerContext* context,
                                  const api::ListRPushXRequest* request,
                                  api::ListRPushXResponse* response) override;

  virtual grpc::Status ListIndex(grpc::ServerContext* context,
                                 const api::ListIndexRequest* request,
                                 api::ListIndexResponse* response) override;

  virtual grpc::Status ListSet(grpc::ServerContext* context,
                               const api::ListSetRequest* request,
                               api::ListSetResponse* response) override;

  virtual grpc::Status ListRange(grpc::ServerContext* context,
                                 const api::ListRangeRequest* request,
                                 api::ListRangeResponse* response) override;

  virtual grpc::Status ListLength(grpc::ServerContext* context,
                                  const api::ListLengthRequest* request,
                                  api::ListLengthResponse* response) override;

  virtual grpc::Status ListRemoveAll(
      grpc::ServerContext* context,
      const api::ListRemoveAllRequest* request,
      api::ListRemoveAllResponse* response) override;

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_

