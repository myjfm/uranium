// Copyright (c) 2016, myjfm(mwxjmmyjfm@gmail.com).  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// Copyright (c) 2013, Facebook, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.
// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef URANIUM_COMMON_STATUS_H_
#define URANIUM_COMMON_STATUS_H_

#include <string>
#include "slice.h"
#include <rocksdb/status.h>

namespace uranium {

class Status final {
 public:
  Status() : code_(kOk), state_(nullptr) { }
  ~Status() { delete[] state_; }

  Status(const rocksdb::Status& s) {
    new (this) Status(static_cast<Code>(static_cast<int>(s.code())));
  }

  Status(const Status& s);
  void operator=(const Status& s);

  static Status OK() { return Status(); }

  static Status NotFound(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kNotFound, msg, msg2);
  }

  // Fast path for not found without malloc;
  static Status NotFound() {
    return Status(kNotFound);
  }
  static Status Corruption(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kCorruption, msg, msg2);
  }
  static Status NotSupported(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kNotSupported, msg, msg2);
  }
  static Status InvalidArgument(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kInvalidArgument, msg, msg2);
  }
  static Status IOError(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kIOError, msg, msg2);
  }
  static Status MergeInProgress(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kMergeInProgress, msg, msg2);
  }
  static Status Incomplete(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kIncomplete, msg, msg2);
  }
  static Status ShutdownInProgress() {
    return Status(kShutdownInProgress);
  }
  static Status ShutdownInProgress(const Slice& msg,
                                   const Slice& msg2 = Slice()) {
    return Status(kShutdownInProgress, msg, msg2);
  }
  static Status TimedOut() {
    return Status(kTimedOut);
  }
  static Status TimedOut(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kTimedOut, msg, msg2);
  }
  static Status Aborted() {
    return Status(kAborted);
  }
  static Status Aborted(const Slice& msg, const Slice& msg2 = Slice()) {
    return Status(kAborted, msg, msg2);
  }

  // Returns true iff the status indicates success.
  bool ok() const { return code() == kOk; }

  // Returns true iff the status indicates a NotFound error.
  bool IsNotFound() const { return code() == kNotFound; }

  // Returns true iff the status indicates a Corruption error.
  bool IsCorruption() const { return code() == kCorruption; }

  // Returns true iff the status indicates a NotSupported error.
  bool IsNotSupported() const { return code() == kNotSupported; }

  // Returns true iff the status indicates an InvalidArgument error.
  bool IsInvalidArgument() const { return code() == kInvalidArgument; }

  // Returns true iff the status indicates an IOError.
  bool IsIOError() const { return code() == kIOError; }

  // Returns true iff the status indicates an MergeInProgress.
  bool IsMergeInProgress() const { return code() == kMergeInProgress; }

  // Returns true iff the status indicates Incomplete
  bool IsIncomplete() const { return code() == kIncomplete; }

  // Returns true iff the status indicates Shutdown In progress
  bool IsShutdownInProgress() const { return code() == kShutdownInProgress; }

  bool IsTimedOut() const { return code() == kTimedOut; }

  bool IsAborted() const { return code() == kAborted; }

  // Return a string representation of this status suitable for printing.
  // Returns the string "OK" for success.
  std::string ToString() const {
    char tmp[30];
    const char* type;
    switch (code_) {
      case kOk:
        return "OK";
      case kNotFound:
        type = "NotFound: ";
        break;
      case kCorruption:
        type = "Corruption: ";
        break;
      case kNotSupported:
        type = "Not implemented: ";
        break;
      case kInvalidArgument:
        type = "Invalid argument: ";
        break;
      case kIOError:
        type = "IO error: ";
        break;
      case kMergeInProgress:
        type = "Merge in progress: ";
        break;
      case kIncomplete:
        type = "Result incomplete: ";
        break;
      case kShutdownInProgress:
        type = "Shutdown in progress: ";
        break;
      case kTimedOut:
        type = "Operation timed out: ";
        break;
      case kAborted:
        type = "Operation aborted: ";
        break;
      default:
        snprintf(tmp, sizeof(tmp), "Unknown code(%d): ",
            static_cast<int>(code()));
        type = tmp;
        break;
    }
    std::string result(type);
    if (state_ != nullptr) {
      uint32_t length;
      memcpy(&length, state_, sizeof(length));
      result.append(state_ + 4, length);
    }
    return result;
  }

  enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5,
    kMergeInProgress = 6,
    kIncomplete = 7,
    kShutdownInProgress = 8,
    kTimedOut = 9,
    kAborted = 10
  };

  Code code() const {
    return code_;
  }
 private:
  // A nullptr state_ (which is always the case for OK) means the message
  // is empty.
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4..]  == message
  Code code_;
  const char* state_;

  explicit Status(Code _code) : code_(_code), state_(nullptr) {}
  Status(Code _code, const Slice& msg, const Slice& msg2) {
    assert(code_ != kOk);
    const uint32_t len1 = static_cast<uint32_t>(msg.size());
    const uint32_t len2 = static_cast<uint32_t>(msg2.size());
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);
    char* result = new char[size + 4];
    memcpy(result, &size, sizeof(size));
    memcpy(result + 4, msg.data(), len1);
    if (len2) {
      result[4 + len1] = ':';
      result[5 + len1] = ' ';
      memcpy(result + 6 + len1, msg2.data(), len2);
    }
    state_ = result;
  }

  static const char* CopyState(const char* s) {
    uint32_t size;
    memcpy(&size, s, sizeof(size));
    char* result = new char[size + 4];
    memcpy(result, s, size + 4);
    return result;
  }
};

inline Status::Status(const Status& s) {
  code_ = s.code_;
  state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
}
inline void Status::operator=(const Status& s) {
  // The following condition catches both aliasing (when this == &s),
  // and the common case where both s and *this are ok.
  code_ = s.code_;
  if (state_ != s.state_) {
    delete[] state_;
    state_ = (s.state_ == nullptr) ? nullptr : CopyState(s.state_);
  }
}

}  // namespace uranium

#endif  // URANIUM_COMMON_STATUS_H_

