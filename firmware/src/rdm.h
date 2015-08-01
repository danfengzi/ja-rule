/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * rdm.h
 * Copyright (C) 2015 Simon Newton
 */

/**
 * @defgroup rdm RDM
 * @brief Remote Device Management
 *
 * @addtogroup rdm
 * @{
 * @file rdm.h
 * @brief Remote Device Management
 */

#ifndef FIRMWARE_SRC_RDM_H_
#define FIRMWARE_SRC_RDM_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The root sub device.
 */
static const uint16_t SUBDEVICE_ROOT = 0u;

/**
 * @brief All sub devices.
 */
static const uint16_t SUBDEVICE_ALL = 0xffffu;

/**
 * @brief The maximum index for a sub device.
 */
static const uint16_t SUBDEVICE_MAX = 0x0200u;

/**
 * @brief The maximum number of product detail ids a responder can have.
 */
enum { MAX_PRODUCT_DETAILS = 6 };

/**
 * @brief The RDM Sub Start Code.
 */
static const uint8_t SUB_START_CODE = 0x01u;

/**
 * @brief The number of bytes in an RDM checksum
 */
static const uint8_t RDM_CHECKSUM_LENGTH = 2u;

/**
 * @brief The location of the message length field in a frame.
 */
static const uint8_t MESSAGE_LENGTH_OFFSET = 2u;

/**
 * @brief The location of the parameter data length in a frame.
 */
static const uint8_t RDM_PARAM_DATA_LENGTH_OFFSET = 23u;

/**
 * @brief The location of the parameter data in a frame.
 */
static const uint8_t RDM_PARAM_DATA_OFFSET = 24u;

/**
 * @brief The RDM version we support.
 */
static const uint16_t RDM_VERSION = 0x0100u;

/**
 * @brief The maximum value for a DMX start address.
 */
static const uint16_t MAX_DMX_START_ADDRESS = 512u;

/**
 * @brief The 'unpatched' DMX start address.
 */
static const uint16_t INVALID_DMX_START_ADDRESS = 0xffffu;

/**
 * @brief Indicates the responder is a managed proxy.
 */
static const uint16_t MUTE_MANAGED_PROXY_FLAG = 0x01;

/**
 * @brief Indicates the responder has subdevices.
 */
static const uint16_t MUTE_SUBDEVICE_FLAG = 0x02;

/**
 * @brief Indicates the responder is in bootloader mode.
 */
static const uint16_t MUTE_BOOTLOADER_FLAG = 0x04;

/**
 * @brief Indicates the responder is a proxy.
 */
static const uint16_t MUTE_PROXY_FLAG = 0x08;

/**
 * @brief The size of the default strings used in RDM.
 */
enum { RDM_DEFAULT_STRING_SIZE = 32 };

/**
 * @brief The size of a DUB response.
 */
enum { DUB_RESPONSE_LENGTH = 24 };

/**
 * @brief The size of an RDM frame.
 */
enum { RDM_MAX_FRAME_SIZE = 257 };

/**
 * @brief The length of a UID.
 */
enum {
  UID_LENGTH = 6  //!< The size of a UID.
};

/**
 * @brief Maximum size of RDM Parameter Data.
 */
enum {
  MAX_PARAM_DATA_SIZE = 231  //!< Maximum size of RDM Parameter Data.
};

/**
 * @brief The recording mask for the recorded value support bit field.
 */
enum { SENSOR_SUPPORTS_RECORDING_MASK = 0x01 };

/**
 * @brief The lowest / highest mask for the recorded value support bit field.
 */
enum { SENSOR_SUPPORTS_LOWEST_HIGHEST_MASK = 0x02 };

/**
 * @brief The all-sensors value.
 */
static const uint8_t ALL_SENSORS = 0xffu;

/**
 * @brief Indicates that recorded / highest / lowest values aren't supported.
 */
static const uint16_t SENSOR_VALUE_UNSUPPORTED = 0u;

/*
 * @brief The maximum number of status messages per frame.
 */
static const unsigned int MAX_STATUS_MESSAGES_PER_FRAME = 21u;

/*
 * @brief The maximum number of slot info entries in a RDM frame.
 */
static const unsigned int MAX_SLOT_INFO_PER_FRAME = 46u;

/**
 * @brief The maximum number of default slot value entries in a RDM frame.
 */
static const unsigned int MAX_DEFAULT_SLOT_VALUE_PER_FRAME = 77u;

/**
 * @brief The maximum pin code from E1.37-1.
 */
static const uint16_t MAX_PIN_CODE = 9999u;

/**
 * @brief The size of the DNS Host Name field from E1.37-2.
 */
enum { DNS_HOST_NAME_SIZE = 63 };

/**
 * @brief The size of the DNS Domain Name field from E1.37-2.
 */
enum { DNS_DOMAIN_NAME_SIZE = 231 };

/**
 * @brief Interface hardware types.
 */
enum {
  ETHERNET_HARDWARE_TYPE = 1,  //!< Ethernet
  IPSEC_HARDWARE_TYPE = 31  //!< IPSEC
};

/**
 * @brief Indicates no route is assigned.
 */
static const uint32_t NO_DEFAULT_ROUTE = 0u;

/**
 * @brief Indicates no IP address is configured.
 */
static const uint32_t IPV4_UNCONFIGURED = 0u;

/**
 * @brief The maximum netmask value.
 */
static const uint8_t MAX_NETMASK = 32u;

/**
 * @brief The size of a MAC address
 */
enum { MAC_ADDRESS_SIZE = 6 };

/**
 * @brief DHCP Status
 */
typedef enum {
  DHCP_STATUS_INACTIVE = 0x00,
  DHCP_STATUS_ACTIVE = 0x01,
  DHCP_STATUS_UNKNOWN = 0x02
} DHCPMode;

/**
 * @brief RDM Command Classes from E1.20.
 * @note See section 6.2.10 of ANSI E1.20 for more information.
 */
typedef enum {
  DISCOVERY_COMMAND = 0x10, /**< Discovery Command */
  DISCOVERY_COMMAND_RESPONSE = 0x11, /**< Discovery Response */
  GET_COMMAND = 0x20, /**< Get Command */
  GET_COMMAND_RESPONSE = 0x21, /**< Get Response */
  SET_COMMAND = 0x30, /**< Set Command */
  SET_COMMAND_RESPONSE = 0x31, /**< Set Response */
} RDMCommandClass;

/**
 * @brief RDM Command Classes from E1.20.
 * @note See section 6.2.10 of ANSI E1.20 for more information.
 */
typedef enum {
  ACK = 0x00, /**< ACK */
  ACK_TIMER = 0x01, /**< ACK Timer */
  NACK_REASON = 0x02, /**< NACK with reason */
  ACK_OVERFLOW = 0x03 /**< ACK OVERFLOW */
} RDMResponseType;

/**
 * @brief RDM Parameter IDs.
 * @note These come from E1.20, E1.37-1, E1.37-2 etc.
 */
typedef enum {
  // Discovery
  PID_DISC_UNIQUE_BRANCH = 0x0001,
  PID_DISC_MUTE = 0x0002,
  PID_DISC_UN_MUTE = 0x0003,
  // network management
  PID_PROXIED_DEVICES = 0x0010,
  PID_PROXIED_DEVICE_COUNT = 0x0011,
  PID_COMMS_STATUS = 0x0015,
  // status collection
  PID_QUEUED_MESSAGE = 0x0020,
  PID_STATUS_MESSAGES = 0x0030,
  PID_STATUS_ID_DESCRIPTION = 0x0031,
  PID_CLEAR_STATUS_ID = 0x0032,
  PID_SUB_DEVICE_STATUS_REPORT_THRESHOLD = 0x0033,
  // RDM information
  PID_SUPPORTED_PARAMETERS = 0x0050,
  PID_PARAMETER_DESCRIPTION = 0x0051,
  // production information
  PID_DEVICE_INFO = 0x0060,
  PID_PRODUCT_DETAIL_ID_LIST = 0x0070,
  PID_DEVICE_MODEL_DESCRIPTION = 0x0080,
  PID_MANUFACTURER_LABEL = 0x0081,
  PID_DEVICE_LABEL = 0x0082,
  PID_FACTORY_DEFAULTS = 0x0090,
  PID_LANGUAGE_CAPABILITIES = 0x00a0,
  PID_LANGUAGE = 0x00b0,
  PID_SOFTWARE_VERSION_LABEL = 0x00c0,
  PID_BOOT_SOFTWARE_VERSION_ID = 0x00c1,
  PID_BOOT_SOFTWARE_VERSION_LABEL = 0x00c2,
  // DMX512
  PID_DMX_PERSONALITY = 0x00e0,
  PID_DMX_PERSONALITY_DESCRIPTION = 0x00e1,
  PID_DMX_START_ADDRESS = 0x00f0,
  PID_SLOT_INFO = 0x0120,
  PID_SLOT_DESCRIPTION = 0x0121,
  PID_DEFAULT_SLOT_VALUE = 0x0122,
  // sensors
  PID_SENSOR_DEFINITION = 0x0200,
  PID_SENSOR_VALUE = 0x0201,
  PID_RECORD_SENSORS = 0x0202,
  // power/lamp settings
  PID_DEVICE_HOURS = 0x0400,
  PID_LAMP_HOURS = 0x0401,
  PID_LAMP_STRIKES = 0x0402,
  PID_LAMP_STATE = 0x0403,
  PID_LAMP_ON_MODE = 0x0404,
  PID_DEVICE_POWER_CYCLES = 0x0405,
  // display settings
  PID_DISPLAY_INVERT = 0x0500,
  PID_DISPLAY_LEVEL = 0x0501,
  // configuration
  PID_PAN_INVERT = 0x0600,
  PID_TILT_INVERT = 0x0601,
  PID_PAN_TILT_SWAP = 0x0602,
  PID_REAL_TIME_CLOCK = 0x0603,
  // control
  PID_IDENTIFY_DEVICE = 0x1000,
  PID_RESET_DEVICE = 0x1001,
  PID_POWER_STATE = 0x1010,
  PID_PERFORM_SELFTEST = 0x1020,
  PID_SELF_TEST_DESCRIPTION = 0x1021,
  PID_CAPTURE_PRESET = 0x1030,
  PID_PRESET_PLAYBACK = 0x1031,

  // E1.37-1 PIDS
  // DMX512 setup
  PID_DMX_BLOCK_ADDRESS = 0x0140,
  PID_DMX_FAIL_MODE = 0x0141,
  PID_DMX_STARTUP_MODE = 0x0142,

  // Dimmer Settings
  PID_DIMMER_INFO = 0x0340,
  PID_MINIMUM_LEVEL = 0x0341,
  PID_MAXIMUM_LEVEL = 0x0342,
  PID_CURVE = 0x0343,
  PID_CURVE_DESCRIPTION = 0x0344,

  // Control
  PID_OUTPUT_RESPONSE_TIME = 0x0345,
  PID_OUTPUT_RESPONSE_TIME_DESCRIPTION = 0x0346,
  PID_MODULATION_FREQUENCY = 0x0347,
  PID_MODULATION_FREQUENCY_DESCRIPTION = 0x0348,

  // Power/Lamp Settings
  PID_BURN_IN = 0x0440,

  // Configuration
  PID_LOCK_PIN = 0x0640,
  PID_LOCK_STATE = 0x0641,
  PID_LOCK_STATE_DESCRIPTION = 0x0642,
  PID_IDENTIFY_MODE = 0x1040,
  PID_PRESET_INFO = 0x1041,
  PID_PRESET_STATUS = 0x1042,
  PID_PRESET_MERGEMODE = 0x1043,
  PID_POWER_ON_SELF_TEST = 0x1044,

  // E1.37-2 PIDs
  PID_LIST_INTERFACES = 0x0700,
  PID_INTERFACE_LABEL = 0x0701,
  PID_INTERFACE_HARDWARE_ADDRESS_TYPE1 = 0x0702,
  PID_IPV4_DHCP_MODE = 0x0703,
  PID_IPV4_ZEROCONF_MODE = 0x0704,
  PID_IPV4_CURRENT_ADDRESS = 0x0705,
  PID_IPV4_STATIC_ADDRESS = 0x0706,
  PID_INTERFACE_RENEW_DHCP = 0x0707,
  PID_INTERFACE_RELEASE_DHCP = 0x0708,
  PID_INTERFACE_APPLY_CONFIGURATION = 0x0709,
  PID_IPV4_DEFAULT_ROUTE = 0x070a,
  PID_DNS_NAME_SERVER = 0x070b,
  PID_DNS_HOSTNAME = 0x070c,
  PID_DNS_DOMAIN_NAME = 0x070d,
} RDMPid;


/**
 * @brief Open Lighting Project manufacturer PIDs.
 */
typedef enum {
  PID_DEVICE_MODEL = 0x8002,
  PID_DEVICE_MODEL_LIST = 0x8003
} OpenLightingManufacturerPID;

/**
 * @brief RDM NACK reason codes from E1.20.
 * @note See Table A-17 from ANSI E1.20 for more information.
 */
typedef enum {
  NR_UNKNOWN_PID = 0x0000,
  NR_FORMAT_ERROR = 0x0001,
  NR_HARDWARE_FAULT = 0x0002,
  NR_PROXY_REJECT = 0x0003,
  NR_WRITE_PROTECT = 0x0004,
  NR_UNSUPPORTED_COMMAND_CLASS = 0x0005,
  NR_DATA_OUT_OF_RANGE = 0x0006,
  NR_BUFFER_FULL = 0x0007,
  NR_PACKET_SIZE_UNSUPPORTED = 0x0008,
  NR_SUB_DEVICE_OUT_OF_RANGE = 0x0009,
  NR_PROXY_BUFFER_FULL = 0x000a,
  NR_ACTION_NOT_SUPPORTED = 0x000b,
  NR_ENDPOINT_NUMBER_INVALID = 0x0011
} RDMNackReason;

typedef enum {
  STATUS_NONE = 0x0,
  STATUS_GET_LAST_MESSAGE = 0x1,
  STATUS_ADVISORY = 0x2,
  STATUS_WARNING = 0x3,
  STATUS_ERROR = 0x4,
  STATUS_ADVISORY_CLEARED = 0x12,
  STATUS_WARNING_CLEARED = 0x13,
  STATUS_ERROR_CLEARED = 0x14,
} RDMStatusType;

/**
 * @brief RDM Product Category codes from E1.20.
 * @note See Table A-5 from ANSI E1.20 for more information.
 */
typedef enum {
  PRODUCT_CATEGORY_NOT_DECLARED = 0x0000,
  PRODUCT_CATEGORY_FIXTURE = 0x0100,
  PRODUCT_CATEGORY_FIXTURE_FIXED = 0x0101,
  PRODUCT_CATEGORY_FIXTURE_MOVING_YOKE = 0x0102,
  PRODUCT_CATEGORY_FIXTURE_MOVING_MIRROR = 0x0103,
  PRODUCT_CATEGORY_FIXTURE_OTHER = 0x01ff,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY = 0x0200,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_COLOR = 0x0201,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_YOKE = 0x0202,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_MIRROR = 0x0203,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_EFFECT = 0x0204,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_BEAM = 0x0205,
  PRODUCT_CATEGORY_FIXTURE_ACCESSORY_OTHER = 0x02ff,
  PRODUCT_CATEGORY_PROJECTOR = 0x0300,
  PRODUCT_CATEGORY_PROJECTOR_FIXED = 0x0301,
  PRODUCT_CATEGORY_PROJECTOR_MOVING_YOKE = 0x0302,
  PRODUCT_CATEGORY_PROJECTOR_MOVING_MIRROR = 0x0303,
  PRODUCT_CATEGORY_PROJECTOR_OTHER = 0x03ff,
  PRODUCT_CATEGORY_ATMOSPHERIC = 0x0400,
  PRODUCT_CATEGORY_ATMOSPHERIC_EFFECT = 0x0401,
  PRODUCT_CATEGORY_ATMOSPHERIC_PYRO = 0x0402,
  PRODUCT_CATEGORY_ATMOSPHERIC_OTHER = 0x04ff,
  PRODUCT_CATEGORY_DIMMER = 0x0500,
  PRODUCT_CATEGORY_DIMMER_AC_INCANDESCENT = 0x0501,
  PRODUCT_CATEGORY_DIMMER_AC_FLUORESCENT = 0x0502,
  PRODUCT_CATEGORY_DIMMER_AC_COLDCATHODE = 0x0503,
  PRODUCT_CATEGORY_DIMMER_AC_NONDIM = 0x0504,
  PRODUCT_CATEGORY_DIMMER_AC_ELV = 0x0505,
  PRODUCT_CATEGORY_DIMMER_AC_OTHER = 0x0506,
  PRODUCT_CATEGORY_DIMMER_DC_LEVEL = 0x0507,
  PRODUCT_CATEGORY_DIMMER_DC_PWM = 0x0508,
  PRODUCT_CATEGORY_DIMMER_CS_LED = 0x0509,
  PRODUCT_CATEGORY_DIMMER_OTHER = 0x05ff,
  PRODUCT_CATEGORY_POWER = 0x0600,
  PRODUCT_CATEGORY_POWER_CONTROL = 0x0601,
  PRODUCT_CATEGORY_POWER_SOURCE = 0x0602,
  PRODUCT_CATEGORY_POWER_OTHER = 0x06ff,
  PRODUCT_CATEGORY_SCENIC = 0x0700,
  PRODUCT_CATEGORY_SCENIC_DRIVE = 0x0701,
  PRODUCT_CATEGORY_SCENIC_OTHER = 0x07ff,
  PRODUCT_CATEGORY_DATA = 0x0800,
  PRODUCT_CATEGORY_DATA_DISTRIBUTION = 0x0801,
  PRODUCT_CATEGORY_DATA_CONVERSION = 0x0802,
  PRODUCT_CATEGORY_DATA_OTHER = 0x08ff,
  PRODUCT_CATEGORY_AV = 0x0900,
  PRODUCT_CATEGORY_AV_AUDIO = 0x0901,
  PRODUCT_CATEGORY_AV_VIDEO = 0x0902,
  PRODUCT_CATEGORY_AV_OTHER = 0x09ff,
  PRODUCT_CATEGORY_MONITOR = 0x0a00,
  PRODUCT_CATEGORY_MONITOR_ACLINEPOWER = 0x0a01,
  PRODUCT_CATEGORY_MONITOR_DCPOWER = 0x0a02,
  PRODUCT_CATEGORY_MONITOR_ENVIRONMENTAL = 0x0a03,
  PRODUCT_CATEGORY_MONITOR_OTHER = 0x0aff,
  PRODUCT_CATEGORY_CONTROL = 0x7000,
  PRODUCT_CATEGORY_CONTROL_CONTROLLER = 0x7001,
  PRODUCT_CATEGORY_CONTROL_BACKUPDEVICE = 0x7002,
  PRODUCT_CATEGORY_CONTROL_OTHER = 0x70ff,
  PRODUCT_CATEGORY_TEST = 0x7100,
  PRODUCT_CATEGORY_TEST_EQUIPMENT = 0x7101,
  PRODUCT_CATEGORY_TEST_EQUIPMENT_OTHER = 0x71ff,
  PRODUCT_CATEGORY_OTHER = 0x7fff,
} __attribute__((packed)) RDMProductCategory;

// product details
typedef enum {
  PRODUCT_DETAIL_NOT_DECLARED = 0x0000,
  PRODUCT_DETAIL_ARC = 0x0001,
  PRODUCT_DETAIL_METAL_HALIDE = 0x0002,
  PRODUCT_DETAIL_INCANDESCENT = 0x0003,
  PRODUCT_DETAIL_LED = 0x0004,
  PRODUCT_DETAIL_FLUORESCENT = 0x0005,
  PRODUCT_DETAIL_COLDCATHODE = 0x0006,
  PRODUCT_DETAIL_ELECTROLUMINESCENT = 0x0007,
  PRODUCT_DETAIL_LASER = 0x0008,
  PRODUCT_DETAIL_FLASHTUBE = 0x0009,
  PRODUCT_DETAIL_COLORSCROLLER = 0x0100,
  PRODUCT_DETAIL_COLORWHEEL = 0x0101,
  PRODUCT_DETAIL_COLORCHANGE = 0x0102,
  PRODUCT_DETAIL_IRIS_DOUSER = 0x0103,
  PRODUCT_DETAIL_DIMMING_SHUTTER = 0x0104,
  PRODUCT_DETAIL_PROFILE_SHUTTER = 0x0105,
  PRODUCT_DETAIL_BARNDOOR_SHUTTER = 0x0106,
  PRODUCT_DETAIL_EFFECTS_DISC = 0x0107,
  PRODUCT_DETAIL_GOBO_ROTATOR = 0x0108,
  PRODUCT_DETAIL_VIDEO = 0x0200,
  PRODUCT_DETAIL_SLIDE = 0x0201,
  PRODUCT_DETAIL_FILM = 0x0202,
  PRODUCT_DETAIL_OILWHEEL = 0x0203,
  PRODUCT_DETAIL_LCDGATE = 0x0204,
  PRODUCT_DETAIL_FOGGER_GLYCOL = 0x0300,
  PRODUCT_DETAIL_FOGGER_MINERALOIL = 0x0301,
  PRODUCT_DETAIL_FOGGER_WATER = 0x0302,
  PRODUCT_DETAIL_CO2 = 0x0303,
  PRODUCT_DETAIL_LN2 = 0x0304,
  PRODUCT_DETAIL_BUBBLE = 0x0305,
  PRODUCT_DETAIL_FLAME_PROPANE = 0x0306,
  PRODUCT_DETAIL_FLAME_OTHER = 0x0307,
  PRODUCT_DETAIL_OLEFACTORY_STIMULATOR = 0x0308,
  PRODUCT_DETAIL_SNOW = 0x0309,
  PRODUCT_DETAIL_WATER_JET = 0x030a,
  PRODUCT_DETAIL_WIND = 0x030b,
  PRODUCT_DETAIL_CONFETTI = 0x030c,
  PRODUCT_DETAIL_HAZARD = 0x030d,
  PRODUCT_DETAIL_PHASE_CONTROL = 0x0400,
  PRODUCT_DETAIL_REVERSE_PHASE_CONTROL = 0x0401,
  PRODUCT_DETAIL_SINE = 0x0402,
  PRODUCT_DETAIL_PWM = 0x0403,
  PRODUCT_DETAIL_DC = 0x0404,
  PRODUCT_DETAIL_HFBALLAST = 0x0405,
  PRODUCT_DETAIL_HFHV_NEONBALLAST = 0x0406,
  PRODUCT_DETAIL_HFHV_EL = 0x0407,
  PRODUCT_DETAIL_MHR_BALLAST = 0x0408,
  PRODUCT_DETAIL_BITANGLE_MODULATION = 0x0409,
  PRODUCT_DETAIL_FREQUENCY_MODULATION = 0x040a,
  PRODUCT_DETAIL_HIGHFREQUENCY_12V = 0x040b,
  PRODUCT_DETAIL_RELAY_MECHANICAL = 0x040c,
  PRODUCT_DETAIL_RELAY_ELECTRONIC = 0x040d,
  PRODUCT_DETAIL_SWITCH_ELECTRONIC = 0x040e,
  PRODUCT_DETAIL_CONTACTOR = 0x040f,
  PRODUCT_DETAIL_MIRRORBALL_ROTATOR = 0x0500,
  PRODUCT_DETAIL_OTHER_ROTATOR = 0x0501,
  PRODUCT_DETAIL_KABUKI_DROP = 0x0502,
  PRODUCT_DETAIL_CURTAIN = 0x0503,
  PRODUCT_DETAIL_LINESET = 0x0504,
  PRODUCT_DETAIL_MOTOR_CONTROL = 0x0505,
  PRODUCT_DETAIL_DAMPER_CONTROL = 0x0506,
  PRODUCT_DETAIL_SPLITTER = 0x0600,
  PRODUCT_DETAIL_ETHERNET_NODE = 0x0601,
  PRODUCT_DETAIL_MERGE = 0x0602,
  PRODUCT_DETAIL_DATAPATCH = 0x0603,
  PRODUCT_DETAIL_WIRELESS_LINK = 0x0604,
  PRODUCT_DETAIL_PROTOCOL_CONVERTOR = 0x0701,
  PRODUCT_DETAIL_ANALOG_DEMULTIPLEX = 0x0702,
  PRODUCT_DETAIL_ANALOG_MULTIPLEX = 0x0703,
  PRODUCT_DETAIL_SWITCH_PANEL = 0x0704,
  PRODUCT_DETAIL_ROUTER = 0x0800,
  PRODUCT_DETAIL_FADER = 0x0801,
  PRODUCT_DETAIL_MIXER = 0x0802,
  PRODUCT_DETAIL_CHANGEOVER_MANUAL = 0x0900,
  PRODUCT_DETAIL_CHANGEOVER_AUTO = 0x0901,
  PRODUCT_DETAIL_TEST = 0x0902,
  PRODUCT_DETAIL_GFI_RCD = 0x0a00,
  PRODUCT_DETAIL_BATTERY = 0x0a01,
  PRODUCT_DETAIL_CONTROLLABLE_BREAKER = 0x0a02,
  PRODUCT_DETAIL_OTHER = 0x7fff,
} __attribute__((packed)) RDMProductDetail;

typedef enum {
  LAMP_OFF = 0x00,
  LAMP_ON = 0x01,
  LAMP_STRIKE = 0x02,
  LAMP_STANDBY = 0x03,
  LAMP_NOT_PRESENT = 0x04,
  LAMP_ERROR = 0x7f,
} RDMLampState;

typedef enum {
  LAMP_ON_MODE_OFF = 0x00,
  LAMP_ON_MODE_DMX = 0x01,
  LAMP_ON_MODE_ON = 0x02,
  LAMP_ON_MODE_ON_AFTER_CAL = 0x03,
} RDMLampMode;

typedef enum {
  DISPLAY_INVERT_OFF = 0x00,
  DISPLAY_INVERT_ON = 0x01,
  DISPLAY_INVERT_AUTO = 0x02,
} RDMDisplayInvert;

typedef enum {
  POWER_STATE_FULL_OFF = 0x00,
  POWER_STATE_SHUTDOWN = 0x01,
  POWER_STATE_STANDBY = 0x02,
  POWER_STATE_NORMAL = 0xff,
} RDMPowerState;

typedef enum {
  SENSOR_TEMPERATURE = 0x00,
  SENSOR_VOLTAGE = 0x01,
  SENSOR_CURRENT = 0x02,
  SENSOR_FREQUENCY = 0x03,
  SENSOR_RESISTANCE = 0x04,
  SENSOR_POWER = 0x05,
  SENSOR_MASS = 0x06,
  SENSOR_LENGTH = 0x07,
  SENSOR_AREA = 0x08,
  SENSOR_VOLUME = 0x09,
  SENSOR_DENSITY = 0x0a,
  SENSOR_VELOCITY = 0x0b,
  SENSOR_ACCELERATION = 0x0c,
  SENSOR_FORCE = 0x0d,
  SENSOR_ENERGY = 0x0e,
  SENSOR_PRESSURE = 0x0f,
  SENSOR_TIME = 0x10,
  SENSOR_ANGLE = 0x11,
  SENSOR_POSITION_X = 0x12,
  SENSOR_POSITION_Y = 0x13,
  SENSOR_POSITION_Z = 0x14,
  SENSOR_ANGULAR_VELOCITY = 0x15,
  SENSOR_LUMINOUS_INTENSITY = 0x16,
  SENSOR_LUMINOUS_FLUX = 0x17,
  SENSOR_ILLUMINANCE = 0x18,
  SENSOR_CHROMINANCE_RED = 0x19,
  SENSOR_CHROMINANCE_GREEN = 0x1a,
  SENSOR_CHROMINANCE_BLUE = 0x1b,
  SENSOR_CONTACTS = 0x1c,
  SENSOR_MEMORY = 0x1d,
  SENSOR_ITEMS = 0x1e,
  SENSOR_HUMIDITY = 0x1f,
  SENSOR_COUNTER_16BIT = 0x20,
  SENSOR_OTHER = 0x7f
} __attribute__((packed)) RDMSensorType;

typedef enum {
  UNITS_NONE = 0x00,
  UNITS_CENTIGRADE = 0x01,
  UNITS_VOLTS_DC = 0x02,
  UNITS_VOLTS_AC_PEAK = 0x03,
  UNITS_VOLTS_AC_RMS = 0x04,
  UNITS_AMPERE_DC = 0x05,
  UNITS_AMPERE_AC_PEAK = 0x06,
  UNITS_AMPERE_AC_RMS = 0x07,
  UNITS_HERTZ = 0x08,
  UNITS_OHM = 0x09,
  UNITS_WATT = 0x0a,
  UNITS_KILOGRAM = 0x0b,
  UNITS_METERS = 0x0c,
  UNITS_METERS_SQUARED = 0x0d,
  UNITS_METERS_CUBED = 0x0e,
  UNITS_KILOGRAMMES_PER_METER_CUBED = 0x0f,
  UNITS_METERS_PER_SECOND = 0x10,
  UNITS_METERS_PER_SECOND_SQUARED = 0x11,
  UNITS_NEWTON = 0x12,
  UNITS_JOULE = 0x13,
  UNITS_PASCAL = 0x14,
  UNITS_SECOND = 0x15,
  UNITS_DEGREE = 0x16,
  UNITS_STERADIAN = 0x17,
  UNITS_CANDELA = 0x18,
  UNITS_LUMEN = 0x19,
  UNITS_LUX = 0x1a,
  UNITS_IRE = 0x1b,
  UNITS_BYTE = 0x1c,
} __attribute__((packed)) RDMUnit;

typedef enum {
  PREFIX_NONE = 0x00,
  PREFIX_DECI = 0x01,
  PREFIX_CENTI = 0x02,
  PREFIX_MILLI = 0x03,
  PREFIX_MICRO = 0x04,
  PREFIX_NANO = 0x05,
  PREFIX_PICO = 0x06,
  PREFIX_FEMPTO = 0x07,
  PREFIX_ATTO = 0x08,
  PREFIX_ZEPTO = 0x09,
  PREFIX_YOCTO = 0x0a,
  PREFIX_DECA = 0x11,
  PREFIX_HECTO = 0x12,
  PREFIX_KILO = 0x13,
  PREFIX_MEGA = 0x14,
  PREFIX_GIGA = 0x15,
  PREFIX_TERRA = 0x16,
  PREFIX_PETA = 0x17,
  PREFIX_EXA = 0x18,
  PREFIX_ZETTA = 0x19,
  PREFIX_YOTTA = 0x1a,
} __attribute__((packed)) RDMPrefix;

/**
 * @brief The RDM slot types, from table C-1 of the standard.
 */
typedef enum {
  ST_PRIMARY = 0x00,
  ST_SEC_FINE = 0x01,
  ST_SEC_TIMING = 0x02,
  ST_SEC_SPEED = 0x03,
  ST_SEC_CONTROL = 0x04,
  ST_SEC_INDEX = 0x05,
  ST_SEC_ROTATION = 0x06,
  ST_SEC_INDEX_ROTATE = 0x07,
  ST_SEC_UNDEFINED = 0xff,
} RDMSlotType;

typedef enum {
  SD_INTENSITY = 0x0001,
  SD_INTENSITY_MASTER = 0x0002,
  SD_PAN = 0x0101,
  SD_TILT = 0x0102,
  SD_COLOR_WHEEL = 0x0201,
  SD_COLOR_SUB_CYAN = 0x0202,
  SD_COLOR_SUB_YELLOW = 0x0203,
  SD_COLOR_SUB_MAGENTA = 0x0204,
  SD_COLOR_ADD_RED = 0x0205,
  SD_COLOR_ADD_GREEN = 0x0206,
  SD_COLOR_ADD_BLUE = 0x0207,
  SD_COLOR_CORRECTION = 0x0208,
  SD_COLOR_SCROLL = 0x0209,
  SD_COLOR_SEMAPHORE = 0x0210,
  SD_COLOR_ADD_AMBER = 0x0211,
  SD_COLOR_ADD_WHITE = 0x0212,
  SD_COLOR_ADD_WARM_WHITE = 0x0213,
  SD_COLOR_ADD_COOL_WHITE = 0x0214,
  SD_COLOR_SUB_UV = 0x0215,
  SD_COLOR_HUE = 0x0216,
  SD_COLOR_SATURATION = 0x0217,
  SD_STATIC_GOBO_WHEEL = 0x0301,
  SD_ROTO_GOBO_WHEEL = 0x0302,
  SD_PRISM_WHEEL = 0x0303,
  SD_EFFECTS_WHEEL = 0x0304,
  SD_BEAM_SIZE_IRIS = 0x0401,
  SD_EDGE = 0x0402,
  SD_FROST = 0x0403,
  SD_STROBE = 0x0404,
  SD_ZOOM = 0x0405,
  SD_FRAMING_SHUTTER = 0x0406,
  SD_SHUTTER_ROTATE = 0x0407,
  SD_DOUSER = 0x0408,
  SD_BARN_DOOR = 0x0409,
  SD_LAMP_CONTROL = 0x0501,
  SD_FIXTURE_CONTROL = 0x0502,
  SD_FIXTURE_SPEED = 0x0503,
  SD_MACRO = 0x0504,
  SD_POWER_CONTROL = 0x0505,
  SD_FAN_CONTROL = 0x0506,
  SD_HEATER_CONTROL = 0x0507,
  SD_FOUNTAIN_CONTROL = 0x0508,
  SD_UNDEFINED = 0xffff,
} RDMSlotCategory;

typedef enum {
  IDENTIFY_MODE_QUIET = 0x00,
  IDENTIFY_MODE_LOUD = 0xff,
} RDMIdentifyMode;

typedef enum {
  PRESET_PLAYBACK_OFF = 0x0000,
  PRESET_PLAYBACK_ALL = 0xffff,
} RDMPresetPlayback;

typedef enum {
  PRESET_NOT_PROGRAMMED = 0x00,
  PRESET_PROGRAMMED = 0x01,
  PRESET_PROGRAMMED_READ_ONLY = 0x02
} RDMPresetProgrammed;

typedef enum {
  MERGE_MODE_DEFAULT = 0x00,
  MERGE_MODE_HTP = 0x01,
  MERGE_MODE_LTP = 0x02,
  MERGE_MODE_DMX_ONLY = 0x03,
  MERGE_MODE_OTHER = 0xff,
} RDMMergeMode;

typedef enum {
  STS_CAL_FAIL = 0x0001,
  STS_SENS_NOT_FOUND = 0x0002,
  STS_SENS_ALWAYS_ON = 0x0003,
  STS_FEEDBACK_ERROR = 0x0004,
  STS_INDEX_ERROR = 0x0005,
  STS_LAMP_DOUSED = 0x0011,
  STS_LAMP_STRIKE = 0x0012,
  STS_LAMP_ACCESS_OPEN = 0x0013,
  STS_LAMP_ALWAYS_ON = 0x0014,
  STS_OVERTEMP = 0x0021,
  STS_UNDERTEMP = 0x0022,
  STS_SENS_OUT_RANGE = 0x0023,
  STS_OVERVOLTAGE_PHASE = 0x0031,
  STS_UNDERVOLTAGE_PHASE = 0x0032,
  STS_OVERCURRENT = 0x0033,
  STS_UNDERCURRENT = 0x0034,
  STS_PHASE = 0x0035,
  STS_PHASE_ERROR = 0x0036,
  STS_AMPS = 0x0037,
  STS_VOLTS = 0x0038,
  STS_DIMSLOT_OCCUPIED = 0x0041,
  STS_BREAKER_TRIP = 0x0042,
  STS_WATTS = 0x0043,
  STS_DIM_FAILURE = 0x0044,
  STS_DIM_PANIC = 0x0045,
  STS_LOAD_FAILURE = 0x0046,
  STS_READY = 0x0050,
  STS_NOT_READY = 0x0051,
  STS_LOW_FLUID = 0x0052,
  STS_EEPROM_ERROR = 0x0060,
  STS_RAM_ERROR = 0x0061,
  STS_FPGA_ERROR = 0x0062,
  STS_PROXY_BROADCAST_DROPPED = 0x0070,
  STS_ASC_RXOK = 0x0071,
  STS_ASC_DROPPED = 0x0072,
  STS_DMXNSCNONE = 0x0080,
  STS_DMXNSCLOSS = 0x0081,
  STS_DMXNSCERROR = 0x0082,
  STS_DMXNSC_OK = 0x0083,
} RDMStatusMessageId;

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif  // FIRMWARE_SRC_RDM_H_
