//
//  NativeLogic.h
//  RubetekEvo
//
//  Created by Andrey Verizhnikov on 13.08.15.
//
//

#define MSW_DISABLE_LARGE_FILE // for libnet

// STL
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include <memory>
#include <sstream>

// NET

#include <rubetek/client/controller_link.hpp>
#include <rubetek/essence/property.hpp>
#include <rubetek/essence/property/client_id.hpp>

#include "imc_constants.h"
#include <rubetek/proto/thrift/serialize.hpp>

#include "ModuleTypes.h"


#ifndef __RubetekEvo__NativeLogic__
#define __RubetekEvo__NativeLogic__

namespace rubetek {

using namespace client;
using namespace std;

class NativeLogicDelegate {
public:
    virtual void on_find_controller() = 0;
    virtual void on_unknown_controllers(unknown_controller_list controllers) = 0;
    virtual void on_known_controllers(known_controller_list controllers) = 0;
    virtual void on_server_connection_status(server_connection_status error) = 0;
    virtual void on_all_controllers(known_controller_list controllers,
                                    unknown_controller_list unknown_controllers) = 0;
    virtual void on_connecting_to_controller(controller_info info, std::string alias) = 0;
    virtual void on_controller_connection_status(controller_availability state, std::string ip) = 0;
    virtual void on_password_request() = 0;
    virtual void on_controller_error(controller_error error) = 0;
    virtual void on_ready(std::string project_json_file) = 0;
    virtual void on_properties_received(property_address address, property_list properties) = 0;
    virtual void on_get_ccserver_response(std::string const& s) = 0;
    virtual void on_reset_HAP(std::string const&s) = 0;
    virtual void on_added_intercom_response(std::string const& s) = 0;
    virtual void on_found_camerare_sponse(std::string const& s) = 0;
    virtual void on_get_camera_info_response(std::string const& s) = 0;
    virtual void on_get_camera_full_info_response(std::string const& s) = 0;
    virtual void on_get_camera_snapshot_response(std::string const& s) = 0;
    virtual void on_set_timezone_response(std::string const& s) = 0;
    virtual void on_get_timezone_response(std::string const& s) = 0;
    virtual void on_switch_to_ap_mode_response(std::string const& s) = 0;
    virtual void on_get_list_clients_response(std::string const& s) = 0;
    virtual void on_get_list_wifi_network_response(std::string const& s) = 0;
    virtual void on_get_wifi_status_response(std::string const& s) = 0;
    virtual void on_scenarios_list_response(std::string const& s) = 0;
    virtual void on_add_scenario_response(std::string const& s) = 0;
    virtual void on_list_devices_response(std::vector<ModuleDevice> devices) = 0;
    virtual void on_device_delete_response(bool state, std::string deviceId) = 0;
    virtual void on_start_add_device_response(bool state) = 0;
    virtual void on_get_ccversion_response(std::string const& s) = 0;
    //virtual void on_reboot_cc_response() = 0; // will not call by engine because CC fall into reset immediately
    
    virtual void on_firmware_update_complete(std::string const& s) = 0;
    virtual void on_check_firmware_update_response(std::string const& s) = 0;
    virtual void on_remove_client_response(std::string const& s) = 0;
    virtual void on_connect_to_wifi_network_response(std::string const& s) = 0;
    virtual void on_sync_add_token_response(std::string const& s) = 0;
    virtual void on_get_list_token_response(std::string const& s) = 0;
    virtual void video_from_relay_server(std::string const& s) = 0;
    virtual void on_local_notification(std::string const& s) = 0;
    virtual void on_local_notification_snapshot_data(std::string const& s) = 0;
    virtual void video_history_received(std::string const& s) = 0;
    virtual void on_added_camera_response(std::string const& s) = 0;
    virtual void added_device_response(bool state) = 0;
    virtual void added_devices_list(std::vector<ModuleDevice> devices, bool isNew) = 0;
    virtual void on_found_rfdevice(std::string const& s) = 0;
    
    virtual void add_rfdevice(std::string const& s) = 0;
    virtual void on_added_wifi_device_response(std::string const& s) = 0;
    virtual void on_start_removing_device_response(bool state) = 0;
    virtual void on_removed_device_response(bool status, std::vector<std::string> rmdList) = 0;
    virtual void on_get_statistics_response(std::string const& s) = 0;
    virtual void on_get_consumption_responce(const std::string &s)  = 0;

    virtual ~NativeLogicDelegate() {}
};

class NativeLogic : public controller_link_back_interface
{
public:
    
    void init();
    NativeLogic(string const& uniqueId,
                string deviceName,
                string versionOS,
                string path,
                string lang,
                string notifyId,
                bool debug,
                string defaultServer,
                string clientVersion);
    
    void set_delegate(weak_ptr<NativeLogicDelegate> delegate);
    
    // NETLIB
    virtual void on_find_controller();
    virtual void on_unknown_controllers(unknown_controller_list);
    virtual void on_known_controllers(known_controller_list);
    virtual void on_all_controllers(known_controller_list, unknown_controller_list);
    virtual void on_notify_about_location(known_controller_list) ;
    virtual void on_server_connection_status(server_connection_status) ;
    virtual void on_connecting_to_controller(controller_info, std::string alias) ;
    virtual void on_controller_connection_status(controller_availability, std::string ip) ;
    virtual void on_controller_rename(std::string const&);
    virtual void on_controller_error(controller_error);
    virtual void on_password_request(option_was_removed) ;
    virtual void on_project_error(project_error) ;
    virtual void on_project_updating() ;
    virtual void on_ready(std::string project_json_file) ;
    virtual void on_properties_received(property_address, property_list) ;
    virtual void on_module_command(std::string const&);
    virtual void on_server_directly(server_packet_type, std::string const&);
    virtual void on_server_auxiliary(std::string const&);
    virtual void on_update_required();
    
    ~NativeLogic();
    
    // callback for device
    void send_property_b(std::string address, int pid, bool value);
    void send_property_f(std::string address, int pid, float value);
    void send_property_s(std::string address, int pid, std::string value);
    void send_property_i(std::string address, int pid, int value);
    //void send_property(property_address, property);
    //void send_property(property_address, property_list);
    void send_password(string);
    void send_project(string const&);
    void send_command(string const&);
    
    //scenario managment
    void createNewScenario(tag::homebuilder::ScriptDescription script);
    void modifyScenario(tag::homebuilder::ScriptDescription script, std::string const& script_id);
    void requestScenariosList();
    void scenarioListResponse(std::string const& s);
    void addScenarioResponse(std::string const& s);
    void deleteScenario(std::string const& s);
    
    //device managment
    void getDeviceList();
    void deleteDeviceById(std::string deviceId, std::string moduleName);
    void startAddDevice();
    void startRemoveDevice();
    void cancelOperation();
    void addRFDeviceByType(std::string rf_id, std::string device_type);
    void addWIFIDevice(std::map<std::string, std::string> wifiDeviceInfo);
    
    //camera managment
    void findCamera();
    void getCameraInfo(std::string const& endpoint);
    void getCameraFullInfo(std::map<std::string, std::string> cameraInfo);
    void addCamera(std::map<std::string, std::string> cameraInfo, int port);
    void addIntercom(std::map<std::string, std::string> cameraInfo, int port, int intPort);
    void requestVideoHistory();
    void getVideoFiles(std::string const& camera_id, int count_files, std::string const& last_file);
    void getCameraSnapshot(std::string const& camera_id);
    
    //statistics managment
    void getStatisticsRequest(std::string const& did, int property_id, int begin_time, int end_time, short width);
    void getEnergyConsumption(std::string const& did, int begin_time, int end_time, std::string const& context = "");
    
    //setting managment
    void getCCVersion();
    void checkFirmwareUpdate();
    void startFirmwareUpdate();
    void getTimeZone();
    void setTimeZone(std::string const& timezone);
    void switchToApMode();
    void getListClients();
    void getListWifiNetwork();
    void getWifiStatus();
    void removeClient(std::string const& clientId);
    bool firstClientStart(std::string const& clientId);
    void connectToWifiNetwork(std::string const& ssid, std::string const& encryption, std::string const& password);
    void syncAddToken(std::string const& token, std::string const& login, std::string const& type);
    void getListToken();
    void finalReset();
    void rebootCC();
    
    // HAP
    //void initHAP(std::string const &language);
    //void checkHAP();
    //void getHAPpassword();
    //void deinitHAP();
    void resetHAP();
    
    //connection
    void connect_to(std::string const & str);
    void setAutoConnectToRecent(bool set);
    std::string parseQR(std::string const & qr);
    
    // scheduler function
    void process();
    
    // manage connection
    void disconnect();
    int updating_progress();
    void focus(bool);
    void auto_connect_by_location(bool);
    void show_new_controllers();
    void show_known_controllers();
    void show_all_controllers();
    void rename_controller(std::string const& cid, std::string const& name);
    void remove_controller(std::string const & cid);
    void remove_all_controllers();
    void set_project_name(std::string const & name);
    void set_notification_id(std::string const & notification_id);
    void refresh();
    
    void requestVideo(std::string const& cid, std::string const& local_uri);
    bool hasRemoteChannelId();
    
    //debug fucntions
    void getCCServer();
    void changeCCServer(std::string const& serverName);
    void recreateCCForUnique(std::string const& serverName, std::string const & ccId);
    
    void cancel_show_list();
    void synchronize_projects(const account_projects &lst);
    // current connected controller
    string current_controller_version() const;
    string current_controller_name() const;
    unique_controller_id current_controller_id() const;
    
    std::string current_controller_ip() const;
    
    static unique_client_id client_id;
    static device_info deviceInfo;
    static string work_path;
    static string unique_id;
    static string device_name;
    
    
private:
    weak_ptr<NativeLogicDelegate> delegate;
    
    static constexpr const char* KEY = "XCgbRXZo2sz9PoUA6RW2";
    static const int LEN = 20;
    
    void listDeviceResponse(std::string const& s);
    void startAddDeviceResponse(std::string const& s);
    void addDeviceResponse(std::string const& s);
    void addCameraResponse(std::string const& s);
    void findCameraResponse(std::string const& s);
    void getCameraInfoResponse(std::string const& s);
    void getCameraFullInfoResponse(std::string const& s);
    void addIntercomResponse(std::string const& s);
    void camerasVideoHistoryResponse(std::string const& s);
    void getCameraSnapshotResponse(std::string const& s);
    void interviewResponse(std::string const& s);
    void foundRFDevice(std::string const& s);
    void addRFDeviceByTypeResponse(std::string const& s);
    void addWIFIDeviceResponse(std::string const& s);
    std::vector<ModuleDevice> parseDevicesList(std::string const& s);
    
    void getCCVersionResponse(std::string const& s);
    void checkFirmwareUpdateResponse(std::string const& s);
    void firmwareUpdateComplete(std::string const& s);
    void getListClientsResponse(std::string const& s);
    void getListWifiNetworkResponse(std::string const& s);
    void getWifiStatusResponse(std::string const& s);
    void removeClientResponse(std::string const& s);
    void connectToWifiNetworkResponse(std::string const& s);
    void switchToApModeResponse(std::string const& s);
    void getTimeZoneResponse(std::string const& s);
    void setTimeZoneResponse(std::string const& s);
    void syncAddTokenResponse(std::string const& s);
    void getListTokenResponse(std::string const& s);
    //void rebootCCResponse(std::string const& s);
    
    void getStatisticsResponse(std::string const& s);
    void getConsumptionResponse(std::string const& s);
    
    void startRemoveDeviceResponse(std::string const& s);
    void removeDeviceResponse(std::string const& s);
    void deleteDeviceByIdResponse(std::string const& s);
    void cancelOperationResponse(std::string const& s);
    
    void videoFromServerRelay(std::string const& s);
    
    void localNotification(std::string const& s);
    void localNotificationFileData(std::string const& s);
    
    unique_ptr<controller_link> connection_;
    controller_info cur_info_;
    std::string current_controller_ip_;
    
    //debug
    void debugLogging(bool isDebug, std::string path);
};
}
#endif /* defined(__RubetekEvo__NativeLogic__) */
