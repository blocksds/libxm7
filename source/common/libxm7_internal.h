// SPDX-License-Identifier: MIT
//
// Copyright (c) 2018 sverx

#ifndef LIBXM7_INTERNAL_H__
#define LIBXM7_INTERNAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nds/ndstypes.h>

typedef struct {
    char FixedText[17];     //  ID text: must be 'Extended module: ' ; will be checked
    char XMModuleName[20];
    char FixedChar;         //  MUST be 0x1a                         ; will be checked
    char TrackerName[20];
    u16 Version;            //  MUST be 0x103 or 0x104               ; will be checked
    u32 HeaderSize;
    u16 SongLength;
    u16 RestartPosition;
    u16 NumberofChannels;   //  1..32, > 16 won't be accepted anyway (we've got 16 chn on the DS)
    u16 NumberofPatterns;
    u16 NumberofInstruments;
    u16 XMModuleFlags;
    u16 DefaultTempo;
    u16 DefaultBPM;
    u8 PatternOrder[257];   // 1 more, to build a pointer to this point
}__attribute__ ((packed)) XM7_XMModuleHeader_Type;

typedef struct {
    u32 HeaderLength;               // _always_ 0x9
    u8 PackingType;                 // MUST be ZERO!
    u16 NumberofLinesinThisPattern; // 1..256
    u16 PackedPatterndataLength;    // CAN be ZERO!
    u8 PatternData[1];              // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMPatternHeader_Type;

typedef struct {
    u32 InstrumentHeaderLength;
    char Name[22];
    u8 Type;                        // MUST be ZERO! (...but I've seen everything here :| ...)
    u16 NumberofSamples;            // 0..16 ... oh yes, ...it CAN be ZERO...
    u8 NextHeaderPart[1];           // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMInstrument1stHeader_Type;

typedef struct {
    u32 SampleHeaderLength;
    u8  SampleforNotes[96];         // the sample to be used for each note
    u16 VolumeEnvelopePoints [24];
    u16 PanningEnvelopePoints[24];  // Envelope points: x,y...x,y.... in couples
                                    // (2 halfwords/point => a maximum of 12 points).
    u8 NumberofVolumePoints;
    u8 NumberofPanningPoints;

    u8 VolumeSustainPoint;
    u8 VolumeLoopStartPoint;
    u8 VolumeLoopEndPoint;

    u8 PanningSustainPoint;
    u8 PanningLoopStartPoint;
    u8 PanningLoopEndPoint;

    u8 VolumeType;                  // bit 0: On; 1: Sustain; 2: Loop
    u8 PanningType;                 // bit 0: On; 1: Sustain; 2: Loop

    u8 VibratoType;
    u8 VibratoSweep;
    u8 VibratoDepth;
    u8 VibratoRate;

    u16 VolumeFadeOut;
    u16 Reserved[11];               // no use

    u8 NextDataPart[1];             // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMInstrument2ndHeader_Type;

typedef struct {
    u32 Length;
    u32 LoopStart;
    u32 LoopLength;

    u8 Volume;
    s8 FineTune;
    u8 Type;                        // bit 0,1 - 0: no loop; 1: forward; 2:ping-pong
                                    // bit   4 - 0: 8 bit; 1: 16 bit
    u8 Panning;                     // 0x00..0xff
    s8 RelativeNote;                // -128..+127
    u8 Reserved;                    // no use

    char Name[22];

    u8 NextHeader[1];               // I need this to build a pointer to this point
}__attribute__ ((packed)) XM7_XMSampleHeader_Type;

// XM file format was found at http://aluigi.altervista.org/mymusic/xm.txt


// MOD section follows:

typedef struct {
    char Name[22];          // Sample name
    u16 Length;             // Sample lenght in Motorola words (*)
    u8 FineTune;            // Finetune, signed nibble.
    u8 Volume;              // 0..0x40
    u16 LoopStart;          // Sample repeat offset in Motorola words (*)
    u16 LoopLength;         // Sample repeat length in Motorola words (*)
                            // (sample has loop if this is > 2 )
}__attribute__ ((packed)) XM7_MODSampleHeader_Type;

typedef struct {
    char MODModuleName[20];                     // MODule name
    XM7_MODSampleHeader_Type Instrument[31];    // array of 31 instruments
    u8 SongLength;                              // MODule lenght in POT
    u8 RestartPosition;                         // MODule restart position in POT
    u8 PatternOrder[128];                       // POT
    char FileFormat[4];                         //    --> SEE NOTE!
    u8 NextDataPart;                            // need this to build a pointer to this point
}__attribute__ ((packed)) XM7_MODModuleHeader_Type;

typedef struct {
    u8 PeriodH;         // high nibble of period (low nibble) & instrument MSB (high nibble)
    u8 PeriodL;         // low byte of period
    u8 Instr_EffType;   // instrument (high nibble) & effect type (low nibble)
    u8 EffParam;        // effect param
}__attribute__ ((packed)) XM7_MODSingleNote_Type;

typedef struct {
    XM7_MODSingleNote_Type SingleNote[1];
}__attribute__ ((packed)) XM7_MODPattern_Type;

// (*) Motorola words = 16 bit BIG ENDIAN = 1 Motorola word is 2 bytes, byte-swapped

// end of MOD section

#ifdef __cplusplus
}
#endif

#endif // LIBXM7_INTERNAL_H__
