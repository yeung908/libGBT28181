#include "libGBT28181Client.h"
#include "clientConfigDefine.h"
#include "workingThread.h"

#define CHECK_NULL_AND_RETURN(pointer)  \
if(NULL == (pointer))                   \
{                                       \
    return GBT28181_NOMEM;              \
}                                       \
else                                    \
{                                       \
    return GBT28181_SUCCESS;            \
}

#define CHECK_NULL_PARAMETER(pointer)   \
if(NULL == (pointer))                   \
{                                       \
    return GBT28181_BADPARAMETER;       \
}

#define CHECK_INITIALED(bool_initialed) \
if(!(bool_initialed))                   \
{                                       \
    return GBT28181_CLIENT_UNINITIALED; \
}

#define CHECK_CALLBACK(pointer)         \
if(NULL == (pointer))                   \
{                                       \
    return GBT28181_CALLBACK_NOTSET;    \
}

#define CHECK_ONLINE_NO_SET(bool_online)    \
if((bool_online))                           \
{                                           \
    return GBT28181_ALREADY_ONLINE;         \
}

#define CHECK_MUST_ON_LINE(bool_online) \
if(!(bool_online))                      \
{                                       \
    return GBT28181_MUST_ONLINE;        \
}

#define CHECK_NOT_STREAMING(bool_streaming) \
if(bool_streaming)                          \
{                                           \
    return GBT28181_ALREADY_STREAMING;      \
}

client_configurations global_client_configurations;

int check_handle(uint32_t handle)
{
    if(0 == global_client_configurations.max_live_video_number)
    {
        return OSIP_API_NOT_INITIALIZED;
    }
    if(handle < 0 || global_client_configurations.max_live_video_number < handle)
    {
        return GBT28181_INVALID_HANDLE;
    }
    if(NULL == global_client_configurations.live_video_context_pointer_array[handle])
    {
        return GBT28181_INVALID_HANDLE;
    }
    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_client_initial(void)
{
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.online = false;
    global_client_configurations.initialed = true;
    InitializeCriticalSectionAndSpinCount(&global_client_configurations.critical_section, 4000);
    global_client_configurations.client_user_name = NULL;
    global_client_configurations.client_authentication_ID = NULL;
    global_client_configurations.client_password = NULL;
    global_client_configurations.client_IP = NULL;
    global_client_configurations.client_port = 5060;
    global_client_configurations.server_ID = NULL;
    global_client_configurations.server_domain = NULL;
    global_client_configurations.server_IP = NULL;
    global_client_configurations.server_port = 5060;
    global_client_configurations.expiration_interval = 3600;
    global_client_configurations.heartbeat_interval = 60;
    global_client_configurations.max_time_out_number = 3;
    global_client_configurations.address_family = GBT28181_AF_INET;
    global_client_configurations.protocol = GBT28181_IPPROTO_UDP;
    global_client_configurations.exosip_context = eXosip_malloc();
    global_client_configurations.registration_ID = 0;
    global_client_configurations.thread_loop = false;
    global_client_configurations.MANSCDP_SN = 1;
    global_client_configurations.give_out_query_device_info_result = NULL;
    global_client_configurations.give_out_query_device_status_result = NULL;
    global_client_configurations.give_out_query_catalog_result = NULL;
    global_client_configurations.max_live_video_number = 0;
    global_client_configurations.live_video_context_pointer_array = NULL;

    CHECK_NULL_AND_RETURN(global_client_configurations.exosip_context);
}

LIBGBT28181CLIENT_API int GBT28181_set_client_name(char* SIP_user_name)
{
    CHECK_NULL_PARAMETER(SIP_user_name);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.client_user_name = osip_strdup(SIP_user_name);

    CHECK_NULL_AND_RETURN(global_client_configurations.client_user_name);
}

LIBGBT28181CLIENT_API int GBT28181_set_client_ID(char* SIP_user_ID)
{
    CHECK_NULL_PARAMETER(SIP_user_ID);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.client_authentication_ID = osip_strdup(SIP_user_ID);

    CHECK_NULL_AND_RETURN(global_client_configurations.client_authentication_ID);
}

LIBGBT28181CLIENT_API int GBT28181_set_client_password(char* SIP_password)
{
    CHECK_NULL_PARAMETER(SIP_password);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.client_password = osip_strdup(SIP_password);

    CHECK_NULL_AND_RETURN(global_client_configurations.client_password);
}

LIBGBT28181CLIENT_API int GBT28181_set_client_IP(char* client_IP)
{
    CHECK_NULL_PARAMETER(client_IP);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.client_IP = osip_strdup(client_IP);

    CHECK_NULL_AND_RETURN(global_client_configurations.client_IP);
}

LIBGBT28181CLIENT_API int GBT28181_set_client_port(int local_port)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.client_port = local_port;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_ID(char* server_ID)
{
    CHECK_NULL_PARAMETER(server_ID);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.server_ID = osip_strdup(server_ID);

    CHECK_NULL_AND_RETURN(global_client_configurations.server_ID);
}

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_domain(char* server_domain)
{
    CHECK_NULL_PARAMETER(server_domain);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.server_domain = osip_strdup(server_domain);

    CHECK_NULL_AND_RETURN(global_client_configurations.server_domain);
}

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_IP(char* server_IP)
{
    CHECK_NULL_PARAMETER(server_IP);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.server_IP = osip_strdup(server_IP);

    CHECK_NULL_AND_RETURN(global_client_configurations.server_IP);
}

LIBGBT28181CLIENT_API int GBT28181_set_SIP_server_port(int server_port)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.server_port = server_port;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_expiration_interval(int seconds)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    if(seconds < 3600)
    {
        return GBT28181_INTERVAL_TOO_SMALL;
    }

    global_client_configurations.expiration_interval = seconds;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_heartbeat_interval(int seconds)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    if(seconds < 60)
    {
        return GBT28181_INTERVAL_TOO_SMALL;
    }

    global_client_configurations.heartbeat_interval = seconds;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_max_time_out_number(int max_number)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    if(max_number < 3)
    {
        return GBT28181_INTERVAL_TOO_SMALL;
    }

    global_client_configurations.max_time_out_number = max_number;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_address_family(int address_family)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    if(address_family < 2 || 30 < address_family)
    {
        return GBT28181_BADPARAMETER;
    }

    global_client_configurations.address_family = address_family;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_IP_protocol(int protocol)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    if(protocol < 6 || 30 < protocol)
    {
        return GBT28181_BADPARAMETER;
    }

    global_client_configurations.protocol = protocol;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_client_go_online(void)
{
    CHECK_INITIALED(global_client_configurations.initialed);

    if(NULL == global_client_configurations.client_user_name ||
       NULL == global_client_configurations.client_authentication_ID ||
       NULL == global_client_configurations.client_password ||
       NULL == global_client_configurations.client_IP ||
       NULL == global_client_configurations.server_ID ||
       NULL == global_client_configurations.server_domain ||
       NULL == global_client_configurations.server_IP)
    {
        return GBT28181_ESSENTIAL_CLIENT_PARAMETER_ERROR;
    }

    int result = 0;
    osip_message_t* registration_message = NULL;
    char* from = osip_malloc(512);
    char* proxy = osip_malloc(512);

    if(NULL == from || NULL == proxy)
    {
        osip_free(from);
        osip_free(proxy);
        return GBT28181_NOMEM;
    }

    result = eXosip_init(global_client_configurations.exosip_context);
    if(GBT28181_SUCCESS != result)
    {
        osip_free(from);
        osip_free(proxy);
        return result;
    }

    result = eXosip_listen_addr(
        global_client_configurations.exosip_context,
        global_client_configurations.protocol,
        NULL,
        global_client_configurations.client_port,
        global_client_configurations.address_family,
        0);
    if(GBT28181_SUCCESS != result)
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return result;
    }

    eXosip_set_user_agent(global_client_configurations.exosip_context, "libGBT28181");

    result = eXosip_add_authentication_info(
        global_client_configurations.exosip_context,
        global_client_configurations.client_user_name,
        global_client_configurations.client_authentication_ID,
        global_client_configurations.client_password,
        NULL,
        NULL);
    if(GBT28181_SUCCESS != result)
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return result;
    }

    snprintf(
        from,
        256,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);

    snprintf(
        proxy,
        256,
        "sip:%s@%s:%d",
        global_client_configurations.server_ID,
        global_client_configurations.server_IP,
        global_client_configurations.server_port);

    global_client_configurations.registration_ID = eXosip_register_build_initial_register(
        global_client_configurations.exosip_context,
        from,
        proxy,
        NULL,
        global_client_configurations.expiration_interval,
        &registration_message);
    if(global_client_configurations.registration_ID < 1)
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return GBT28181_UNDEFINED_ERROR;
    }

    osip_free(from);
    osip_free(proxy);

    result = eXosip_register_send_register(
        global_client_configurations.exosip_context,
        global_client_configurations.registration_ID,
        registration_message);
    if(GBT28181_SUCCESS != result)
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return result;
    }

    global_client_configurations.thread_loop = true;

    if(NULL == osip_thread_create(20000, register_working_thread, &global_client_configurations))
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return GBT28181_THREAD_CREATE_FAILED;
    }

    if(NULL == osip_thread_create(20000, event_working_thread, &global_client_configurations))
    {
        osip_free(from);
        osip_free(proxy);
        eXosip_quit(global_client_configurations.exosip_context);
        return GBT28181_THREAD_CREATE_FAILED;
    }

    global_client_configurations.online = true;

    global_client_configurations.live_video_context_pointer_array =
        osip_malloc(sizeof(real_time_stream_context*) * global_client_configurations.max_live_video_number);
    memset(
        global_client_configurations.live_video_context_pointer_array,
        0x0,
        sizeof(real_time_stream_context*) * global_client_configurations.max_live_video_number);

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_query_device_info_callback(function_query_device_info_callback cb)
{
    CHECK_NULL_PARAMETER(cb);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.give_out_query_device_info_result = cb;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_query_device_info(char* target_sip_user_name, char* target_IP, uint16_t port)
{
    CHECK_NULL_PARAMETER(target_sip_user_name);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    CHECK_CALLBACK(global_client_configurations.give_out_query_device_info_result);

    int result = OSIP_SUCCESS;
    osip_message_t* query_query_device_message = NULL;
    char* from = osip_malloc(512);
    char* to = osip_malloc(512);
    char* proxy = osip_malloc(512);
    char* message_body = osip_malloc(1500);

    if(NULL == from || NULL == to || NULL == proxy || NULL == message_body)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return GBT28181_NOMEM;
    }

    snprintf(
        from,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        to,
        512,
        "sip:%s@%s",
        target_sip_user_name,
        global_client_configurations.server_domain);

    if(NULL != target_IP)
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%u>",
            target_sip_user_name,
            target_IP,
            port);
    }
    else
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%d>",
            global_client_configurations.server_ID,
            global_client_configurations.server_IP,
            global_client_configurations.server_port);
    }

    result = eXosip_message_build_request(
        global_client_configurations.exosip_context,
        &query_query_device_message,
        "MESSAGE",
        to,
        from,
        proxy);
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    result = osip_message_set_content_type(query_query_device_message, "application/MANSCDP+xml");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    snprintf(message_body, 1500, "<?xml version=\"1.0\"?><Query><CmdType>DeviceInfo</CmdType><SN>%u</SN><DeviceID>%s</DeviceID></Query>", global_client_configurations.MANSCDP_SN, target_sip_user_name);

    result = osip_message_set_body(query_query_device_message, message_body, strnlen(message_body, 1500));
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    eXosip_lock(global_client_configurations.exosip_context);
    result = eXosip_message_send_request(global_client_configurations.exosip_context, query_query_device_message);
    eXosip_unlock(global_client_configurations.exosip_context);
    if(0 > result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    EnterCriticalSection(&global_client_configurations.critical_section);
    global_client_configurations.MANSCDP_SN++;
    LeaveCriticalSection(&global_client_configurations.critical_section);

    osip_free(from);
    osip_free(to);
    osip_free(proxy);
    osip_free(message_body);

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_query_device_status_callback(function_query_device_status_callback cb)
{
    CHECK_NULL_PARAMETER(cb);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.give_out_query_device_status_result = cb;

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_query_device_status(char* target_sip_user_name, char* target_IP, uint16_t port)
{
    CHECK_NULL_PARAMETER(target_sip_user_name);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    CHECK_CALLBACK(global_client_configurations.give_out_query_device_status_result);

    int result = OSIP_SUCCESS;
    osip_message_t* query_device_status_message = NULL;
    char* from = osip_malloc(512);
    char* to = osip_malloc(512);
    char* proxy = osip_malloc(512);
    char* message_body = osip_malloc(1500);

    if(NULL == from || NULL == to || NULL == proxy || NULL == message_body)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return GBT28181_NOMEM;
    }

    snprintf(
        from,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        to,
        512,
        "sip:%s@%s",
        target_sip_user_name,
        global_client_configurations.server_domain);

    if(NULL != target_IP)
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%u>",
            target_sip_user_name,
            target_IP,
            port);
    }
    else
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%d>",
            global_client_configurations.server_ID,
            global_client_configurations.server_IP,
            global_client_configurations.server_port);
    }

    result = eXosip_message_build_request(
        global_client_configurations.exosip_context,
        &query_device_status_message,
        "MESSAGE",
        to,
        from,
        proxy);
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    result = osip_message_set_content_type(query_device_status_message, "application/MANSCDP+xml");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    snprintf(message_body, 1500, "<?xml version=\"1.0\"?><Query><CmdType>DeviceStatus</CmdType><SN>%u</SN><DeviceID>%s</DeviceID></Query>", global_client_configurations.MANSCDP_SN, target_sip_user_name);

    result = osip_message_set_body(query_device_status_message, message_body, strnlen(message_body, 1500));
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    eXosip_lock(global_client_configurations.exosip_context);
    result = eXosip_message_send_request(global_client_configurations.exosip_context, query_device_status_message);
    eXosip_unlock(global_client_configurations.exosip_context);
    if(0 > result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    EnterCriticalSection(&global_client_configurations.critical_section);
    global_client_configurations.MANSCDP_SN++;
    LeaveCriticalSection(&global_client_configurations.critical_section);

    osip_free(from);
    osip_free(to);
    osip_free(proxy);
    osip_free(message_body);

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_query_catalog_callback(function_query_catalog_callback cb)
{
    CHECK_NULL_PARAMETER(cb);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.give_out_query_catalog_result = cb;

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_query_catalog(char* target_sip_user_name, char* target_IP, uint16_t port)
{
    CHECK_NULL_PARAMETER(target_sip_user_name);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    CHECK_CALLBACK(global_client_configurations.give_out_query_catalog_result);

    int result = OSIP_SUCCESS;
    osip_message_t* query_catalog_message = NULL;
    char* from = osip_malloc(512);
    char* to = osip_malloc(512);
    char* proxy = osip_malloc(512);
    char* message_body = osip_malloc(1500);

    if(NULL == from || NULL == to || NULL == proxy || NULL == message_body)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return GBT28181_NOMEM;
    }

    snprintf(
        from,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        to,
        512,
        "sip:%s@%s",
        target_sip_user_name,
        global_client_configurations.server_domain);

    if(NULL != target_IP)
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%u>",
            target_sip_user_name,
            target_IP,
            port);
    }
    else
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%d>",
            global_client_configurations.server_ID,
            global_client_configurations.server_IP,
            global_client_configurations.server_port);
    }

    result = eXosip_message_build_request(
        global_client_configurations.exosip_context,
        &query_catalog_message,
        "MESSAGE",
        to,
        from,
        proxy);
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    result = osip_message_set_content_type(query_catalog_message, "application/MANSCDP+xml");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    snprintf(message_body, 1500, "<?xml version=\"1.0\"?><Query><CmdType>Catalog</CmdType><SN>%u</SN><DeviceID>%s</DeviceID></Query>", global_client_configurations.MANSCDP_SN, target_sip_user_name);

    result = osip_message_set_body(query_catalog_message, message_body, strnlen(message_body, 1500));
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    eXosip_lock(global_client_configurations.exosip_context);
    result = eXosip_message_send_request(global_client_configurations.exosip_context, query_catalog_message);
    eXosip_unlock(global_client_configurations.exosip_context);
    if(0 > result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    EnterCriticalSection(&global_client_configurations.critical_section);
    global_client_configurations.MANSCDP_SN++;
    LeaveCriticalSection(&global_client_configurations.critical_section);

    osip_free(from);
    osip_free(to);
    osip_free(proxy);
    osip_free(message_body);

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_free_client(void)
{
    CHECK_INITIALED(global_client_configurations.initialed);

    if(NULL != global_client_configurations.live_video_context_pointer_array)
    {
        for(uint32_t i = 0; i < global_client_configurations.max_live_video_number; i++)
        {
            if(NULL != global_client_configurations.live_video_context_pointer_array[i])
            {
                if(global_client_configurations.live_video_context_pointer_array[i]->real_time_streaming)
                {
                    GBT28181_close_real_time_stream(i);
                }
            }
        }
    }

    osip_free(global_client_configurations.live_video_context_pointer_array);

    uninitial_RTP_library();

    int result = 0;
    osip_message_t* registration_message = NULL;
    char* from = osip_malloc(512);
    char* proxy = osip_malloc(512);

    if(NULL == from || NULL == proxy)
    {
        osip_free(from);
        osip_free(proxy);
        return GBT28181_BADPARAMETER;
    }

    snprintf(
        from,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        proxy,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.server_ID,
        global_client_configurations.server_IP,
        global_client_configurations.server_port);

    if(global_client_configurations.online)
    {
        result = eXosip_register_build_initial_register(
            global_client_configurations.exosip_context,
            from,
            proxy,
            NULL,
            0,
            &registration_message);
        if(result < 1)
        {
            osip_free(from);
            osip_free(proxy);
            return GBT28181_UNDEFINED_ERROR;
        }

        result = eXosip_register_send_register(
            global_client_configurations.exosip_context,
            result,
            registration_message);
        if(GBT28181_SUCCESS != result)
        {
            osip_free(from);
            osip_free(proxy);
            return result;
        }

        global_client_configurations.thread_loop = false;

        osip_usleep(500000);

        eXosip_quit(global_client_configurations.exosip_context);
    }

    DeleteCriticalSection(&global_client_configurations.critical_section);

    osip_free(global_client_configurations.client_user_name);
    osip_free(global_client_configurations.client_authentication_ID);
    osip_free(global_client_configurations.client_password);
    osip_free(global_client_configurations.client_IP);
    osip_free(global_client_configurations.server_ID);
    osip_free(global_client_configurations.server_domain);
    osip_free(global_client_configurations.server_IP);

    osip_free(from);
    osip_free(proxy);

    return GBT28181_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_max_number_of_live_video(uint32_t max_number)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_ONLINE_NO_SET(global_client_configurations.online);

    global_client_configurations.max_live_video_number = max_number;
    initial_RTP_library(max_number);

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_get_idle_real_time_stream_handle(uint32_t* handle)
{
    CHECK_NULL_PARAMETER(handle);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);

    uint32_t i;
    for(i = 0; i < global_client_configurations.max_live_video_number; i++)
    {
        if(NULL == global_client_configurations.live_video_context_pointer_array[i])
        {
            global_client_configurations.live_video_context_pointer_array[i] = osip_malloc(sizeof(real_time_stream_context));
            if(NULL == global_client_configurations.live_video_context_pointer_array[i])
            {
                return OSIP_NOMEM;
            }
            global_client_configurations.live_video_context_pointer_array[i]->call_id = 0;
            global_client_configurations.live_video_context_pointer_array[i]->dialog_id = 0;
            global_client_configurations.live_video_context_pointer_array[i]->real_time_streaming = false;
            global_client_configurations.live_video_context_pointer_array[i]->port_RTP = 0;
            global_client_configurations.live_video_context_pointer_array[i]->port_SIP = 0;
            global_client_configurations.live_video_context_pointer_array[i]->target_IP = NULL;
            global_client_configurations.live_video_context_pointer_array[i]->target_sip_user_name = NULL;
            global_client_configurations.live_video_context_pointer_array[i]->local_IP = osip_strdup(global_client_configurations.client_IP);
            get_new_RTP_session(&global_client_configurations.live_video_context_pointer_array[i]->session_handle);
            *handle = i;
            break;
        }
    }

    if(i == global_client_configurations.max_live_video_number)
    {
        return GBT28181_HANDLE_DEPLETED;
    }
    else
    {
        return OSIP_SUCCESS;
    }
}

LIBGBT28181CLIENT_API int GBT28181_get_real_time_stream(uint32_t handle, char* target_sip_user_name, char* target_IP, uint16_t port)
{
    CHECK_NULL_PARAMETER(target_sip_user_name);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    int result = check_handle(handle);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }

    osip_message_t* invite_SIP_message = NULL;
    char* from = osip_malloc(512);
    char* to = osip_malloc(512);
    char* route = osip_malloc(512);
    char* SDP_payload = osip_malloc(1500);
    char* RTP_protocol = NULL;

    snprintf(
        from,
        512,
        "<sip:%s@%s:%d>",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        to,
        512,
        "<sip:%s@%s>",
        target_sip_user_name,
        global_client_configurations.server_domain);
    if(NULL != target_IP)
    {
        snprintf(
            route,
            512,
            "<sip:%s@%s:%u>",
            target_sip_user_name,
            target_IP,
            port);
        global_client_configurations.live_video_context_pointer_array[handle]->target_sip_user_name = osip_strdup(target_sip_user_name);
        global_client_configurations.live_video_context_pointer_array[handle]->target_IP = osip_strdup(target_IP);
        global_client_configurations.live_video_context_pointer_array[handle]->port_SIP = port;
    }
    else
    {
        snprintf(
            route,
            512,
            "<sip:%s@%s>",
            target_sip_user_name,
            global_client_configurations.server_domain);
    }

    result = eXosip_call_build_initial_invite(
        global_client_configurations.exosip_context,
        &invite_SIP_message,
        to,
        from,
        route,
        "I need live video");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(route);
        osip_free(SDP_payload);
        return result;
    }

    switch(global_client_configurations.live_video_context_pointer_array[handle]->protocol_RTP)
    {
        case GBT28181_IPPROTO_TCP:
            RTP_protocol = "RTP/AVP/TCP";
            break;
        case GBT28181_IPPROTO_UDP:
            RTP_protocol = "RTP/AVP";
            break;
        default:
            break;
    }

    snprintf(
        SDP_payload,
        1500,
        "v=0\r\no=%s 0 0 IN IP4 %s\r\ns=Play\r\nc=IN IP4 %s\r\nt=0 0\r\nm=video %u %s 96 98 97\r\na=recvonly\r\na=rtpmap:96 PS/90000\r\na=rtpmap:98 H264/90000\r\na=rtpmap:97 MPEG4/90000\r\n",
        //"v=0\r\no=%s 0 0 IN IP4 %s\r\ns=Play\r\nc=IN IP4 %s\r\nt=0 0\r\nm=video %u %s 96\r\na=recvonly\r\na=rtpmap:96 PS/90000\r\n",
        //"v=0\r\no=%s 0 0 IN IP4 %s\r\ns=Play\r\nc=IN IP4 %s\r\nt=0 0\r\nm=video %u %s 98 97\r\na=recvonly\r\na=rtpmap:98 H264/90000\r\na=rtpmap:97 MPEG4/90000\r\n",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_IP,
        global_client_configurations.live_video_context_pointer_array[handle]->port_RTP,
        RTP_protocol);

    result = osip_message_set_body(invite_SIP_message, SDP_payload, strnlen(SDP_payload, 1500));
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(route);
        osip_free(SDP_payload);
        return result;
    }

    result = osip_message_set_content_type(invite_SIP_message, "application/sdp");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(route);
        osip_free(SDP_payload);
        return result;
    }

    eXosip_lock(global_client_configurations.exosip_context);
    global_client_configurations.live_video_context_pointer_array[handle]->call_id = eXosip_call_send_initial_invite(
        global_client_configurations.exosip_context,
        invite_SIP_message);
    if(global_client_configurations.live_video_context_pointer_array[handle]->call_id > 0)
    {
        eXosip_call_set_reference(
            global_client_configurations.exosip_context,
            global_client_configurations.live_video_context_pointer_array[handle]->call_id,
            global_client_configurations.live_video_context_pointer_array[handle]);
    }
    eXosip_unlock(global_client_configurations.exosip_context);

    osip_free(from);
    osip_free(to);
    osip_free(route);
    osip_free(SDP_payload);

    global_client_configurations.live_video_context_pointer_array[handle]->real_time_streaming = true;

    result = set_RTP_session_IP_version(
        global_client_configurations.live_video_context_pointer_array[handle]->session_handle,
        global_client_configurations.address_family);
    if(LIBRTP_OK != result)
    {
        // to do
    }

    result = set_RTP_session_local_IPv4(
        global_client_configurations.live_video_context_pointer_array[handle]->session_handle,
        global_client_configurations.client_IP);
    if(LIBRTP_OK != result)
    {
        // to do
    }

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_RTP_port(uint32_t handle, uint16_t port)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    int result = check_handle(handle);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }
    CHECK_NOT_STREAMING(global_client_configurations.live_video_context_pointer_array[handle]->real_time_streaming);

    global_client_configurations.live_video_context_pointer_array[handle]->port_RTP = port;
    result = set_RTP_session_local_port(
        global_client_configurations.live_video_context_pointer_array[handle]->session_handle,
        port);
    if(LIBRTP_OK != result)
    {
        // to do
    }

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_RTP_protocol(uint32_t handle, int protocol)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    int result = check_handle(handle);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }
    CHECK_NOT_STREAMING(global_client_configurations.live_video_context_pointer_array[handle]->real_time_streaming);

    global_client_configurations.live_video_context_pointer_array[handle]->protocol_RTP = protocol;
    result = set_RTP_session_IP_protocol(
        global_client_configurations.live_video_context_pointer_array[handle]->session_handle,
        protocol);
    if(LIBRTP_OK != result)
    {
        // to do
    }

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_close_real_time_stream(uint32_t handle)
{
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    int result = check_handle(handle);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }

    result = eXosip_lock(global_client_configurations.exosip_context);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }

    result = eXosip_call_terminate(
        global_client_configurations.exosip_context,
        global_client_configurations.live_video_context_pointer_array[handle]->call_id,
        global_client_configurations.live_video_context_pointer_array[handle]->dialog_id);

    eXosip_unlock(global_client_configurations.exosip_context);

    global_client_configurations.live_video_context_pointer_array[handle]->real_time_streaming = false;

    result = close_RTP_session(global_client_configurations.live_video_context_pointer_array[handle]->session_handle);
    if(LIBRTP_OK != result)
    {
        // to do
    }

    osip_free(global_client_configurations.live_video_context_pointer_array[handle]->target_IP);
    osip_free(global_client_configurations.live_video_context_pointer_array[handle]->target_sip_user_name);
    osip_free(global_client_configurations.live_video_context_pointer_array[handle]->local_IP);
    osip_free(global_client_configurations.live_video_context_pointer_array[handle]);

    global_client_configurations.live_video_context_pointer_array[handle] = NULL;

    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_set_RTP_payload_give_out_callback(uint32_t handle, void* cb)
{
    CHECK_NULL_PARAMETER(cb);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);
    int result = check_handle(handle);
    if(OSIP_SUCCESS != result)
    {
        return result;
    }
    CHECK_NOT_STREAMING(global_client_configurations.live_video_context_pointer_array[handle]->real_time_streaming);
    set_RTP_session_payload_give_out_callback(
        global_client_configurations.live_video_context_pointer_array[handle]->session_handle,
        cb);
    return OSIP_SUCCESS;
}

LIBGBT28181CLIENT_API int GBT28181_PTZ_control(char* target_sip_user_name, char* target_IP, uint16_t port, PTZ_control_data* p_PTZ_control_data)
{
    CHECK_NULL_PARAMETER(target_sip_user_name);
    CHECK_NULL_PARAMETER(p_PTZ_control_data);
    CHECK_INITIALED(global_client_configurations.initialed);
    CHECK_MUST_ON_LINE(global_client_configurations.online);

    if((1 == p_PTZ_control_data->up && 1 == p_PTZ_control_data->down) ||
       (1 == p_PTZ_control_data->left && 1 == p_PTZ_control_data->right) ||
       (1 == p_PTZ_control_data->zoom_in && 1 == p_PTZ_control_data->zoom_out))
    {
        return GBT28181_WRONG_PTZ_CMD;
    }

    int result = OSIP_SUCCESS;
    osip_message_t* query_catalog_message = NULL;
    char* from = osip_malloc(512);
    char* to = osip_malloc(512);
    char* proxy = osip_malloc(512);
    char* message_body = osip_malloc(1500);
    uint8_t cmd[8];
    char text_cmd[20];
    memset(&text_cmd, 0x0, 17);
    memset(&cmd, 0x0, sizeof(uint8_t) * 8);

    cmd[0] = 0xA5;
    cmd[1] = ((cmd[0] >> 4) + (cmd[0] & 0x0F)) % 16;
    cmd[2] = 0x00;
    if(1 == p_PTZ_control_data->up)
    {
        cmd[3] += 0x08;
    }
    if(1 == p_PTZ_control_data->down)
    {
        cmd[3] += 0x04;
    }
    if(1 == p_PTZ_control_data->left)
    {
        cmd[3] += 0x02;
    }
    if(1 == p_PTZ_control_data->right)
    {
        cmd[3] += 0x01;
    }
    if(1 == p_PTZ_control_data->zoom_in)
    {
        cmd[3] += 0x10;
    }
    if(1 == p_PTZ_control_data->zoom_out)
    {
        cmd[3] += 0x20;
    }
    cmd[4] = p_PTZ_control_data->relative_pan_speed;
    cmd[5] = p_PTZ_control_data->relative_tilt_speed;
    cmd[6] = p_PTZ_control_data->relative_zoom_speed << 4;
    cmd[7] = (cmd[0] + cmd[1] + cmd[2] + cmd[3] + cmd[4] + cmd[5] + cmd[6]) % 256;

    snprintf(text_cmd, 17, "%02X%02X%02X%02X%02X%02X%02X%02X", cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7]);
    text_cmd[17] = 0;

    if(NULL == from || NULL == to || NULL == proxy || NULL == message_body)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return GBT28181_NOMEM;
    }

    snprintf(
        from,
        512,
        "sip:%s@%s:%d",
        global_client_configurations.client_user_name,
        global_client_configurations.client_IP,
        global_client_configurations.client_port);
    snprintf(
        to,
        512,
        "sip:%s@%s",
        target_sip_user_name,
        global_client_configurations.server_domain);

    if(NULL != target_IP)
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%u>",
            target_sip_user_name,
            target_IP,
            port);
    }
    else
    {
        snprintf(
            proxy,
            512,
            "<sip:%s@%s:%u>",
            global_client_configurations.server_ID,
            global_client_configurations.server_IP,
            global_client_configurations.server_port);
    }

    result = eXosip_message_build_request(
        global_client_configurations.exosip_context,
        &query_catalog_message,
        "MESSAGE",
        to,
        from,
        proxy);
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    result = osip_message_set_content_type(query_catalog_message, "application/MANSCDP+xml");
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    snprintf(message_body, 1500, "<?xml version=\"1.0\"?><Control><CmdType>DeviceControl</CmdType><SN>%u</SN><DeviceID>%s</DeviceID><PTZCmd>%s</PTZCmd></Control>", global_client_configurations.MANSCDP_SN, target_sip_user_name, text_cmd);

    result = osip_message_set_body(query_catalog_message, message_body, strnlen(message_body, 1500));
    if(OSIP_SUCCESS != result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    eXosip_lock(global_client_configurations.exosip_context);
    result = eXosip_message_send_request(global_client_configurations.exosip_context, query_catalog_message);
    eXosip_unlock(global_client_configurations.exosip_context);
    if(0 > result)
    {
        osip_free(from);
        osip_free(to);
        osip_free(proxy);
        osip_free(message_body);
        return result;
    }

    EnterCriticalSection(&global_client_configurations.critical_section);
    global_client_configurations.MANSCDP_SN++;
    LeaveCriticalSection(&global_client_configurations.critical_section);

    osip_free(from);
    osip_free(to);
    osip_free(proxy);
    osip_free(message_body);

    return OSIP_SUCCESS;
}