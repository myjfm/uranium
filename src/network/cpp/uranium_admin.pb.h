// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: uranium_admin.proto

#ifndef PROTOBUF_uranium_5fadmin_2eproto__INCLUDED
#define PROTOBUF_uranium_5fadmin_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
#include "common.pb.h"
// @@protoc_insertion_point(includes)

namespace uranium {
namespace admin {

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_uranium_5fadmin_2eproto();
void protobuf_AssignDesc_uranium_5fadmin_2eproto();
void protobuf_ShutdownFile_uranium_5fadmin_2eproto();

class SchemaTableOptions;
class SchemalessTableOptions;
class TableOptions;

enum StorageType {
  FLAT = 0,
  COLUMN_SEPARATED = 1,
  COLUMN_GROUP = 2,
  StorageType_INT_MIN_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32min,
  StorageType_INT_MAX_SENTINEL_DO_NOT_USE_ = ::google::protobuf::kint32max
};
bool StorageType_IsValid(int value);
const StorageType StorageType_MIN = FLAT;
const StorageType StorageType_MAX = COLUMN_GROUP;
const int StorageType_ARRAYSIZE = StorageType_MAX + 1;

const ::google::protobuf::EnumDescriptor* StorageType_descriptor();
inline const ::std::string& StorageType_Name(StorageType value) {
  return ::google::protobuf::internal::NameOfEnum(
    StorageType_descriptor(), value);
}
inline bool StorageType_Parse(
    const ::std::string& name, StorageType* value) {
  return ::google::protobuf::internal::ParseNamedEnum<StorageType>(
    StorageType_descriptor(), name, value);
}
// ===================================================================

class TableOptions : public ::google::protobuf::Message {
 public:
  TableOptions();
  virtual ~TableOptions();

  TableOptions(const TableOptions& from);

  inline TableOptions& operator=(const TableOptions& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const TableOptions& default_instance();

  enum OptionsCase {
    kSchemaTableOptions = 2,
    kSchemalessTableOptions = 3,
    OPTIONS_NOT_SET = 0,
  };

  void Swap(TableOptions* other);

  // implements Message ----------------------------------------------

  inline TableOptions* New() const { return New(NULL); }

  TableOptions* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const TableOptions& from);
  void MergeFrom(const TableOptions& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(TableOptions* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .uranium.common.TableName table_name = 1;
  bool has_table_name() const;
  void clear_table_name();
  static const int kTableNameFieldNumber = 1;
  const ::uranium::common::TableName& table_name() const;
  ::uranium::common::TableName* mutable_table_name();
  ::uranium::common::TableName* release_table_name();
  void set_allocated_table_name(::uranium::common::TableName* table_name);

  // optional .uranium.admin.SchemaTableOptions schema_table_options = 2;
  bool has_schema_table_options() const;
  void clear_schema_table_options();
  static const int kSchemaTableOptionsFieldNumber = 2;
  const ::uranium::admin::SchemaTableOptions& schema_table_options() const;
  ::uranium::admin::SchemaTableOptions* mutable_schema_table_options();
  ::uranium::admin::SchemaTableOptions* release_schema_table_options();
  void set_allocated_schema_table_options(::uranium::admin::SchemaTableOptions* schema_table_options);

  // optional .uranium.admin.SchemalessTableOptions schemaless_table_options = 3;
  bool has_schemaless_table_options() const;
  void clear_schemaless_table_options();
  static const int kSchemalessTableOptionsFieldNumber = 3;
  const ::uranium::admin::SchemalessTableOptions& schemaless_table_options() const;
  ::uranium::admin::SchemalessTableOptions* mutable_schemaless_table_options();
  ::uranium::admin::SchemalessTableOptions* release_schemaless_table_options();
  void set_allocated_schemaless_table_options(::uranium::admin::SchemalessTableOptions* schemaless_table_options);

  OptionsCase options_case() const;
  // @@protoc_insertion_point(class_scope:uranium.admin.TableOptions)
 private:
  inline void set_has_schema_table_options();
  inline void set_has_schemaless_table_options();

  inline bool has_options() const;
  void clear_options();
  inline void clear_has_options();

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::uranium::common::TableName* table_name_;
  union OptionsUnion {
    OptionsUnion() {}
    ::uranium::admin::SchemaTableOptions* schema_table_options_;
    ::uranium::admin::SchemalessTableOptions* schemaless_table_options_;
  } options_;
  mutable int _cached_size_;
  ::google::protobuf::uint32 _oneof_case_[1];

  friend void  protobuf_AddDesc_uranium_5fadmin_2eproto();
  friend void protobuf_AssignDesc_uranium_5fadmin_2eproto();
  friend void protobuf_ShutdownFile_uranium_5fadmin_2eproto();

  void InitAsDefaultInstance();
  static TableOptions* default_instance_;
};
// -------------------------------------------------------------------

class SchemaTableOptions : public ::google::protobuf::Message {
 public:
  SchemaTableOptions();
  virtual ~SchemaTableOptions();

  SchemaTableOptions(const SchemaTableOptions& from);

  inline SchemaTableOptions& operator=(const SchemaTableOptions& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SchemaTableOptions& default_instance();

  void Swap(SchemaTableOptions* other);

  // implements Message ----------------------------------------------

  inline SchemaTableOptions* New() const { return New(NULL); }

  SchemaTableOptions* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SchemaTableOptions& from);
  void MergeFrom(const SchemaTableOptions& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SchemaTableOptions* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .uranium.admin.StorageType type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::uranium::admin::StorageType type() const;
  void set_type(::uranium::admin::StorageType value);

  // repeated .uranium.common.ColumnDefination columns = 3;
  int columns_size() const;
  void clear_columns();
  static const int kColumnsFieldNumber = 3;
  const ::uranium::common::ColumnDefination& columns(int index) const;
  ::uranium::common::ColumnDefination* mutable_columns(int index);
  ::uranium::common::ColumnDefination* add_columns();
  ::google::protobuf::RepeatedPtrField< ::uranium::common::ColumnDefination >*
      mutable_columns();
  const ::google::protobuf::RepeatedPtrField< ::uranium::common::ColumnDefination >&
      columns() const;

  // @@protoc_insertion_point(class_scope:uranium.admin.SchemaTableOptions)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  ::google::protobuf::RepeatedPtrField< ::uranium::common::ColumnDefination > columns_;
  int type_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_uranium_5fadmin_2eproto();
  friend void protobuf_AssignDesc_uranium_5fadmin_2eproto();
  friend void protobuf_ShutdownFile_uranium_5fadmin_2eproto();

  void InitAsDefaultInstance();
  static SchemaTableOptions* default_instance_;
};
// -------------------------------------------------------------------

class SchemalessTableOptions : public ::google::protobuf::Message {
 public:
  SchemalessTableOptions();
  virtual ~SchemalessTableOptions();

  SchemalessTableOptions(const SchemalessTableOptions& from);

  inline SchemalessTableOptions& operator=(const SchemalessTableOptions& from) {
    CopyFrom(from);
    return *this;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const SchemalessTableOptions& default_instance();

  void Swap(SchemalessTableOptions* other);

  // implements Message ----------------------------------------------

  inline SchemalessTableOptions* New() const { return New(NULL); }

  SchemalessTableOptions* New(::google::protobuf::Arena* arena) const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SchemalessTableOptions& from);
  void MergeFrom(const SchemalessTableOptions& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SchemalessTableOptions* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _internal_metadata_.arena();
  }
  inline void* MaybeArenaPtr() const {
    return _internal_metadata_.raw_arena_ptr();
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional .uranium.admin.StorageType type = 1;
  void clear_type();
  static const int kTypeFieldNumber = 1;
  ::uranium::admin::StorageType type() const;
  void set_type(::uranium::admin::StorageType value);

  // @@protoc_insertion_point(class_scope:uranium.admin.SchemalessTableOptions)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  bool _is_default_instance_;
  int type_;
  mutable int _cached_size_;
  friend void  protobuf_AddDesc_uranium_5fadmin_2eproto();
  friend void protobuf_AssignDesc_uranium_5fadmin_2eproto();
  friend void protobuf_ShutdownFile_uranium_5fadmin_2eproto();

  void InitAsDefaultInstance();
  static SchemalessTableOptions* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// TableOptions

// optional .uranium.common.TableName table_name = 1;
inline bool TableOptions::has_table_name() const {
  return !_is_default_instance_ && table_name_ != NULL;
}
inline void TableOptions::clear_table_name() {
  if (GetArenaNoVirtual() == NULL && table_name_ != NULL) delete table_name_;
  table_name_ = NULL;
}
inline const ::uranium::common::TableName& TableOptions::table_name() const {
  // @@protoc_insertion_point(field_get:uranium.admin.TableOptions.table_name)
  return table_name_ != NULL ? *table_name_ : *default_instance_->table_name_;
}
inline ::uranium::common::TableName* TableOptions::mutable_table_name() {
  
  if (table_name_ == NULL) {
    table_name_ = new ::uranium::common::TableName;
  }
  // @@protoc_insertion_point(field_mutable:uranium.admin.TableOptions.table_name)
  return table_name_;
}
inline ::uranium::common::TableName* TableOptions::release_table_name() {
  // @@protoc_insertion_point(field_release:uranium.admin.TableOptions.table_name)
  
  ::uranium::common::TableName* temp = table_name_;
  table_name_ = NULL;
  return temp;
}
inline void TableOptions::set_allocated_table_name(::uranium::common::TableName* table_name) {
  delete table_name_;
  table_name_ = table_name;
  if (table_name) {
    
  } else {
    
  }
  // @@protoc_insertion_point(field_set_allocated:uranium.admin.TableOptions.table_name)
}

// optional .uranium.admin.SchemaTableOptions schema_table_options = 2;
inline bool TableOptions::has_schema_table_options() const {
  return options_case() == kSchemaTableOptions;
}
inline void TableOptions::set_has_schema_table_options() {
  _oneof_case_[0] = kSchemaTableOptions;
}
inline void TableOptions::clear_schema_table_options() {
  if (has_schema_table_options()) {
    delete options_.schema_table_options_;
    clear_has_options();
  }
}
inline  const ::uranium::admin::SchemaTableOptions& TableOptions::schema_table_options() const {
  // @@protoc_insertion_point(field_get:uranium.admin.TableOptions.schema_table_options)
  return has_schema_table_options()
      ? *options_.schema_table_options_
      : ::uranium::admin::SchemaTableOptions::default_instance();
}
inline ::uranium::admin::SchemaTableOptions* TableOptions::mutable_schema_table_options() {
  if (!has_schema_table_options()) {
    clear_options();
    set_has_schema_table_options();
    options_.schema_table_options_ = new ::uranium::admin::SchemaTableOptions;
  }
  // @@protoc_insertion_point(field_mutable:uranium.admin.TableOptions.schema_table_options)
  return options_.schema_table_options_;
}
inline ::uranium::admin::SchemaTableOptions* TableOptions::release_schema_table_options() {
  // @@protoc_insertion_point(field_release:uranium.admin.TableOptions.schema_table_options)
  if (has_schema_table_options()) {
    clear_has_options();
    ::uranium::admin::SchemaTableOptions* temp = options_.schema_table_options_;
    options_.schema_table_options_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void TableOptions::set_allocated_schema_table_options(::uranium::admin::SchemaTableOptions* schema_table_options) {
  clear_options();
  if (schema_table_options) {
    set_has_schema_table_options();
    options_.schema_table_options_ = schema_table_options;
  }
  // @@protoc_insertion_point(field_set_allocated:uranium.admin.TableOptions.schema_table_options)
}

// optional .uranium.admin.SchemalessTableOptions schemaless_table_options = 3;
inline bool TableOptions::has_schemaless_table_options() const {
  return options_case() == kSchemalessTableOptions;
}
inline void TableOptions::set_has_schemaless_table_options() {
  _oneof_case_[0] = kSchemalessTableOptions;
}
inline void TableOptions::clear_schemaless_table_options() {
  if (has_schemaless_table_options()) {
    delete options_.schemaless_table_options_;
    clear_has_options();
  }
}
inline  const ::uranium::admin::SchemalessTableOptions& TableOptions::schemaless_table_options() const {
  // @@protoc_insertion_point(field_get:uranium.admin.TableOptions.schemaless_table_options)
  return has_schemaless_table_options()
      ? *options_.schemaless_table_options_
      : ::uranium::admin::SchemalessTableOptions::default_instance();
}
inline ::uranium::admin::SchemalessTableOptions* TableOptions::mutable_schemaless_table_options() {
  if (!has_schemaless_table_options()) {
    clear_options();
    set_has_schemaless_table_options();
    options_.schemaless_table_options_ = new ::uranium::admin::SchemalessTableOptions;
  }
  // @@protoc_insertion_point(field_mutable:uranium.admin.TableOptions.schemaless_table_options)
  return options_.schemaless_table_options_;
}
inline ::uranium::admin::SchemalessTableOptions* TableOptions::release_schemaless_table_options() {
  // @@protoc_insertion_point(field_release:uranium.admin.TableOptions.schemaless_table_options)
  if (has_schemaless_table_options()) {
    clear_has_options();
    ::uranium::admin::SchemalessTableOptions* temp = options_.schemaless_table_options_;
    options_.schemaless_table_options_ = NULL;
    return temp;
  } else {
    return NULL;
  }
}
inline void TableOptions::set_allocated_schemaless_table_options(::uranium::admin::SchemalessTableOptions* schemaless_table_options) {
  clear_options();
  if (schemaless_table_options) {
    set_has_schemaless_table_options();
    options_.schemaless_table_options_ = schemaless_table_options;
  }
  // @@protoc_insertion_point(field_set_allocated:uranium.admin.TableOptions.schemaless_table_options)
}

inline bool TableOptions::has_options() const {
  return options_case() != OPTIONS_NOT_SET;
}
inline void TableOptions::clear_has_options() {
  _oneof_case_[0] = OPTIONS_NOT_SET;
}
inline TableOptions::OptionsCase TableOptions::options_case() const {
  return TableOptions::OptionsCase(_oneof_case_[0]);
}
// -------------------------------------------------------------------

// SchemaTableOptions

// optional .uranium.admin.StorageType type = 1;
inline void SchemaTableOptions::clear_type() {
  type_ = 0;
}
inline ::uranium::admin::StorageType SchemaTableOptions::type() const {
  // @@protoc_insertion_point(field_get:uranium.admin.SchemaTableOptions.type)
  return static_cast< ::uranium::admin::StorageType >(type_);
}
inline void SchemaTableOptions::set_type(::uranium::admin::StorageType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:uranium.admin.SchemaTableOptions.type)
}

// repeated .uranium.common.ColumnDefination columns = 3;
inline int SchemaTableOptions::columns_size() const {
  return columns_.size();
}
inline void SchemaTableOptions::clear_columns() {
  columns_.Clear();
}
inline const ::uranium::common::ColumnDefination& SchemaTableOptions::columns(int index) const {
  // @@protoc_insertion_point(field_get:uranium.admin.SchemaTableOptions.columns)
  return columns_.Get(index);
}
inline ::uranium::common::ColumnDefination* SchemaTableOptions::mutable_columns(int index) {
  // @@protoc_insertion_point(field_mutable:uranium.admin.SchemaTableOptions.columns)
  return columns_.Mutable(index);
}
inline ::uranium::common::ColumnDefination* SchemaTableOptions::add_columns() {
  // @@protoc_insertion_point(field_add:uranium.admin.SchemaTableOptions.columns)
  return columns_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::uranium::common::ColumnDefination >*
SchemaTableOptions::mutable_columns() {
  // @@protoc_insertion_point(field_mutable_list:uranium.admin.SchemaTableOptions.columns)
  return &columns_;
}
inline const ::google::protobuf::RepeatedPtrField< ::uranium::common::ColumnDefination >&
SchemaTableOptions::columns() const {
  // @@protoc_insertion_point(field_list:uranium.admin.SchemaTableOptions.columns)
  return columns_;
}

// -------------------------------------------------------------------

// SchemalessTableOptions

// optional .uranium.admin.StorageType type = 1;
inline void SchemalessTableOptions::clear_type() {
  type_ = 0;
}
inline ::uranium::admin::StorageType SchemalessTableOptions::type() const {
  // @@protoc_insertion_point(field_get:uranium.admin.SchemalessTableOptions.type)
  return static_cast< ::uranium::admin::StorageType >(type_);
}
inline void SchemalessTableOptions::set_type(::uranium::admin::StorageType value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:uranium.admin.SchemalessTableOptions.type)
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace admin
}  // namespace uranium

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::uranium::admin::StorageType> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::uranium::admin::StorageType>() {
  return ::uranium::admin::StorageType_descriptor();
}

}  // namespace protobuf
}  // namespace google
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_uranium_5fadmin_2eproto__INCLUDED
