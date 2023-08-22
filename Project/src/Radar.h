#ifndef RADAR_H
#define RADAR_H

#include "global.h"

typedef struct TargetInfo{
    bool hasTarget;
    uchar distance;
    uchar pitch;
    uchar yaw;
    uchar closingRate;
} TargetInfo;

typedef struct RadarInfo{
    uchar targetNum;
    TargetInfo targets[RADAR_MAX_TARGET];
    TargetInfo preTargets[RADAR_MAX_TARGET];
} RadarInfo;

extern uchar RADAR_GET_STATUS[RADAR_GET_STATUS_LEN];
extern uchar RADAR_GET_DATA[RADAR_GET_DATA_LEN];
extern RadarInfo radarInfo;
extern uchar RadarInfo_Buffer[RADAR_BUFFER_MAX_LEN];
extern uchar RadarInfo_Buffer_Index;
extern uchar RadarInfo_Buffer_ExpectLen;

void Init_RadarInfo();
void Update_RadarInfo();

#endif // !RADAR_H
