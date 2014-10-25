/*
 * Copyright (C) 2006 The Android Open Source Project
 * Copyright (C) 2014 Rudolf Tammekivi <rtammekivi@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_RIL_V4_H
#define ANDROID_RIL_V4_H

typedef struct {
    char *aidPtr;   /* AID value, See ETSI 102.221 8.1 and 101.220 4, NULL if no value. */
    int command;    /* one of the commands listed for TS 27.007 +CRSM*/
    int fileid;     /* EF id */
    char *path;     /* "pathid" from TS 27.007 +CRSM command.
                       Path is in hex asciii format eg "7f205f70"
                       Path must always be provided.
                     */
    int p1;
    int p2;
    int p3;
    char *data;     /* May be NULL*/
    char *pin2;     /* May be NULL*/
} RIL_SIM_IO_v4;

typedef struct
{
  RIL_CardState card_state;
  RIL_PinState  universal_pin_state;             /* applicable to USIM and CSIM: RIL_PINSTATE_xxx */
  int           num_current_3gpp_indexes;
  int           subscription_3gpp_app_index[RIL_CARD_MAX_APPS]; /* value < RIL_CARD_MAX_APPS */
  int           num_current_3gpp2_indexes;
  int           subscription_3gpp2_app_index[RIL_CARD_MAX_APPS]; /* value < RIL_CARD_MAX_APPS */
  int           num_applications;                /* value <= RIL_CARD_MAX_APPS */
  RIL_AppStatus applications[RIL_CARD_MAX_APPS];
} RIL_CardStatus_v4;

typedef struct {
    int             cid;        /* Context ID */
    int             active;     /* 0=inactive, 1=active/physical link down, 2=active/physical link up */
    char *          type;       /* One of the PDP_type values in TS 27.007 section 10.1.1.
                                   For example, "IP", "IPV6", "IPV4V6", or "PPP". */
    char *          apn;
    char *          address;    /* A space-delimited list of addresses, e.g., "192.0.1.3" or
                                   "192.0.1.11 2001:db8::1". */
    RIL_RadioTechnology     radioTech;     /* Radio Technology, this data call was setup on */
    int             inactiveReason; /* if the data call went inactive(0),
                                       then the reason for being inactive.
                                       defined in RIL_DataCallDropCause */
} RIL_Data_Call_Response_v4_;

typedef struct {
    RIL_PersoSubstate depersonalizationType;
    char             *depersonalizationCode;
} RIL_Depersonalization_v4;

typedef struct {
    RIL_SimRefreshResult     refreshResult;      /* Sim Refresh result */
    char                    *aidPtr;             /* null terminated string, e.g., from 0xA0, 0x00
                                                    0x41, 0x30*/
    int                      efId;               /* EFID */
} RIL_SimRefreshResponse_v4;

#define RIL_REQUEST_VOICE_RADIO_TECH_V4 105
#define RIL_REQUEST_IMS_REGISTRATION_STATE_V4 106
#define RIL_REQUEST_IMS_SEND_SMS_V4 107
#define RIL_REQUEST_GET_DATA_CALL_PROFILE_V4 108

#define RIL_UNSOL_VOICE_RADIO_TECH_CHANGED_V4 1034
#define RIL_UNSOL_RESPONSE_IMS_NETWORK_STATE_CHANGED_V4 1035
#define RIL_UNSOL_RESPONSE_TETHERED_MODE_STATE_CHANGED_V4 1036
#define RIL_UNSOL_RESPONSE_DATA_NETWORK_STATE_CHANGED_V4 1037

/* Virtual */
#define RIL_UNSOL_RIL_CONNECTED_V4 1040

#endif /* ANDROID_RIL_V4_H */
