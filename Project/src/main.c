#include "global.h"
//#include "Timer.h"
//#include "OC.h"
#include "init.h"
#include "UART.h"
#include "TFT.h"
#include "Radar.h"
#include <math.h>
#include "SPI.h"
#include "Timer.h"
#include "OC.h"
#include "Servo.h"
#include "HF_IPS.h"
//#include "SPI.h"
#include "ADC.h"

bool WeaponPage = false;

Turret_para turret_para;

uchar choosenTargetID;

bool TWS = false;

f_Vector2 TDC_pos;

float distZoom = 1;

bool TWS_buttonDown = false;
bool ZoomIn_buttonDown = false;
bool ZoomOut_buttonDown = false;
bool SwitchScreen_buttonDown = false;
bool WeaponShoot_buttonDown = false;

// use radar target info to calculate the position of target with respect to turrent
Vector3 Get_Target_Position(TargetInfo info){
    Vector3 target_Position;
    char pitch = 90-info.pitch + RADAR_PITCH;
    float distance_xz = (float)info.distance * cos(M_PI/180 * pitch);
    target_Position.x = distance_xz * cos(M_PI/180 * info.yaw);
    target_Position.z = distance_xz * sin(M_PI/180 * info.yaw);
    target_Position.y = (float)info.distance * sin(M_PI/180 * pitch); // refer to LD6001-60G instruction for details of implementation
    return target_Position;
}

void Update_FireControl_Direct(Vector3 lockedTarget_Position){
    Vector3 realTarget_Position = {lockedTarget_Position.x-TURRENT_X_OFFSET, 
                                    lockedTarget_Position.y-TURRENT_Y_OFFSET, 
                                    lockedTarget_Position.z-TURRENT_Z_OFFSET};  // calculate the target position with respect to turret
    //Print_Vector3D(realTarget_Position);

    float distance_xyz = sqrt(pow(realTarget_Position.x, 2) + pow(realTarget_Position.y, 2) + pow(realTarget_Position.z, 2));
    float distance_xz = sqrt(pow(realTarget_Position.x, 2) + pow(realTarget_Position.z, 2));
    
    float previous_pitch = turret_para.pitch;
    float previous_yaw = turret_para.yaw;

    float now_pitch = 180/M_PI * atan(realTarget_Position.y/distance_xz); // up: y positive
    float now_yaw = 90 - 180/M_PI * atan(realTarget_Position.x/realTarget_Position.z); // forward: z positive; right: x positive; anti-clockwise: 0~180 yaw angle

    //turret_para.pitch = previous_pitch + float_clamp(0.3f * (now_pitch - previous_pitch), -1, 1);
    //turret_para.yaw = previous_yaw + float_clamp(0.3f * (now_yaw - previous_yaw), -1, 1);
    turret_para.pitch = now_pitch;
    turret_para.yaw = now_yaw;


    if (turret_para.pitch<-30) {
        turret_para.pitch = -30;
    }else if(turret_para.pitch>45){
        turret_para.pitch = 45;
    }

    if (turret_para.yaw < 0) {
        turret_para.yaw = 0;
        }else if(turret_para.yaw > 180){
        turret_para.yaw = 180;
    }
}

void Reset_FireControl(){
    turret_para.pitch = 0; // up: y positive
    turret_para.yaw = 90; // forward: z positive; right: x positive; anti-clockwise: 0~180 yaw angle
}

void Update_Turret_Servo(){
    Set_Servo1(turret_para.yaw+YAW_SERVO_OFFSET);
    Set_Servo2(-turret_para.pitch+90+PITCH_SERVO_OFFSET);
}

void Init_Turret_Servo(){
    Enable_Servo1();
    Set_Servo1(90);
    Enable_Servo2();
    Set_Servo2(90);
}

// calculate the screen position of TDC
u_Vector2 GetTDCScreenPosition(){
    u_Vector2 res;
    res.x = (uint)(TDC_pos.x * 100 + 100);
    res.y = (uint)(-TDC_pos.y * 110 + 110);
    return res;
}

// calculate the screen position of target
u_Vector2 TargetInfo_2_ScreenPos(TargetInfo info){
    u_Vector2 res;
    res.x = (info.yaw-30) * 1.75f; // 30-150 : 0-210 : 15-225
    res.y = (info.distance) * 2.875f; // 0-80 : 0-230 : 245-15
    // restruct x and y
    if (res.x < 0) res.x = 0;
    if (res.x > 210) res.x = 210;
    if (res.y < 0) res.y = 0;
    if (res.y > 230) res.y = 230;
    return res;
}

// find the cloest target to TDC and set the choosenID
bool Get_TDC_Selection(){
    uint dist = 999;
    uchar id;
    u_Vector2 TDCPosition = GetTDCScreenPosition();
    for (int i = 0; i<10; i++) {
        if (radarInfo.targets[i].hasTarget) {
            u_Vector2 target_pos = TargetInfo_2_ScreenPos(radarInfo.targets[i]);
            uint tmp_dist = abs(target_pos.x-TDCPosition.x) + abs(target_pos.y-TDCPosition.y);
            if (tmp_dist < dist) {
                id = i;
                dist = tmp_dist;
            }
        }
    }

    if (dist != 999) {
        choosenTargetID = id;
        return true;
    }else{
        return false;
    }
}

// switch between TWS and RWS
void SwitchTWS(){
    if (Get_TDC_Selection() && !TWS) {
        // wait for the previous command to finish execution
        IPS_CHECKBUSY();
        TWS = true;
        U2_Print("SET_TXT(0,'TWS');\r\n");
    }else if(TWS){
        IPS_CHECKBUSY();
        TWS = false;
        U2_Print("SET_TXT(0,'RWS');\r\n");
    }
}

// move TDC to the choosen target
void TDC_TrackTarget(){
    if (radarInfo.targets[choosenTargetID].hasTarget) {
        u_Vector2 target_pos = TargetInfo_2_ScreenPos(radarInfo.targets[choosenTargetID]);
        TDC_pos.x = target_pos.x/100.0f - 1.05;
        TDC_pos.y = -(target_pos.y/110.0f - 1.03);
        if (TDC_pos.x>1) {
            TDC_pos.x = 1;
        }else if (TDC_pos.x < -1) {
            TDC_pos.x = -1;
        }
        if (TDC_pos.y>1) {
            TDC_pos.y = 1;
        }else if (TDC_pos.y < -1) {
            TDC_pos.y = -1;
        }
    }else{
        if (TWS) {
            SwitchTWS();
        }
    }
}

void IncreaseZoom(){
    distZoom *= 2;
    IPS_CHECKBUSY();
    U2_Print("SET_TXT(6,'");
    U2_Print_float(8/distZoom);
    U2_Print("m');\r\n");
}

void DecreaseZoom(){
    distZoom /= 2;
    IPS_CHECKBUSY();
    U2_Print("SET_TXT(6,'");
    U2_Print_float(8/distZoom);
    U2_Print("m');\r\n");
}

// switch the screen between Radar and TWS page
void SwitchPage(){
    if (WeaponPage) {
        WeaponPage = false;
        IPS_CHECKBUSY();
        U2_Print("JUMP(0);\r\n");
        DelayMsec(500);
        // reload the configuration of radar page including TWS and zoom
        U2_Print("SET_TXT(6,'");
        U2_Print_float(8/distZoom);
        U2_Print("m');");
        if (TWS) {
            U2_Print("SET_TXT(0,'TWS');");
        }
        IPS_CMD_EXECUTE();
    }else{
        WeaponPage = true;
        IPS_CHECKBUSY();
        U2_Print("JUMP(1);\r\n");
        DelayMsec(500);
    }
}


void Setup(){
    Init_MCU();
    Init_Turret_Servo();
    IPS_RESET();
    //LATBbits.LATB3 = 1;
    SHOOT = 0;
}

void StickLoop(){
    DelayMsec(10);
    f_Vector2 res = Get_Stick_xy();
    /*
    SPI1_Print_float(res.x);
    SPI1_Print(",");
    SPI1_Print_float(res.y);
    SPI1_Print("\r\n");*/
    TDC_pos.x += 0.05*res.y*TDC_SPEED;
    TDC_pos.y += -0.05*res.x*TDC_SPEED;
    if (TDC_pos.x>1) {
        TDC_pos.x = 1;
    }else if (TDC_pos.x < -1) {
        TDC_pos.x = -1;
    }
    if (TDC_pos.y>1) {
        TDC_pos.y = 1;
    }else if (TDC_pos.y < -1) {
        TDC_pos.y = -1;
    }
}

void RenderLoop(){
    if (!WeaponPage) {
        bool hasCommand = false;
        //first clear existing spot on radar
        hasCommand = IPS_CLR_ALL_TARGET();
        if (hasCommand) {
            IPS_CLR_TDC();
        }else{
            hasCommand = IPS_CLR_TDC();
        }
        // update TDC position
        IPS_DRAW_TDC(GetTDCScreenPosition(),distZoom);
        // update new spot on screen
        for (uchar id = 0; id<10; id++) {
            if (radarInfo.targets[id].hasTarget) {
                IPS_DRAW_TARGET(radarInfo.targets[id],distZoom);
                hasCommand = true;
            }
        }

        if (hasCommand) {
            IPS_CMD_EXECUTE();  // the execution command is sent, the commands will take some time for execution
        }
    }else{
        DelayMsec(50);
        U2_Print("SET_POINT(4,");
        int angle = 180 - (int)turret_para.yaw;
        U2_Print_float(angle);
        U2_Print(");");
        
        U2_Print("SET_PROG(5,");
        int pit = (int)(turret_para.pitch + 30)*1.66;
        U2_Print_float(pit);
        U2_Print(");");

        U2_Print("SET_TXT(10,");
        U2_Print_float(turret_para.yaw);
        U2_Print(");");
        U2_Print("SET_TXT(11,");
        U2_Print_float(turret_para.pitch);
        U2_Print(");");

        IPS_CMD_EXECUTE();
    }
}

void Loop(){
    if (SCREENRESET_BUTTON == 0) {
        IPS_RESET();
        TWS = false;
        distZoom = 1;
        WeaponPage = false;
    }
    if (SCREENSWITCH_BUTTON == 0 && !SwitchScreen_buttonDown) {
        SwitchPage();
        SwitchScreen_buttonDown = true;
    }else if(SCREENSWITCH_BUTTON == 1 && SwitchScreen_buttonDown){
        SwitchScreen_buttonDown = false;
    }
    if (!WeaponPage) {
        if (SWITCHTWS_BUTTON == 0 && !TWS_buttonDown) {
            SwitchTWS();
            TWS_buttonDown = true;
        }else if(SWITCHTWS_BUTTON == 1 && TWS_buttonDown){
            TWS_buttonDown = false;
        }

        if (ZOOMIN_BUTTON == 0 && !ZoomIn_buttonDown) {
            IncreaseZoom();
            ZoomIn_buttonDown = true;
        }else if(ZOOMIN_BUTTON == 1 && ZoomIn_buttonDown){
            ZoomIn_buttonDown = false;
        }

        if (ZOOMOUT_BUTTON == 0 && !ZoomOut_buttonDown) {
            DecreaseZoom();
            ZoomOut_buttonDown = true;
        }else if(ZOOMOUT_BUTTON == 1 && ZoomOut_buttonDown){
            ZoomOut_buttonDown = false;
        }
    }else{
        if (TWS) {
            if (WEAPONSHOOT_BUTTON == 0) {
                SHOOT = 1;
            }else{
                SHOOT = 0;
            }
        }
    }


    if (TWS) {
        TDC_TrackTarget();
        if (radarInfo.targets[choosenTargetID].hasTarget) {
            Update_FireControl_Direct(Get_Target_Position(radarInfo.targets[choosenTargetID]));
        }
    }else{
        Reset_FireControl();
    }
    

    Update_Turret_Servo();

}


int main(void)
{
    Setup();
    while(1){
        Loop();
        StickLoop();
        RenderLoop();
    }
    return 0;
}
