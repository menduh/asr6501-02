/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifndef LINKWAN_H
#define LINKWAN_H

#include <stdbool.h>
#include <stdio.h>

#define LORAWAN_APP_DATA_BUFF_SIZE 242
#define LINKWAN_APP_DATA_SIZE 51
#define LORAWAN_CONFIRMED_MSG ENABLE
#define JOINREQ_NBTRIALS 3

typedef enum eJoinModeType
{
    JOIN_MODE_OTAA,
    JOIN_MODE_ABP
} JoinMode_t;
typedef enum node_mode_s {
    NODE_MODE_NORMAL = 0,
    NODE_MODE_REPEATER = 1,  // Alibaba Node Repeater
    NODE_MODE_AUTOSWITCH = 2,  // switch between normal and repeater
} node_mode_t;

typedef enum node_freq_mode_s {
    FREQ_MODE_INTRA = 1,  // uplink and downlink use same frequencies
    FREQ_MODE_INTER = 2,  // uplink and downlink use different frequencies
} node_freq_mode_t;

typedef enum valid_lora_config_s{
    VALID_LORA_CONFIG = 0xbeef,
    INVALID_LORA_CONFIG = 0xfffe,
}valid_lora_config;

typedef struct lora_config_s {
    uint32_t freqband;
    int8_t datarate;
    uint16_t flag;
} lora_config_t;

typedef struct lora_dev_s {
    uint8_t dev_eui[8];
    uint8_t app_eui[8];
    uint8_t app_key[16];
    int8_t freqband;
    int8_t datarate;
    int8_t class;
    struct classb_param {
        int8_t periodicity;
        uint32_t beacon_freq;
        int8_t beacon_dr;
        uint32_t pslot_freq;
        int8_t pslot_dr;
    }classb;
    uint8_t mode;  // normal or repeater
    uint16_t mask;
    uint16_t flag;
    uint16_t crc;
} __attribute__((packed)) lora_dev_t;
typedef struct lora_abp_id_s {
    uint32_t devaddr;
    uint8_t appskey[16];
    uint8_t nwkskey[16];
    uint16_t flag;
} lora_abp_id_t;

typedef enum join_method_s {
    STORED_JOIN_METHOD = 0,
    DEF_JOIN_METHOD = 1,
    SCAN_JOIN_METHOD = 2,
    JOIN_METHOD_NUM
} join_method_t;

#ifdef CONFIG_DEBUG_LINKWAN
#include "debug.h"
#define DBG_LINKWAN(...)     DBG_PRINTF(__VA_ARGS__)
#else
#define DBG_LINKWAN(...)
#define LORA_LOG(...)
#endif

typedef enum eTxEventType {
    TX_ON_TIMER,
    TX_ON_EVENT,
    TX_ON_NONE
} TxEventType_t;

typedef struct {
    uint8_t *Buff;
    uint8_t BuffSize;
    uint8_t Port;
} lora_AppData_t;

typedef struct sLoRaMainCallback {
    uint8_t (*BoardGetBatteryLevel)(void);
    void (*BoardGetUniqueId)(uint8_t *id);
    uint32_t (*BoardGetRandomSeed)(void);
    void (*LoraTxData)(lora_AppData_t *AppData);
    void (*LoraRxData)(lora_AppData_t *AppData);
} LoRaMainCallback_t;

typedef struct sLoRaParam {
    TxEventType_t TxEvent;
    uint32_t TxDutyCycleTime;
    bool AdrEnable;
    int8_t TxDatarate;
    bool EnablePublicNetwork;
    uint8_t NbTrials;
    JoinMode_t JoinMode;
} LoRaParam_t;

typedef enum eDevicState {
    DEVICE_STATE_INIT,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_JOINED,
    DEVICE_STATE_SEND,
    DEVICE_STATE_SEND_MAC,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP,
    DEVICE_STATE_REQ_DEVICE_TIME,
    DEVICE_STATE_BEACON_ACQUISITION,
    DEVICE_STATE_REQ_PINGSLOT_ACK
} DeviceState_t;

typedef enum eDeviceStatus {
    DEVICE_STATUS_IDLE,//0
    DEVICE_STATUS_SENDING,
    DEVICE_STATUS_SEND_FAIL,
    DEVICE_STATUS_SEND_PASS,
    DEVICE_STATUS_JOIN_PASS,
    DEVICE_STATUS_JOIN_FAIL,
    DEVICE_STATUS_NETWORK_ABNORMAL,
    DEVICE_STATUS_SEND_PASS_WITHOUT_DL,
    DEVICE_STATUS_SEND_PASS_WITH_DL,
} DeviceStatus_t;

void lora_init(LoRaMainCallback_t *callbacks);
void lora_fsm( void );

bool set_lora_freqband_mask(uint16_t mask);
uint16_t get_lora_freqband_mask(void);

bool set_lora_dev_eui(uint8_t *eui);
bool set_lora_app_eui(uint8_t *eui);
bool set_lora_app_key(uint8_t *key);

node_freq_mode_t get_lora_freq_mode(void);
bool set_lora_freq_mode(node_freq_mode_t mode);
bool set_lora_tx_datarate(int8_t datarate);
int8_t get_lora_tx_datarate(void);

bool set_lora_adr(int state);
int get_lora_adr(void);

bool set_lora_class(int8_t class, int8_t branch, uint32_t param1, uint32_t param2, uint32_t param3, uint32_t param4);
int8_t get_lora_class(void);

bool set_lora_tx_cfm_flag(int confirmed);
int get_lora_tx_cfm_flag(void);

bool set_lora_tx_cfm_trials(uint8_t trials);
uint8_t get_lora_tx_cfm_trials(void);

bool set_lora_state(DeviceState_t state);

bool set_lora_tx_dutycycle(uint32_t dutycycle);
uint32_t get_lora_tx_dutycycle(void);

lora_AppData_t *get_lora_tx_data(void);
bool tx_lora_data(void);
lora_AppData_t *get_lora_rx_data(void);
void tx_lora_mac_req(void);

int get_device_status(void);

// for linkWAN 
JoinMode_t get_lora_join_mode(void);
bool set_lora_join_mode(JoinMode_t mode);

bool set_lora_tx_len(uint16_t len);
uint8_t get_lora_tx_len(void);

bool send_lora_link_check(void);

uint8_t get_lora_tx_pwr(void);
bool set_lora_tx_pwr(uint8_t tx_power);

bool set_lora_debug(bool debug);
bool get_lora_debug(void);

bool set_lora_active_mode(bool active_mode);
bool get_lora_active_mode(void);

uint32_t get_lora_devaddr(void);
bool set_lora_devaddr(uint32_t devaddr);

bool set_lora_dev_eui(uint8_t *eui);
uint8_t *get_lora_dev_eui(void);

bool set_lora_app_eui(uint8_t *eui);
uint8_t *get_lora_app_eui(void);

bool set_lora_app_key(uint8_t *key);
uint8_t *get_lora_app_key(void);

uint8_t *get_lora_appskey(void);
bool set_lora_appskey(uint8_t *buf);

uint8_t *get_lora_nwkskey(void);
bool set_lora_nwkskey(uint8_t *buf);
uint8_t get_lora_app_port(void);
bool set_lora_app_port(uint8_t port);

bool lora_del_multicast(uint32_t dev_addr);
uint8_t get_lora_mulitcast_num(void);

uint8_t get_device_battery();
bool get_lora_join_params(uint8_t *bJoin, uint8_t *bAuto, uint16_t *joinInterval, uint16_t *joinRetryCnt);

bool init_lora_join(uint8_t bJoin, uint8_t bAutoJoin, uint16_t joinInterval, uint16_t joinRetryCnt);

void get_lora_rssi(uint8_t band, int16_t *channel_rssi);
DeviceStatus_t get_lora_device_status(void);
bool set_lora_device_status(DeviceStatus_t ds);

bool lora_tx_data_payload(uint8_t confirm, uint8_t Nbtrials, uint8_t *payload, uint8_t len);

bool get_lora_report_mode(void);
bool set_lora_report_mode(int8_t mode);

int8_t get_lora_tx_power(void);
bool set_lora_tx_power(int8_t pwr);

bool set_lora_rx_window_params(uint8_t RX1DRoffset, uint8_t RX2DR, uint32_t RX2Freq);
bool set_lora_mac_rx1_delay(long rx1delay);

void lora_reboot(int8_t mode);

bool is_at_available();

bool lora_device_time_req();

bool lora_pingSlotInfoReq(uint8_t periodicity);

uint8_t lora_getPingSlotPeriodicity();
#endif /* LINKWAN_H */
