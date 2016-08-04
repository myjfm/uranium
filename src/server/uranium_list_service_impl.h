#ifndef URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_
#define URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_

#include <memory>

#include "common/status.h"
#include "db/table_manager.h"
#include "network/cpp/uranium.grpc.pb.h"

namespace uranium {

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
                                 api::ListLPushResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->LPush(request->key().key(), request->values());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListLPop(grpc::ServerContext* context,
                                const api::ListLPopRequest* request,
                                api::ListLPopResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->LPop(request->key().key(), response->mutable_value());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListRPush(grpc::ServerContext* context,
                                 const api::ListRPushRequest* request,
                                 api::ListRPushResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->RPush(request->key().key(), request->values());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListRPop(grpc::ServerContext* context,
                                const api::ListRPopRequest* request,
                                api::ListRPopResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->RPop(request->key().key(), response->mutable_value());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListLPushX(grpc::ServerContext* context,
                                  const api::ListLPushXRequest* request,
                                 api::ListLPushXResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->LPushX(request->key().key(), request->values());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListRPushX(grpc::ServerContext* context,
                                  const api::ListRPushXRequest* request,
                                  api::ListRPushXResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->RPushX(request->key().key(), request->values());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListIndex(grpc::ServerContext* context,
                                 const api::ListIndexRequest* request,
                                 api::ListIndexResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Index(request->key().key(),
                            request->index(),
                            response->mutable_value());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListSet(grpc::ServerContext* context,
                               const api::ListSetRequest* request,
                               api::ListSetResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Set(request->key().key(),
                          request->index(),
                          request->value().value());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListRange(grpc::ServerContext* context,
                                 const api::ListRangeRequest* request,
                                 api::ListRangeResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->Range(request->key().key(),
                            request->start(),
                            request->end(),
                            response->mutable_values());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListLength(grpc::ServerContext* context,
                                  const api::ListLengthRequest* request,
                                  api::ListLengthResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      int64_t length = 0;
      auto s = table->Length(request->key().key(), &length);
      if (s.ok()) {
        response->set_length(length);
      }
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

  virtual grpc::Status ListRemoveAll(
      grpc::ServerContext* context,
      const api::ListRemoveAllRequest* request,
      api::ListRemoveAllResponse* response) override {
    auto table =
        table_manager_->GetListTable(request->table_name().name());
    if (!table.get()) {
      Status::TABLE_NOT_FOUND(response->mutable_result());
      return grpc::Status::OK;
    } else {
      auto s = table->RemoveAll(request->key().key());
      Status::RESULT(s, response->mutable_result());
      return grpc::Status::OK;
    }
  }

 private:
  std::shared_ptr<TableManager> table_manager_;
};

}  // namespace uranium

#endif  // URANIUM_SERVER_URANIUM_LIST_SERVICE_IMPL_H_

