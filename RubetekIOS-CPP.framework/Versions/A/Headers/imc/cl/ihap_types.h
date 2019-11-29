/**
 * Autogenerated by Thrift Compiler (0.9.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef ihap_TYPES_H
#define ihap_TYPES_H

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include "version_types.h"


namespace rubetek { namespace proto { namespace ihap {

struct packet_type {
  enum type {
    version = 0,
    controller_info = 1,
    remove = 2,
    activate = 3,
    inactivate = 4,
    password = 5,
    no_project = 6,
    no_required_resolution = 7,
    project_size = 8,
    large_begin = 9,
    large_block = 10,
    large_end = 11,
    loading_cancel = 12,
    project = 13,
    ready = 15,
    property = 16,
    refresh = 17,
    module_command = 18,
    project_alias = 19,
    rename = 20,
    notification_id = 21,
    module_command_large = 22,
    module_command_end = 23,
    become_known = 24,
    remove_client = 25
  };
};

extern const std::map<int, const char*> _packet_type_VALUES_TO_NAMES;

typedef struct _packet__isset {
  _packet__isset() : type(false), payload(false) {}
  bool type;
  bool payload;
} _packet__isset;

class packet {
 public:

  static const char* ascii_fingerprint; // = "24652790C81ECE22B629CB60A19F1E93";
  static const uint8_t binary_fingerprint[16]; // = {0x24,0x65,0x27,0x90,0xC8,0x1E,0xCE,0x22,0xB6,0x29,0xCB,0x60,0xA1,0x9F,0x1E,0x93};

  packet() : type((packet_type::type)0), payload() {
  }

  virtual ~packet() throw() {}

  packet_type::type type;
  std::string payload;

  _packet__isset __isset;

  void __set_type(const packet_type::type val) {
    type = val;
  }

  void __set_payload(const std::string& val) {
    payload = val;
    __isset.payload = true;
  }

  bool operator == (const packet & rhs) const
  {
    if (!(type == rhs.type))
      return false;
    if (__isset.payload != rhs.__isset.payload)
      return false;
    else if (__isset.payload && !(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const packet &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const packet & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(packet &a, packet &b);

typedef struct _controller_info__isset {
  _controller_info__isset() : name(false), version(false), system_info(false), demo_mode(false), server_address(false) {}
  bool name;
  bool version;
  bool system_info;
  bool demo_mode;
  bool server_address;
} _controller_info__isset;

class controller_info {
 public:

  static const char* ascii_fingerprint; // = "D15635F063E02B657E6FB6315CC06E70";
  static const uint8_t binary_fingerprint[16]; // = {0xD1,0x56,0x35,0xF0,0x63,0xE0,0x2B,0x65,0x7E,0x6F,0xB6,0x31,0x5C,0xC0,0x6E,0x70};

  controller_info() : name(), version(), system_info(), demo_mode(0), server_address() {
  }

  virtual ~controller_info() throw() {}

  std::string name;
  std::string version;
  std::string system_info;
  bool demo_mode;
  std::string server_address;

  _controller_info__isset __isset;

  void __set_name(const std::string& val) {
    name = val;
  }

  void __set_version(const std::string& val) {
    version = val;
  }

  void __set_system_info(const std::string& val) {
    system_info = val;
  }

  void __set_demo_mode(const bool val) {
    demo_mode = val;
  }

  void __set_server_address(const std::string& val) {
    server_address = val;
  }

  bool operator == (const controller_info & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(version == rhs.version))
      return false;
    if (!(system_info == rhs.system_info))
      return false;
    if (!(demo_mode == rhs.demo_mode))
      return false;
    if (!(server_address == rhs.server_address))
      return false;
    return true;
  }
  bool operator != (const controller_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const controller_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(controller_info &a, controller_info &b);

typedef struct _activate__isset {
  _activate__isset() : project_crc(false), device_name(false), language(false), notification_id(false), operation_system(false), project_alias(false), client_version(false), operation_system_version(false) {}
  bool project_crc;
  bool device_name;
  bool language;
  bool notification_id;
  bool operation_system;
  bool project_alias;
  bool client_version;
  bool operation_system_version;
} _activate__isset;

class activate {
 public:

  static const char* ascii_fingerprint; // = "FFF38465DA0C4DDE1318B6F60085F7EB";
  static const uint8_t binary_fingerprint[16]; // = {0xFF,0xF3,0x84,0x65,0xDA,0x0C,0x4D,0xDE,0x13,0x18,0xB6,0xF6,0x00,0x85,0xF7,0xEB};

  activate() : project_crc(0), device_name(), language(), notification_id(), operation_system(), project_alias(), client_version(), operation_system_version() {
  }

  virtual ~activate() throw() {}

  int32_t project_crc;
  std::string device_name;
  std::string language;
  std::string notification_id;
  std::string operation_system;
  std::string project_alias;
  std::string client_version;
  std::string operation_system_version;

  _activate__isset __isset;

  void __set_project_crc(const int32_t val) {
    project_crc = val;
  }

  void __set_device_name(const std::string& val) {
    device_name = val;
  }

  void __set_language(const std::string& val) {
    language = val;
  }

  void __set_notification_id(const std::string& val) {
    notification_id = val;
  }

  void __set_operation_system(const std::string& val) {
    operation_system = val;
  }

  void __set_project_alias(const std::string& val) {
    project_alias = val;
  }

  void __set_client_version(const std::string& val) {
    client_version = val;
  }

  void __set_operation_system_version(const std::string& val) {
    operation_system_version = val;
  }

  bool operator == (const activate & rhs) const
  {
    if (!(project_crc == rhs.project_crc))
      return false;
    if (!(device_name == rhs.device_name))
      return false;
    if (!(language == rhs.language))
      return false;
    if (!(notification_id == rhs.notification_id))
      return false;
    if (!(operation_system == rhs.operation_system))
      return false;
    if (!(project_alias == rhs.project_alias))
      return false;
    if (!(client_version == rhs.client_version))
      return false;
    if (!(operation_system_version == rhs.operation_system_version))
      return false;
    return true;
  }
  bool operator != (const activate &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const activate & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(activate &a, activate &b);

typedef struct _password__isset {
  _password__isset() : value(false) {}
  bool value;
} _password__isset;

class password {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  password() : value() {
  }

  virtual ~password() throw() {}

  std::string value;

  _password__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const password & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const password &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const password & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(password &a, password &b);

typedef struct _password_request__isset {
  _password_request__isset() : who_removed(false) {}
  bool who_removed;
} _password_request__isset;

class password_request {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  password_request() : who_removed() {
  }

  virtual ~password_request() throw() {}

  std::string who_removed;

  _password_request__isset __isset;

  void __set_who_removed(const std::string& val) {
    who_removed = val;
  }

  bool operator == (const password_request & rhs) const
  {
    if (!(who_removed == rhs.who_removed))
      return false;
    return true;
  }
  bool operator != (const password_request &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const password_request & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(password_request &a, password_request &b);

typedef struct _project_size__isset {
  _project_size__isset() : value(false) {}
  bool value;
} _project_size__isset;

class project_size {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  project_size() : value(0) {
  }

  virtual ~project_size() throw() {}

  int32_t value;

  _project_size__isset __isset;

  void __set_value(const int32_t val) {
    value = val;
  }

  bool operator == (const project_size & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const project_size &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const project_size & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(project_size &a, project_size &b);

typedef struct _large_begin__isset {
  _large_begin__isset() : size(false), payload(false) {}
  bool size;
  bool payload;
} _large_begin__isset;

class large_begin {
 public:

  static const char* ascii_fingerprint; // = "3F5FC93B338687BC7235B1AB103F47B3";
  static const uint8_t binary_fingerprint[16]; // = {0x3F,0x5F,0xC9,0x3B,0x33,0x86,0x87,0xBC,0x72,0x35,0xB1,0xAB,0x10,0x3F,0x47,0xB3};

  large_begin() : size(0), payload() {
  }

  virtual ~large_begin() throw() {}

  int32_t size;
  std::string payload;

  _large_begin__isset __isset;

  void __set_size(const int32_t val) {
    size = val;
  }

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const large_begin & rhs) const
  {
    if (!(size == rhs.size))
      return false;
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const large_begin &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const large_begin & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(large_begin &a, large_begin &b);

typedef struct _large_block__isset {
  _large_block__isset() : payload(false) {}
  bool payload;
} _large_block__isset;

class large_block {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  large_block() : payload() {
  }

  virtual ~large_block() throw() {}

  std::string payload;

  _large_block__isset __isset;

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const large_block & rhs) const
  {
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const large_block &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const large_block & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(large_block &a, large_block &b);

typedef struct _large_block_end__isset {
  _large_block_end__isset() : payload(false) {}
  bool payload;
} _large_block_end__isset;

class large_block_end {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  large_block_end() : payload() {
  }

  virtual ~large_block_end() throw() {}

  std::string payload;

  _large_block_end__isset __isset;

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const large_block_end & rhs) const
  {
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const large_block_end &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const large_block_end & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(large_block_end &a, large_block_end &b);

typedef struct _project__isset {
  _project__isset() : crc(false) {}
  bool crc;
} _project__isset;

class project {
 public:

  static const char* ascii_fingerprint; // = "E86CACEB22240450EDCBEFC3A83970E4";
  static const uint8_t binary_fingerprint[16]; // = {0xE8,0x6C,0xAC,0xEB,0x22,0x24,0x04,0x50,0xED,0xCB,0xEF,0xC3,0xA8,0x39,0x70,0xE4};

  project() : crc(0) {
  }

  virtual ~project() throw() {}

  int32_t crc;

  _project__isset __isset;

  void __set_crc(const int32_t val) {
    crc = val;
  }

  bool operator == (const project & rhs) const
  {
    if (!(crc == rhs.crc))
      return false;
    return true;
  }
  bool operator != (const project &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const project & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(project &a, project &b);

typedef struct _project_content__isset {
  _project_content__isset() : value(false) {}
  bool value;
} _project_content__isset;

class project_content {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  project_content() : value() {
  }

  virtual ~project_content() throw() {}

  std::string value;

  _project_content__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const project_content & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const project_content &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const project_content & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(project_content &a, project_content &b);

typedef struct _property__isset {
  _property__isset() : payload(false) {}
  bool payload;
} _property__isset;

class property {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  property() : payload() {
  }

  virtual ~property() throw() {}

  std::string payload;

  _property__isset __isset;

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const property & rhs) const
  {
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const property &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const property & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(property &a, property &b);

typedef struct _module_command__isset {
  _module_command__isset() : payload(false) {}
  bool payload;
} _module_command__isset;

class module_command {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  module_command() : payload() {
  }

  virtual ~module_command() throw() {}

  std::string payload;

  _module_command__isset __isset;

  void __set_payload(const std::string& val) {
    payload = val;
  }

  bool operator == (const module_command & rhs) const
  {
    if (!(payload == rhs.payload))
      return false;
    return true;
  }
  bool operator != (const module_command &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const module_command & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(module_command &a, module_command &b);

typedef struct _project_alias__isset {
  _project_alias__isset() : value(false) {}
  bool value;
} _project_alias__isset;

class project_alias {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  project_alias() : value() {
  }

  virtual ~project_alias() throw() {}

  std::string value;

  _project_alias__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const project_alias & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const project_alias &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const project_alias & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(project_alias &a, project_alias &b);

typedef struct _rename__isset {
  _rename__isset() : value(false) {}
  bool value;
} _rename__isset;

class rename {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  rename() : value() {
  }

  virtual ~rename() throw() {}

  std::string value;

  _rename__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const rename & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const rename &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const rename & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(rename &a, rename &b);

typedef struct _notification_id__isset {
  _notification_id__isset() : value(false) {}
  bool value;
} _notification_id__isset;

class notification_id {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  notification_id() : value() {
  }

  virtual ~notification_id() throw() {}

  std::string value;

  _notification_id__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const notification_id & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const notification_id &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const notification_id & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(notification_id &a, notification_id &b);

typedef struct _remove_client__isset {
  _remove_client__isset() : value(false) {}
  bool value;
} _remove_client__isset;

class remove_client {
 public:

  static const char* ascii_fingerprint; // = "EFB929595D312AC8F305D5A794CFEDA1";
  static const uint8_t binary_fingerprint[16]; // = {0xEF,0xB9,0x29,0x59,0x5D,0x31,0x2A,0xC8,0xF3,0x05,0xD5,0xA7,0x94,0xCF,0xED,0xA1};

  remove_client() : value() {
  }

  virtual ~remove_client() throw() {}

  std::string value;

  _remove_client__isset __isset;

  void __set_value(const std::string& val) {
    value = val;
  }

  bool operator == (const remove_client & rhs) const
  {
    if (!(value == rhs.value))
      return false;
    return true;
  }
  bool operator != (const remove_client &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const remove_client & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

void swap(remove_client &a, remove_client &b);

}}} // namespace

#endif
