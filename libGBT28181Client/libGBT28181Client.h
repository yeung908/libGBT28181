#ifndef _LIBGBT28181CLIENT_H_
#define _LIBGBT28181CLIENT_H_

#ifdef __cplusplus
#define C_EXPORT_BEGIN extern "C" {
#define C_EXPORT_END }
#else
#define C_EXPORT_BEGIN
#define C_EXPORT_END
#endif // __cplusplus

#ifdef LIBGBT28181CLIENT_EXPORTS
#define LIBGBT28181CLIENT_API __declspec(dllexport)
#else
#define LIBGBT28181CLIENT_API __declspec(dllimport)
#endif

C_EXPORT_BEGIN

#include "callbackDefines.h"

#define GBT28181_SUCCESS                            0
#define GBT28181_UNDEFINED_ERROR                    -1
#define GBT28181_BADPARAMETER                       -2
#define GBT28181_WRONG_STATE                        -3
#define GBT28181_NOMEM                              -4
#define GBT28181_SYNTAXERROR                        -5
#define GBT28181_NOTFOUND                           -6
#define GBT28181_API_NOT_INITIALIZED                -7
#define GBT28181_NO_NETWORK                         -10
#define GBT28181_PORT_BUSY                          -11
#define GBT28181_UNKNOWN_HOST                       -12
#define GBT28181_DISK_FULL                          -30
#define GBT28181_NO_RIGHTS                          -31
#define GBT28181_FILE_NOT_EXIST                     -32
#define GBT28181_TIMEOUT                            -50
#define GBT28181_TOOMUCHCALL                        -51
#define GBT28181_WRONG_FORMAT                       -52
#define GBT28181_NOCOMMONCODEC                      -53
#define GBT28181_CLIENT_UNINITIALED                 -10000
#define GBT28181_INTERVAL_TOO_SMALL                 -10001
#define GBT28181_THREAD_CREATE_FAILED               -10002
#define GBT28181_ESSENTIAL_CLIENT_PARAMETER_ERROR   -10003
#define GBT28181_CALLBACK_NOTSET                    -10004
#define GBT28181_ALREADY_ONLINE                     -10005
#define GBT28181_MUST_ONLINE                        -10006
#define GBT28181_HANDLE_DEPLETED                    -10007
#define GBT28181_INVALID_HANDLE                     -10008
#define GBT28181_ALREADY_STREAMING                  -10009
#define GBT28181_WRONG_PTZ_CMD                      -10010

typedef struct _PTZ_control_data
{
    uint8_t left;
    uint8_t right;
    uint8_t up;
    uint8_t down;
    uint8_t zoom_in;
    uint8_t zoom_out;
    uint8_t relative_pan_speed;
    uint8_t relative_tilt_speed;
    uint8_t relative_zoom_speed;
}PTZ_control_data;

LIBGBT28181CLIENT_API int GBT28181_client_initial(void);

LIBGBT28181CLIENT_API int GBT28181_set_client_name(char* SIP_user_name);

LIBGBT28181CLIENT_API int GBT28181_set_client_ID(char* SIP_user_ID);

LIBGBT28181CLIENT_API int GBT28181_set_client_password(char* SIP_password);

LIBGBT28181CLIENT_API int GBT28181_set_client_IP(char* client_IP);

LIBGBT28181CLIENT_API int GBT28181_set_client_port(int local_port);

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_ID(char* server_ID);

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_domain(char* server_domain);

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_IP(char* server_IP);

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_port(int server_port);

LIBGBT28181CLIENT_API int GBT28181_set_expiration_interval(int seconds);

LIBGBT28181CLIENT_API int GBT28181_set_heartbeat_interval(int seconds);

LIBGBT28181CLIENT_API int GBT28181_set_max_time_out_number(int max_number);

#define GBT28181_AF_INET    2
#define GBT28181_AF_INET6   23

LIBGBT28181CLIENT_API int GBT28181_set_address_family(int address_family);

#define GBT28181_IPPROTO_TCP    6
#define GBT28181_IPPROTO_UDP    17

LIBGBT28181CLIENT_API int GBT28181_set_IP_protocol(int protocol);

LIBGBT28181CLIENT_API int GBT28181_set_query_device_info_callback(function_query_device_info_callback cb);

LIBGBT28181CLIENT_API int GBT28181_set_query_device_status_callback(function_query_device_status_callback cb);

LIBGBT28181CLIENT_API int GBT28181_set_query_catalog_callback(function_query_catalog_callback cb);

LIBGBT28181CLIENT_API int GBT28181_set_max_number_of_live_video(uint32_t max_number);

LIBGBT28181CLIENT_API int GBT28181_client_go_online(void);

LIBGBT28181CLIENT_API int GBT28181_query_device_info(char* target_sip_user_name, char* target_IP, uint16_t port);

LIBGBT28181CLIENT_API int GBT28181_query_device_status(char* target_sip_user_name, char* target_IP, uint16_t port);

LIBGBT28181CLIENT_API int GBT28181_query_catalog(char* target_sip_user_name, char* target_IP, uint16_t port);

LIBGBT28181CLIENT_API int GBT28181_PTZ_control(char* target_sip_user_name, char* target_IP, uint16_t port, PTZ_control_data* p_PTZ_control_data);

LIBGBT28181CLIENT_API int GBT28181_get_idle_real_time_stream_handle(uint32_t* handle);

LIBGBT28181CLIENT_API int GBT28181_set_RTP_port(uint32_t handle, uint16_t port);

LIBGBT28181CLIENT_API int GBT28181_set_RTP_protocol(uint32_t handle, int protocol);

LIBGBT28181CLIENT_API int GBT28181_set_RTP_payload_give_out_callback(uint32_t handle, void* cb);

LIBGBT28181CLIENT_API int GBT28181_get_real_time_stream(uint32_t handle, char* target_sip_user_name, char* target_IP, uint16_t port);

LIBGBT28181CLIENT_API int GBT28181_close_real_time_stream(uint32_t handle);

LIBGBT28181CLIENT_API int GBT28181_free_client(void);

C_EXPORT_END

#endif // !_LIBGBT28181CLIENT_H_



