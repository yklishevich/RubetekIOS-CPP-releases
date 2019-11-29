/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef video_recorder_TYPES_H
#define video_recorder_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>



namespace rubetek { namespace tag { namespace videorecorder {

struct COMMAND {
  enum type {
    START_RECORD_CAMERA = 800,
    START_RECORD_CAMERA_RESPONSE = 801,
    STOP_RECORD_CAMERA = 802,
    STOP_RECORD_CAMERA_RESPONSE = 803,
    GET_ALL_RECORDED_CAMERA = 804,
    GET_ALL_RECORDED_CAMERA_RESPONSE = 805,
    MAKE_STREAM_SNAPSHOT = 806,
    MAKE_STREAM_SNAPSHOT_RESPONSE = 807
  };
};

extern const std::map<int, const char*> _COMMAND_VALUES_TO_NAMES;

typedef struct _StartRecordCamera__isset {
  _StartRecordCamera__isset() : uuid(false), name(false), id(false), source_url(false), token_id(false), record_mode(false), login(false), password(false), script_mode(false) {}
  bool uuid;
  bool name;
  bool id;
  bool source_url;
  bool token_id;
  bool record_mode;
  bool login;
  bool password;
  bool script_mode;
} _StartRecordCamera__isset;

class StartRecordCamera {
 public:

  static const char* ascii_fingerprint; // = "DFFD290DFC35F6F1BE0790E5EC0C6B55";
  static const uint8_t binary_fingerprint[16]; // = {0xDF,0xFD,0x29,0x0D,0xFC,0x35,0xF6,0xF1,0xBE,0x07,0x90,0xE5,0xEC,0x0C,0x6B,0x55};

  StartRecordCamera() : uuid(), name(), id(), source_url(), token_id(), record_mode(), login(), password(), script_mode() {
  }

  virtual ~StartRecordCamera() throw() {}

  std::string uuid;
  std::string name;
  std::string id;
  std::string source_url;
  std::string token_id;
  std::string record_mode;
  std::string login;
  std::string password;
  std::string script_mode;

  _StartRecordCamera__isset __isset;

  void __set_uuid(const std::string& val) {
    uuid = val;
  }

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_id(const std::string& val) {
    id = val;
  }

  void __set_source_url(const std::string& val) {
    source_url = val;
  }

  void __set_token_id(const std::string& val) {
    token_id = val;
  }

  void __set_record_mode(const std::string& val) {
    record_mode = val;
  }

  void __set_login(const std::string& val) {
    login = val;
  }

  void __set_password(const std::string& val) {
    password = val;
  }

  void __set_script_mode(const std::string& val) {
    script_mode = val;
  }

  bool operator == (const StartRecordCamera & rhs) const
  {
    if (!(uuid == rhs.uuid))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(id == rhs.id))
      return false;
    if (!(source_url == rhs.source_url))
      return false;
    if (!(token_id == rhs.token_id))
      return false;
    if (!(record_mode == rhs.record_mode))
      return false;
    if (!(login == rhs.login))
      return false;
    if (!(password == rhs.password))
      return false;
    if (!(script_mode == rhs.script_mode))
      return false;
    return true;
  }
  bool operator != (const StartRecordCamera &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StartRecordCamera & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StartRecordCamera &a, StartRecordCamera &b);

typedef struct _StartRecordCameraResponse__isset {
  _StartRecordCameraResponse__isset() : status(false) {}
  bool status;
} _StartRecordCameraResponse__isset;

class StartRecordCameraResponse {
 public:

  static const char* ascii_fingerprint; // = "5892306F7B861249AE8E27C8ED619593";
  static const uint8_t binary_fingerprint[16]; // = {0x58,0x92,0x30,0x6F,0x7B,0x86,0x12,0x49,0xAE,0x8E,0x27,0xC8,0xED,0x61,0x95,0x93};

  StartRecordCameraResponse() : status(0) {
  }

  virtual ~StartRecordCameraResponse() throw() {}

  bool status;

  _StartRecordCameraResponse__isset __isset;

  void __set_status(const bool val) {
    status = val;
  }

  bool operator == (const StartRecordCameraResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const StartRecordCameraResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StartRecordCameraResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StartRecordCameraResponse &a, StartRecordCameraResponse &b);

typedef struct _StopRecordCamera__isset {
  _StopRecordCamera__isset() : uuid(false) {}
  bool uuid;
} _StopRecordCamera__isset;

class StopRecordCamera {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  StopRecordCamera() : uuid() {
  }

  virtual ~StopRecordCamera() throw() {}

  std::string uuid;

  _StopRecordCamera__isset __isset;

  void __set_uuid(const std::string& val) {
    uuid = val;
  }

  bool operator == (const StopRecordCamera & rhs) const
  {
    if (!(uuid == rhs.uuid))
      return false;
    return true;
  }
  bool operator != (const StopRecordCamera &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StopRecordCamera & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StopRecordCamera &a, StopRecordCamera &b);

typedef struct _StopRecordCameraResponse__isset {
  _StopRecordCameraResponse__isset() : status(false), error(false) {}
  bool status;
  bool error;
} _StopRecordCameraResponse__isset;

class StopRecordCameraResponse {
 public:

  static const char* ascii_fingerprint; // = "1767FFB0CB3D9275BC64B198AB3B8A8B";
  static const uint8_t binary_fingerprint[16]; // = {0x17,0x67,0xFF,0xB0,0xCB,0x3D,0x92,0x75,0xBC,0x64,0xB1,0x98,0xAB,0x3B,0x8A,0x8B};

  StopRecordCameraResponse() : status(0), error() {
  }

  virtual ~StopRecordCameraResponse() throw() {}

  bool status;
  std::string error;

  _StopRecordCameraResponse__isset __isset;

  void __set_status(const bool val) {
    status = val;
  }

  void __set_error(const std::string& val) {
    error = val;
  }

  bool operator == (const StopRecordCameraResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(error == rhs.error))
      return false;
    return true;
  }
  bool operator != (const StopRecordCameraResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const StopRecordCameraResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(StopRecordCameraResponse &a, StopRecordCameraResponse &b);

typedef struct _MakeStreamSnapshot__isset {
  _MakeStreamSnapshot__isset() : rtsp_uri(false), camera_id(false), context(false) {}
  bool rtsp_uri;
  bool camera_id;
  bool context;
} _MakeStreamSnapshot__isset;

class MakeStreamSnapshot {
 public:

  static const char* ascii_fingerprint; // = "AB879940BD15B6B25691265F7384B271";
  static const uint8_t binary_fingerprint[16]; // = {0xAB,0x87,0x99,0x40,0xBD,0x15,0xB6,0xB2,0x56,0x91,0x26,0x5F,0x73,0x84,0xB2,0x71};

  MakeStreamSnapshot() : rtsp_uri(), camera_id(), context() {
  }

  virtual ~MakeStreamSnapshot() throw() {}

  std::string rtsp_uri;
  std::string camera_id;
  std::string context;

  _MakeStreamSnapshot__isset __isset;

  void __set_rtsp_uri(const std::string& val) {
    rtsp_uri = val;
  }

  void __set_camera_id(const std::string& val) {
    camera_id = val;
  }

  void __set_context(const std::string& val) {
    context = val;
  }

  bool operator == (const MakeStreamSnapshot & rhs) const
  {
    if (!(rtsp_uri == rhs.rtsp_uri))
      return false;
    if (!(camera_id == rhs.camera_id))
      return false;
    if (!(context == rhs.context))
      return false;
    return true;
  }
  bool operator != (const MakeStreamSnapshot &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MakeStreamSnapshot & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MakeStreamSnapshot &a, MakeStreamSnapshot &b);

typedef struct _MakeStreamSnapshotResponse__isset {
  _MakeStreamSnapshotResponse__isset() : status(false), content(false), camera_id(false), context(false) {}
  bool status;
  bool content;
  bool camera_id;
  bool context;
} _MakeStreamSnapshotResponse__isset;

class MakeStreamSnapshotResponse {
 public:

  static const char* ascii_fingerprint; // = "4D574F8242FF72AB225D42F31BAD56F4";
  static const uint8_t binary_fingerprint[16]; // = {0x4D,0x57,0x4F,0x82,0x42,0xFF,0x72,0xAB,0x22,0x5D,0x42,0xF3,0x1B,0xAD,0x56,0xF4};

  MakeStreamSnapshotResponse() : status(0), camera_id(), context() {
  }

  virtual ~MakeStreamSnapshotResponse() throw() {}

  bool status;
  std::vector<int8_t>  content;
  std::string camera_id;
  std::string context;

  _MakeStreamSnapshotResponse__isset __isset;

  void __set_status(const bool val) {
    status = val;
  }

  void __set_content(const std::vector<int8_t> & val) {
    content = val;
  }

  void __set_camera_id(const std::string& val) {
    camera_id = val;
  }

  void __set_context(const std::string& val) {
    context = val;
  }

  bool operator == (const MakeStreamSnapshotResponse & rhs) const
  {
    if (!(status == rhs.status))
      return false;
    if (!(content == rhs.content))
      return false;
    if (!(camera_id == rhs.camera_id))
      return false;
    if (!(context == rhs.context))
      return false;
    return true;
  }
  bool operator != (const MakeStreamSnapshotResponse &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const MakeStreamSnapshotResponse & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(MakeStreamSnapshotResponse &a, MakeStreamSnapshotResponse &b);

}}} // namespace

#endif