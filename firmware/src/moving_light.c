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
 * moving_light.c
 * Copyright (C) 2015 Simon Newton
 */
#include "moving_light.h"

#include <stdlib.h>

#include "coarse_timer.h"
#include "constants.h"
#include "macros.h"
#include "rdm_frame.h"
#include "rdm_responder.h"
#include "rdm_util.h"
#include "utils.h"

// Various constants
#define LAMP_STRIKE_DELAY 50000
#define SOFTWARE_VERSION 0x00000000

enum {PERSONALITY_COUNT = 2};

static const char DEVICE_MODEL_DESCRIPTION[] = "Ja Rule Moving Light";
static const char SOFTWARE_LABEL[] = "Alpha";
static const char DEFAULT_DEVICE_LABEL[] = "Default Label";
static const char PERSONALITY_DESCRIPTION1[] = "8-bit mode";
static const char PERSONALITY_DESCRIPTION2[] = "16-bit mode";

static const ResponderDefinition RESPONDER_DEFINITION;

/*
 * @brief The simple model state.
 */
typedef struct {
  uint32_t device_hours;
  uint32_t lamp_hours;
  uint32_t lamp_strikes;
  uint32_t device_power_cycles;
  CoarseTimer_Value lamp_strike_time;
  uint8_t lamp_state;
  uint8_t lamp_on_mode;
  uint8_t display_level;
  uint8_t display_invert;
  uint8_t power_state;
  bool pan_invert;
  bool tilt_invert;
  bool pan_tilt_swap;
} MovingLightModel;

static MovingLightModel g_moving_light;

// PID Handlers
// ----------------------------------------------------------------------------
int MovingLightModel_GetBool(const RDMHeader *header,
                             UNUSED const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_PAN_INVERT:
      return RDMResponder_GenericGetBool(header, g_moving_light.pan_invert);
    case PID_TILT_INVERT:
      return RDMResponder_GenericGetBool(header, g_moving_light.tilt_invert);
    case PID_PAN_TILT_SWAP:
      return RDMResponder_GenericGetBool(header, g_moving_light.pan_tilt_swap);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_SetBool(const RDMHeader *header,
                             const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_PAN_INVERT:
      return RDMResponder_GenericSetBool(header, param_data,
                                         &g_moving_light.pan_invert);
    case PID_TILT_INVERT:
      return RDMResponder_GenericSetBool(header, param_data,
                                         &g_moving_light.tilt_invert);
    case PID_PAN_TILT_SWAP:
      return RDMResponder_GenericSetBool(header, param_data,
                                         &g_moving_light.pan_tilt_swap);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_GetUInt8(const RDMHeader *header,
                              UNUSED const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_LAMP_STATE:
      return RDMResponder_GenericGetUInt8(header, g_moving_light.lamp_state);
    case PID_LAMP_ON_MODE:
      return RDMResponder_GenericGetUInt8(header, g_moving_light.lamp_on_mode);
    case PID_DISPLAY_INVERT:
      return RDMResponder_GenericGetUInt8(header,
                                          g_moving_light.display_invert);
    case PID_DISPLAY_LEVEL:
      return RDMResponder_GenericGetUInt8(header, g_moving_light.display_level);
    case PID_POWER_STATE:
      return RDMResponder_GenericGetUInt8(header, g_moving_light.power_state);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_SetUInt8(const RDMHeader *header,
                              const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_DISPLAY_LEVEL:
      return RDMResponder_GenericSetUInt8(header, param_data,
                                          &g_moving_light.display_level);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_GetUInt32(const RDMHeader *header,
                               UNUSED const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_DEVICE_HOURS:
      return RDMResponder_GenericGetUInt32(header, g_moving_light.device_hours);
    case PID_LAMP_HOURS:
      return RDMResponder_GenericGetUInt32(header, g_moving_light.lamp_hours);
    case PID_LAMP_STRIKES:
      return RDMResponder_GenericGetUInt32(header, g_moving_light.lamp_strikes);
    case PID_DEVICE_POWER_CYCLES:
      return RDMResponder_GenericGetUInt32(
          header, g_moving_light.device_power_cycles);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_SetUInt32(const RDMHeader *header,
                               const uint8_t *param_data) {
  switch (ntohs(header->param_id)) {
    case PID_DEVICE_HOURS:
      return RDMResponder_GenericSetUInt32(header, param_data,
                                           &g_moving_light.device_hours);
    case PID_LAMP_HOURS:
      return RDMResponder_GenericSetUInt32(header, param_data,
                                           &g_moving_light.lamp_hours);
    case PID_LAMP_STRIKES:
      return RDMResponder_GenericSetUInt32(header, param_data,
                                           &g_moving_light.lamp_strikes);
    case PID_DEVICE_POWER_CYCLES:
      return RDMResponder_GenericSetUInt32(header, param_data,
                                           &g_moving_light.device_power_cycles);
    default:
      return RDM_RESPONDER_NO_RESPONSE;
  }
}

int MovingLightModel_SetLampState(const RDMHeader *header,
                                  const uint8_t *param_data) {
  if (header->param_data_length != sizeof(uint8_t)) {
    return RDMResponder_BuildNack(header, NR_FORMAT_ERROR);
  }

  if (param_data[0] > LAMP_STRIKE) {
    return RDMResponder_BuildNack(header, NR_DATA_OUT_OF_RANGE);
  }
  if (g_moving_light.lamp_state == LAMP_OFF && param_data[0] == LAMP_ON) {
    g_moving_light.lamp_strikes++;
  }

  g_moving_light.lamp_state = param_data[0];
  if (g_moving_light.lamp_state == LAMP_STRIKE) {
    g_moving_light.lamp_strike_time = CoarseTimer_GetTime();
  }
  return RDMResponder_BuildSetAck(header);
}

int MovingLightModel_SetLampOnMode(const RDMHeader *header,
                                   const uint8_t *param_data) {
  if (header->param_data_length != sizeof(uint8_t)) {
    return RDMResponder_BuildNack(header, NR_FORMAT_ERROR);
  }

  if (param_data[0] > LAMP_ON_MODE_ON_AFTER_CAL) {
    return RDMResponder_BuildNack(header, NR_DATA_OUT_OF_RANGE);
  }
  g_moving_light.lamp_on_mode = param_data[0];
  return RDMResponder_BuildSetAck(header);
}

int MovingLightModel_SetDisplayInvert(const RDMHeader *header,
                                      const uint8_t *param_data) {
  if (header->param_data_length != sizeof(uint8_t)) {
    return RDMResponder_BuildNack(header, NR_FORMAT_ERROR);
  }

  if (param_data[0] > DISPLAY_INVERT_AUTO) {
    return RDMResponder_BuildNack(header, NR_DATA_OUT_OF_RANGE);
  }
  g_moving_light.display_invert = param_data[0];
  return RDMResponder_BuildSetAck(header);
}

int MovingLightModel_SetPowerState(const RDMHeader *header,
                                   const uint8_t *param_data) {
  if (header->param_data_length != sizeof(uint8_t)) {
    return RDMResponder_BuildNack(header, NR_FORMAT_ERROR);
  }

  if (param_data[0] > POWER_STATE_NORMAL) {
    return RDMResponder_BuildNack(header, NR_DATA_OUT_OF_RANGE);
  }
  g_moving_light.power_state = param_data[0];
  return RDMResponder_BuildSetAck(header);
}

// Public Functions
// ----------------------------------------------------------------------------
void MovingLightModel_Initialize(const MovingLightModelSettings *settings) {
  g_moving_light.device_hours = 0;
  g_moving_light.lamp_hours = 0;
  g_moving_light.lamp_strikes = 0;
  g_moving_light.device_power_cycles = 0;
  g_moving_light.lamp_strike_time = 0;
  g_moving_light.lamp_state = LAMP_OFF;
  g_moving_light.lamp_on_mode = LAMP_ON_MODE_ON;
  g_moving_light.display_level = 255;
  g_moving_light.display_invert = false;
  g_moving_light.power_state = POWER_STATE_NORMAL;
  g_moving_light.pan_invert = false;
  g_moving_light.tilt_invert = false;
  g_moving_light.pan_tilt_swap = false;
}

static void MovingLightModel_Activate() {
  g_responder.def = &RESPONDER_DEFINITION;
  RDMResponder_ResetToFactoryDefaults();
}

static void MovingLightModel_Deactivate() {
}

static int MovingLightModel_Ioctl(ModelIoctl command, uint8_t *data,
                                  unsigned int length) {
  switch (command) {
    case IOCTL_GET_UID:
      if (length != UID_LENGTH) {
        return 0;
      }
      RDMResponder_GetUID(data);
      return 1;
    default:
      return 0;
  }
}

static int MovingLightModel_HandleRequest(const RDMHeader *header,
                                          const uint8_t *param_data) {
  if (!RDMUtil_RequiresAction(g_responder.uid, header->dest_uid)) {
    return RDM_RESPONDER_NO_RESPONSE;
  }

  uint16_t sub_device = ntohs(header->sub_device);

  // No subdevices.
  if (sub_device != SUBDEVICE_ROOT && sub_device != SUBDEVICE_ALL) {
    return RDMResponder_BuildNack(header, NR_SUB_DEVICE_OUT_OF_RANGE);
  }

  if (header->command_class == DISCOVERY_COMMAND) {
    return RDMResponder_HandleDiscovery(header, param_data);
  }

  // This model has no sub devices.
  if (header->command_class == GET_COMMAND && sub_device == SUBDEVICE_ALL) {
    return RDMResponder_BuildNack(header, NR_SUB_DEVICE_OUT_OF_RANGE);
  }

  return RDMResponder_DispatchPID(header, param_data);
}

static void MovingLightModel_Tasks() {
  if (g_moving_light.lamp_state == LAMP_STRIKE &&
      CoarseTimer_HasElapsed(g_moving_light.lamp_strike_time,
                             LAMP_STRIKE_DELAY)) {
    g_moving_light.lamp_state = LAMP_ON;
    g_moving_light.lamp_strikes++;
  }
}

const ModelEntry MOVING_LIGHT_MODEL_ENTRY = {
  .model_id = MOVING_LIGHT_MODEL_ID,
  .activate_fn = MovingLightModel_Activate,
  .deactivate_fn = MovingLightModel_Deactivate,
  .ioctl_fn = MovingLightModel_Ioctl,
  .request_fn = MovingLightModel_HandleRequest,
  .tasks_fn = MovingLightModel_Tasks
};

static const PIDDescriptor PID_DESCRIPTORS[] = {
  {PID_SUPPORTED_PARAMETERS, RDMResponder_GetSupportedParameters, 0,
    (PIDCommandHandler) NULL},
  {PID_DEVICE_INFO, RDMResponder_GetDeviceInfo, 0, (PIDCommandHandler) NULL},
  {PID_PRODUCT_DETAIL_ID_LIST, RDMResponder_GetProductDetailIds, 0,
    (PIDCommandHandler) NULL},
  {PID_DEVICE_MODEL_DESCRIPTION, RDMResponder_GetDeviceModelDescription, 0,
    (PIDCommandHandler) NULL},
  {PID_MANUFACTURER_LABEL, RDMResponder_GetManufacturerLabel, 0,
    (PIDCommandHandler) NULL},
  {PID_DEVICE_LABEL, RDMResponder_GetDeviceLabel, 0,
    RDMResponder_SetDeviceLabel},
  {PID_SOFTWARE_VERSION_LABEL, RDMResponder_GetSoftwareVersionLabel, 0,
    (PIDCommandHandler) NULL},
  {PID_DMX_PERSONALITY, RDMResponder_GetDMXPersonality, 0,
    RDMResponder_SetDMXPersonality},
  {PID_DMX_PERSONALITY_DESCRIPTION, RDMResponder_GetDMXPersonalityDescription,
    1, (PIDCommandHandler) NULL},
  {PID_DMX_START_ADDRESS, RDMResponder_GetDMXStartAddress, 0,
    RDMResponder_SetDMXStartAddress},
  {PID_SLOT_INFO, RDMResponder_GetSlotInfo, 0,
    (PIDCommandHandler) NULL},
  {PID_SLOT_DESCRIPTION, RDMResponder_GetSlotDescription, 2,
    (PIDCommandHandler) NULL},
  {PID_DEFAULT_SLOT_VALUE, RDMResponder_GetDefaultSlotValue, 0,
    (PIDCommandHandler) NULL},
  {PID_DEVICE_HOURS, MovingLightModel_GetUInt32, 0, MovingLightModel_SetUInt32},
  {PID_LAMP_HOURS, MovingLightModel_GetUInt32, 0, MovingLightModel_SetUInt32},
  {PID_LAMP_STRIKES, MovingLightModel_GetUInt32, 0, MovingLightModel_SetUInt32},
  {PID_LAMP_STATE, MovingLightModel_GetUInt8, 0, MovingLightModel_SetLampState},
  {PID_LAMP_ON_MODE, MovingLightModel_GetUInt8, 0,
    MovingLightModel_SetLampOnMode},
  {PID_DEVICE_POWER_CYCLES, MovingLightModel_GetUInt32, 0,
    MovingLightModel_SetUInt32},
  {PID_DISPLAY_INVERT, MovingLightModel_GetUInt8, 0,
    MovingLightModel_SetDisplayInvert},
  {PID_DISPLAY_LEVEL, MovingLightModel_GetUInt8, 0, MovingLightModel_SetUInt8},
  {PID_PAN_INVERT, MovingLightModel_GetBool, 0, MovingLightModel_SetBool},
  {PID_TILT_INVERT, MovingLightModel_GetBool, 0, MovingLightModel_SetBool},
  {PID_PAN_TILT_SWAP, MovingLightModel_GetBool, 0, MovingLightModel_SetBool},
  {PID_IDENTIFY_DEVICE, RDMResponder_GetIdentifyDevice, 0,
    RDMResponder_SetIdentifyDevice},
  {PID_POWER_STATE, MovingLightModel_GetUInt8, 0,
    MovingLightModel_SetPowerState}
};

static const ProductDetailIds PRODUCT_DETAIL_ID_LIST = {
  .ids = {PRODUCT_DETAIL_TEST, PRODUCT_DETAIL_CHANGEOVER_MANUAL,
          PRODUCT_DETAIL_LED},
  .size = 3
};

static const char SLOT_DIMMER_DESCRIPTION[] = "Dimmer";
static const char SLOT_PAN_DESCRIPTION[] = "Pan";
static const char SLOT_PAN_FINE_DESCRIPTION[] = "Pan (Fine)";
static const char SLOT_TILT_DESCRIPTION[] = "Tilt";
static const char SLOT_TILT_FINE_DESCRIPTION[] = "Tilt (Fine)";
static const char SLOT_COLOR_DESCRIPTION[] = "Color Wheel";

static const SlotDefinition PERSONALITY_SLOTS1[] = {
  {
    .description = SLOT_DIMMER_DESCRIPTION,
    .slot_label_id = SD_INTENSITY,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_PAN_DESCRIPTION,
    .slot_label_id = SD_PAN,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_TILT_DESCRIPTION,
    .slot_label_id = SD_TILT,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_COLOR_DESCRIPTION,
    .slot_label_id = SD_COLOR_WHEEL,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  }
};

static const SlotDefinition PERSONALITY_SLOTS2[] = {
  {
    .description = SLOT_DIMMER_DESCRIPTION,
    .slot_label_id = SD_INTENSITY,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_PAN_DESCRIPTION,
    .slot_label_id = SD_PAN,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_PAN_FINE_DESCRIPTION,
    .slot_label_id = 1,
    .slot_type = ST_SEC_FINE,
    .default_value = 0,
  },
  {
    .description = SLOT_TILT_DESCRIPTION,
    .slot_label_id = SD_TILT,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  },
  {
    .description = SLOT_TILT_FINE_DESCRIPTION,
    .slot_label_id = 3,
    .slot_type = ST_SEC_FINE,
    .default_value = 0,
  },
  {
    .description = SLOT_COLOR_DESCRIPTION,
    .slot_label_id = SD_COLOR_WHEEL,
    .slot_type = ST_PRIMARY,
    .default_value = 0,
  }
};

static const PersonalityDefinition PERSONALITIES[PERSONALITY_COUNT] = {
  {
    .dmx_footprint = 4,
    .description = PERSONALITY_DESCRIPTION1,
    .slots = PERSONALITY_SLOTS1,
    .slot_count = 4
  },
  {
    .dmx_footprint = 6,
    .description = PERSONALITY_DESCRIPTION2,
    .slots = PERSONALITY_SLOTS2,
    .slot_count = 6
  }
};

static const ResponderDefinition RESPONDER_DEFINITION = {
  .descriptors = PID_DESCRIPTORS,
  .descriptor_count = sizeof(PID_DESCRIPTORS) / sizeof(PIDDescriptor),
  .sensors = NULL,
  .sensor_count = 0,
  .personalities = PERSONALITIES,
  .personality_count = PERSONALITY_COUNT,
  .software_version_label = SOFTWARE_LABEL,
  .manufacturer_label = MANUFACTURER_LABEL,
  .model_description = DEVICE_MODEL_DESCRIPTION,
  .product_detail_ids = &PRODUCT_DETAIL_ID_LIST,
  .default_device_label = DEFAULT_DEVICE_LABEL,
  .software_version = SOFTWARE_VERSION,
  .model_id = MOVING_LIGHT_MODEL_ID,
  .product_category = PRODUCT_CATEGORY_TEST_EQUIPMENT
};
