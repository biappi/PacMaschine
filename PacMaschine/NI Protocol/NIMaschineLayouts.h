//
//  NIMaschineLayouts.h
//  SalaMacchine
//
//  Created by Antonio "Willy" Malara on 11/08/13.
//  Copyright (c) 2013 Antonio "Willy" Malara. All rights reserved.
//

typedef enum {
    NIMaschineButton_Mute       =  0,
    NIMaschineButton_Solo       =  1,
    NIMaschineButton_Select     =  2,
    NIMaschineButton_Duplicate  =  3,
    NIMaschineButton_Navigate   =  4,
    NIMaschineButton_PadMode    =  5,
    NIMaschineButton_Pattern    =  6,
    NIMaschineButton_Scene      =  7,
    
    NIMaschineButton_Restart    = 15,
    NIMaschineButton_Rewind     = 14,
    NIMaschineButton_Forward    = 13,
    NIMaschineButton_Grid       = 12,
    NIMaschineButton_Play       = 41,
    NIMaschineButton_Rec        =  9,
    NIMaschineButton_Erase      = 10,
    NIMaschineButton_Shift      = 11,
    
    NIMaschineButton_GroupA     = 23,
    NIMaschineButton_GroupB     = 22,
    NIMaschineButton_GroupC     = 21,
    NIMaschineButton_GroupD     = 20,
    NIMaschineButton_GroupE     = 16,
    NIMaschineButton_GroupF     = 17,
    NIMaschineButton_GroupG     = 18,
    NIMaschineButton_GroupH     = 19,
    
    NIMaschineButton_NoteRepeat = 40,
    
    NIMaschineButton_Snap       = 27,
    NIMaschineButton_AutoWrite  = 28,
    
    NIMaschineButton_PageLeft   = 26,
    NIMaschineButton_PageRight  = 29,
    
    NIMaschineButton_Control    = 24,
    NIMaschineButton_Step       = 31,
    
    NIMaschineButton_Browse     = 25,
    NIMaschineButton_Sampling   = 30,
    
    NIMaschineButton_Soft1      = 39,
    NIMaschineButton_Soft2      = 38,
    NIMaschineButton_Soft3      = 37,
    NIMaschineButton_Soft4      = 36,
    NIMaschineButton_Soft5      = 35,
    NIMaschineButton_Soft6      = 34,
    NIMaschineButton_Soft7      = 33,
    NIMaschineButton_Soft8      = 32,
} NIMaschineButtonsLayout;

typedef enum {
    NIMaschineWheel_Volume,
    NIMaschineWheel_Tempo,
    NIMaschineWheel_Swing,
    
    NIMaschineWheel_Soft1,
    NIMaschineWheel_Soft2,
    NIMaschineWheel_Soft3,
    NIMaschineWheel_Soft4,
    NIMaschineWheel_Soft5,
    NIMaschineWheel_Soft6,
    NIMaschineWheel_Soft7,
    NIMaschineWheel_Soft8,
} NIMaschineWheelsLayout;

typedef enum {
    NIMaschineLed_Pad4,
    NIMaschineLed_Pad3,
    NIMaschineLed_Pad2,
    NIMaschineLed_Pad1,
    NIMaschineLed_Pad8,
    NIMaschineLed_Pad7,
    NIMaschineLed_Pad6,
    NIMaschineLed_Pad5,
    NIMaschineLed_Pad12,
    NIMaschineLed_Pad11,
    NIMaschineLed_Pad10,
    NIMaschineLed_Pad9,
    NIMaschineLed_Pad16,
    NIMaschineLed_Pad15,
    NIMaschineLed_Pad14,
    NIMaschineLed_Pad13,
    
    NIMaschineLed_Mute,
    NIMaschineLed_Solo,
    NIMaschineLed_Select,
    NIMaschineLed_Duplicate,
    NIMaschineLed_Navigate,
    NIMaschineLed_PadMode,
    NIMaschineLed_Pattern,
    NIMaschineLed_Scene,
    
    // -
    NIMaschineLed_Shift,
    NIMaschineLed_Erase,
    NIMaschineLed_Grid,
    NIMaschineLed_Forward,
    NIMaschineLed_Rec,
    NIMaschineLed_Play,
    // -
    NIMaschineLed_Rewind,
    NIMaschineLed_Restart,
    
    NIMaschineLed_GroupH,
    NIMaschineLed_GroupG,
    NIMaschineLed_GroupD,
    NIMaschineLed_GroupC,
    NIMaschineLed_GroupF,
    NIMaschineLed_GroupE,
    NIMaschineLed_GroupB,
    NIMaschineLed_GroupA,
    
    NIMaschineLed_AutowWrite,
    NIMaschineLed_Snap,
    NIMaschineLed_PageRight,
    NIMaschineLed_PageLeft,
    NIMaschineLed_Sampling,
    NIMaschineLed_Browse,
    NIMaschineLed_Step,
    NIMaschineLed_Control,
    
    NIMaschineLed_SoftButton8,
    NIMaschineLed_SoftButton7,
    NIMaschineLed_SoftButton6,
    NIMaschineLed_SoftButton5,
    NIMaschineLed_SoftButton4,
    NIMaschineLed_SoftButton3,
    NIMaschineLed_SoftButton2,
    NIMaschineLed_SoftButton1,
    
    NIMaschineLed_NoteRepeat,
} NIMaschineLedsLayout;