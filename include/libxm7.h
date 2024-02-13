// SPDX-License-Identifier: MIT
//
// Copyright (c) 2018 sverx

/// @file libxm7.h
///
/// @brief Global include of libXM7.
///
/// @mainpage libXM7 documentation
///
/// ## Introduction
///
/// A C library to play XM (and MOD) modules on Nintendo DS using only ARM7
/// resources (thus freeing your ARM9 from that task).
///
/// ## API documentation
///
/// @ref libxm7.h "Global libXM7 header"
///
/// ## libXM7 Replay features
///
/// The current version of libXM7 (1.06) can play XMs featuring the following
/// characteristics:
///
/// - Up to 16 channels.
/// - Distinct volume and effect column for each channel.
/// - Module length up to 256 patterns.
/// - Module can contain up to 256 different patterns of variable length from 1
///   to 256 lines.
/// - Up to 128 instruments, each one of which:
///
///   - Has up to 16 different 8 or 16 bit samples, each one having a forward
///     (normal) or a ping-pong loop optionally.
///   - May have a volume envelope with up to 12 points, optionally with a
///     sustain point and/or a loop, plus a release (fade out) rate.
///   - May have a panning envelope with up to 12 points, optionally with a
///     sustain point and/or a loop.
///   - May have a vibrato definition with speed, depth and sweep.
///
/// - Both *linear frequency table* mode (XM native) and *Amiga frequency table*
///   mode (MOD native) are supported for completeness, even if the latter is
///   very uncommon nowadays
///
/// libXM7 has **complete** effect support. For reference, these are the
/// effects:
///
/// - `0xy` Arpeggio
/// - `1xx` Portamento up (M)
/// - `2xx` Portamento down (M)
/// - `3xx` Portamento to note (M)
/// - `4xy` Vibrato (M)
/// - `5xy` Portamento to note with volume slide (M)
/// - `6xy` Vibrato with volume slide (M)
/// - `7xy` Tremolo (M)
/// - `8xx` Set note panning
/// - `9xx` Set sample offset (M)
/// - `Axy` Slide note volume up or down (M)
/// - `Bxx` Jump to position
/// - `Cxx` Set note volume
/// - `Dxx` Pattern break
/// - `E1x` Fine portamento up (M)
/// - `E2x` Fine portamento down (M)
/// - `E3x` Glissando control
/// - `E4x` Vibrato control
/// - `E5x` Instrument finetune
/// - `E6x` Pattern loop
/// - `E7x` Tremolo control
/// - `E8x` Set note panning
/// - `E9x` Retrig note
/// - `EAx` Slide note fine volume up (M)
/// - `EBx` Slide note fine volume down (M)
/// - `ECx` Note cut
/// - `EDx` Note delay
/// - `EEx` Pattern delay
/// - `Fxx` Set song speed
/// - `Gxx` Set global volume
/// - `Hxy` Slide global volume up or down (M)
/// - `Kxx` Key off
/// - `Lxx` Set envelope position
/// - `Pxy` Panning slide (M)
/// - `Rxy` Retrig note with volume slide (M)
/// - `Txy` Tremor (M)
/// - `X1x` Extra fine portamento up (M)
/// - `X2x` Extra fine portamento down (M)
///
/// (M) Means that this effect has "memory".
///
/// The volume column effects support is also **complete**. For your reference
/// the effects follows:
///
/// - `xx` Set note volume (`0x10-0x50`)
/// - `Dx` Slide note volume down (`0x60-0x6F`)
/// - `Ux` Slide note volume up (`0x70-0x7F`)
/// - `dx` Slide note fine volume down (`0x80-0x8F`)
/// - `ux` Slide note fine volume up (`0x90-0x9F`)
/// - `Sx` Set vibrato speed (`0xA0-0xAF`)
/// - `Vx` Vibrato (`0xB0-0xBF`)
/// - `Px` Set note panning (`0xC0-0xCF`)
/// - `Lx` Slide note panning left (`0xD0-0xDF`)
/// - `Rx` Slide note panning right (`0xE0-0xEF`)
/// - `Mx` Portamento to note (`0xF0-0xFF`)
///
/// There's a very good document explaining XM effects (and volume column
/// effects) usage [here](https://milkytracker.org/docs/MilkyTracker.html), on
/// MilkyTracker website.
///
/// The library can play also MODs featuring the following characteristics:
///
/// - `M.K.`, `M!K!` and `FLT4` module subtypes 4 channels modules
/// - `OCTA`, `CD81` and `FLT8` (\*) module subtypes 8 channels modules
/// - `TDZx` module subtype 1 to 3 channels modules
/// - `xCHN` module subtype 2 to 9 channels modules
/// - `xxCH` module subtype 10 to 16 channels modules
/// - Distinct effect column for each channel
/// - Module length up to 128 patterns
/// - Module can contain up to 256 different patterns of fixed length of 64
///   lines each.
/// - Up to 31 instruments, one 8 bits sample each, having a forward (normal)
///   loop optionally.
///
/// Of course libXM7 has **complete** MOD effect support (because they're a
/// subset of XM effects).
///
/// (\*) Yeah, I mean `FLT8` really.
///
/// ## Getting the most of your tune
///
/// Since the library doesn't manipulate the samples data directly but simply
/// uses the Nintendo DS hardware 16 digital channels to replay samples, there
/// are some things you should be aware of when composing tunes especially for
/// this system.  First of all, remember that hardware mixing among the channels
/// on a Nintendo DS will result in a 10bits/32kHz stereo sound being
/// reproduced. It's not poor quality, but it isn't exactly CD quality either.
/// In the mixing process the samples don't get resampled when pitched so make
/// sure you disable this feature in your tracker (*sample interpolation*) if
/// you want to simulate the output you would get on the DS as better as
/// possible.
///
/// The DS hardware also wants all the figures to be in 32bit boundaries, which
/// means that the samples should conform to some rules:
///
/// - Length of samples *without loop* should be *multiple* of 4 for 8 bits
///   samples and should be even for 16 bits samples.
///
/// - The length of the **non-repeating** part of samples *with loops* (of any
///   kind) and the length of the **repeating** part of samples *with forward
///   loops* should be as stated above.
///
/// - Length of the **repeating** part of 8 bits samples *with ping-pong loops*
///   should be *even*. No problems with *ping-pong loops* of 16 bits samples
///   (this happens because DS hardware doesn't support ping-pong loops, so
///   these loops are converted into forward loops by the loading routines).
///
/// A side effect (*detuning*) is possible when these rules are ignored, and it
/// could be perceivable on samples with loops, especially when the repeating
/// part of the sample is very short.
///
/// Finally, it's impossible for the DS hardware to start playing a sample that
/// has a loop from inside the loop from other position but the beginning. This
/// means that you should put some care in preparing your samples having loops
/// if you plan to use the `9xx` (*Set sample offset*) effect on them. Anyway
/// there are no problems if you simply plan to skip samples of the
/// **non-repeating** part of a sample with loop.
///
/// ## How to use libXM7 files
///
/// The library consists of one header file (`libxm7.h`) and two archive files
/// (`libxm77.a` and `libxm79.a`). You should add the path with the header file
/// to your include paths, say the `include/libxm7` directory. On the other
/// hand, you should add the `libs` directory to your linker paths. Then you
/// should modify the makefiles for your project: locate the line where the
/// libraries are listed (`LIBS := -l<library> -l<library>`) and add `-lxm77` in
/// the ARM7 makefile and `LIBDIRS := path/to/libxm7/libs`. Add also `-lmx79` in
/// the ARM9 makefile if you plan to use both libXM7 library parts. Make sure
/// that `-lxm77` appears *before* `-lnds7` and that `-lxm79` appears *before*
/// `-lnds9`.
///
/// ## Inside libXM7 library
///
/// libXM7 is a combined library: it has one part that is linked to the ARM9
/// executable and another part that is linked to the ARM7 executable. However
/// the library implements no means of communication between the processors. So
/// it's up to the programmer to choose a suitable way to communicate, let's
/// say, to the ARM7 where the ARM9 has loaded the module that the former should
/// play. Also, the programmer is supposed to ensure that the XM7_StopModule()
/// gets executed on ARM7 before XM7_UnloadXM() gets executed on ARM9. By the
/// way, you can even write your own module loader (for instance one that reads
/// directly from FAT) and not use the libXM7 ARM9 part of that library. So,
/// since the ARM7 part is the core of this library, it can be used
/// independently.
///
/// libnds comes with a really handy interface for FIFO based Interprocessor
/// Communication. The example sourcecode included above uses this interface.
///
/// The ARM7 part of the library interacts directly with the Nintendo DS
/// hardware.  More precisely, it interacts with:
///
/// - As many audio channels as needed for the tune starting from channel 15 on,
///   allocating channels 'backwards' so that if your module has 6 channels then
///   channels 10 through 15 are busy and the first DS hardware channels, from 0
///   to 9, are free. They will remain completely untouched by the library so
///   that you can use your libnds sound functions directly without any
///   modification needed.
///
/// - The timer number 1 on ARM7: it's the heartbeat that imposes the correct
///   speed to the module. Of course, the library sets the corresponding
///   interrupt too.
///
/// As already mentioned, it doesn't interact with anything else, in particular:
///
/// - It doesn't turn on or off the speakers.
///
/// - It doesn't raise or lower the main volume.
///
/// So you have to take care of both according to your needs.

#ifndef LIBXM7_H__
#define LIBXM7_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <nds/ndstypes.h>

/// List of the error codes for XM7_LoadXM() and XM7_LoadMOD().
///
/// They are all fatal errors; the loading will be interrupted and it won't be
/// possible to play the module. The `XM7_UnloadXM()` function (or its alias
/// `XM7_UnloadMOD()`) should be called anyway to free the already allocated
/// memory when the error code is greater than `0x07`.
typedef enum {
    XM7_NO_ERROR                           = 0x00,
    XM7_ERR_NOT_A_VALID_MODULE             = 0x01,
    XM7_ERR_UNKNOWN_MODULE_VERSION         = 0x02,
    XM7_ERR_UNSUPPORTED_NUMBER_OF_CHANNELS = 0x03,
    XM7_ERR_UNSUPPORTED_PATTERN_HEADER     = 0x08,
    XM7_ERR_INCOMPLETE_PATTERN             = 0x09,
    XM7_ERR_UNSUPPORTED_INSTRUMENT_HEADER  = 0x10,
    XM7_ERR_NOT_ENOUGH_MEMORY              = 0x100
} XM7_Error;

/// Panning styles.
typedef enum {
    /// Panning will be driven by the module. Default for XMs.
    XM7_PANNING_TYPE_NORMAL = 0x00,
    /// Panning will be driven by the channel number. Default for MODs.
    XM7_PANNING_TYPE_AMIGA  = 0x01
} XM7_PanningStyles;

/// Panning displacement styles.
typedef enum {
    /// The panning as it was meant on Amiga.
    XM7_HARD_PANNING_DISPLACEMENT    = 0,
    /// All the channels will be centered.
    XM7_MONO_PANNING_DISPLACEMENT    = 64,

    /// Default. Panning mix: 1/3 of the left channels volume goes to the right
    /// speaker and 1/3 of the right channels volume goes to the left speaker.
    /// Quite common setting among MOD players. 42 = ~127/3 = panning 1/3 + 2/3
    XM7_DEFAULT_PANNING_DISPLACEMENT = 42
} XM7_PanningDisplacementStyles;

/// Replay style flags.
typedef enum {
    XM7_REPLAY_STYLE_XM_PLAYER           = 0x00,
    XM7_REPLAY_STYLE_MOD_PLAYER          = 0x01,
    XM7_REPLAY_ONTHEFLYSAMPLECHANGE_FLAG = 0x02,

    /// Alias of XM7_REPLAY_STYLE_XM_PLAYER. The default for XM modules.
    XM7_REPLAY_STYLE_FT2 = XM7_REPLAY_STYLE_XM_PLAYER,
    /// The default for MOD modules
    XM7_REPLAY_STYLE_PT  = XM7_REPLAY_STYLE_MOD_PLAYER | XM7_REPLAY_ONTHEFLYSAMPLECHANGE_FLAG
} XM7_ReplayStyles;

typedef struct {
    u8 Note;            // 0 = no note; 1..96 = C-0...B-7; 97 = key off
    u8 Instrument;      // 0 or 1..128
    u8 Volume;
    u8 EffectType;
    u8 EffectParam;
}__attribute__ ((packed)) XM7_SingleNote_Type;

typedef struct {
    XM7_SingleNote_Type Noteblock[1];
}__attribute__ ((packed)) XM7_SingleNoteArray_Type;

typedef struct {
    s8 Data[1];
}__attribute__ ((packed)) XM7_SampleData_Type;

typedef struct {
    s16 Data[1];
}__attribute__ ((packed)) XM7_SampleData16_Type;

typedef struct {

    union {
        XM7_SampleData_Type* SampleData;
        XM7_SampleData16_Type* SampleData16;
    };

    u32 Length;         // length of data, always in bytes

    u32 LoopStart;
    u32 LoopLength;

    char Name[22];

    u8 Volume;
    u8 Panning;         // (0..0xFF, left to right. 0x80 = center )

    s8 RelativeNote;    //  (note relative to C-4)
    s8 FineTune;        //  (finetune, in 128th of an half-tone)

    u8 Flags;           //  bit 0: it has a loop
                        //  bit 4: it's a 16 bit sample

} XM7_Sample_Type;

typedef struct {

    u16 x;
    u16 y;

} XM7_EnvelopePoints_Type;

typedef struct {

    XM7_Sample_Type* Sample[16];    // pointers to the samples

    XM7_EnvelopePoints_Type VolumeEnvelopePoint[12];    //  -- Envelope points: x,y...x,y....
    XM7_EnvelopePoints_Type PanningEnvelopePoint[12];   //  -- in couples (2 words/point => a maximum of 12 points).

    u32 VibratoSweep;               //  0..0x10000

    u16 VolumeFadeout;

    char Name[22];

    u8 SampleforNote[96];           // sample number for note 1..96

    u8 NumberofSamples;             // 0..16  (zero... no samples!)

    u8 NumberofVolumeEnvelopePoints;
    u8 NumberofPanningEnvelopePoints;

    u8 VolumeSustainPoint;
    u8 VolumeLoopStartPoint;
    u8 VolumeLoopEndPoint;

    u8 PanningSustainPoint;
    u8 PanningLoopStartPoint;
    u8 PanningLoopEndPoint;

    u8 VolumeType;                  //  bit 0: On; 1: Sustain; 2: Loop
    u8 PanningType;                 //  bit 0: On; 1: Sustain; 2: Loop

    u8 VibratoType;                 //  0..3 (wave type)
    u8 VibratoDepth;                //  0..0x0F
    u8 VibratoRate;                 //  0..0xFF  ( 0..0x3F on FT2)

} XM7_Instrument_Type;

typedef struct {

    // status of the engine/module/playback
    u16 State;                  // bit 15: error if set
                                // bit 14: module loaded if set
                                // bit 13: module playing if set
    u16 ModuleLength;
    u16 RestartPoint;
    u16 NumberofPatterns;       // ...up to 256

    // -

    u8 NumberofChannels;        // 1..16 ok, > 16 NO!
    u8 NumberofInstruments;     // 1..128
    u8 FreqTable;               // Frequency table  ( 0= Amiga , 1= linear )
    u8 DefaultTempo;            // 1..31  (ticks per 'line', default 6)

    // -

    u8 DefaultBPM;                  // 32..255 (BeatPerMinute)

    // Should panning be 'fixed' in the Amiga style?
    XM7_PanningStyles AmigaPanningEmulation;
    // Amiga panning 'displacement' will be added to 0x00 / subtracted to 0x7F
    XM7_PanningDisplacementStyles AmigaPanningDisplacement;

    XM7_ReplayStyles ReplayStyle;   // bit 0: 0 = FT2 style
                                    //        1 = MOD player style
                                    // bit 1: on-the-fly sample change flag (1=ON)

    // -

    // these _could_ change during the playback!
    u8 CurrentTempo;            // copy of DefaultTempo (at startup)
    u8 CurrentBPM;              // copy of DefaultBPM   (at startup)
    u8 CurrentGlobalVolume;     // 0x40                 (at startup)

    u8 CurrentSongPosition;     // the pattern in playback now (position in the PatternOrder array)
    u8 CurrentPatternNumber;    // the pattern in playback now (its number!)
    u8 CurrentLine;             // the line in playback now (0..length of the pattern)
    u8 CurrentTick;             // the tick in playback now (0..31)

    u8 CurrentDelayLines;       // the lines to "delay" after this line (effect EEx)
    // -

    u16 CurrentDelayTick;       // the ticks to "delay" after this line (effect EEx)
    u16 CurrentAdditionalTick;  // the "additional" tick we are in (effect EEx)

    // -

    u8 PatternOrder[256];       // play the patterns in this order (PatternNumber is 0..255)

    u16 PatternLength[256];     // the length (in lines) of each pattern (min 1, max 256)  (default=64!)

    XM7_SingleNoteArray_Type* Pattern[256]; // pointer to the beginning of every single pattern
                                            // length = (PatternLength)*(NumberofChannels)*5  (bytes)

    XM7_Instrument_Type* Instrument[128];   // pointer to the instruments

    // -

    u8 CurrentSampleVolume[16];     // the volume of the sample on this channel ( 0..0x40 )
    u8 CurrentSamplePanning[16];    // the panning of sample in this channel    ( 0..0xFF left to right, 0x80 = center )

    u8 CurrentSampleVolumeEnvelopeState[16];    // the volume envelope state of the sample on this channel
    u8 CurrentSamplePanningEnvelopeState[16];   // the panning envelope state of the sample on this channel

    u8 CurrentSampleVolumeEnvelope[16];         // the envelope volume of the sample on this channel ( 0..0x40 )
    u8 CurrentSamplePanningEnvelope[16];        // the envelope panning of the sample on this channel ( 0..0x40 )

    u16 CurrentSampleVolumeEnvelopePoint[16];   // the volume envelope point (x) of the sample on this channel
    u16 CurrentSamplePanningEnvelopePoint[16];  // the panning envelope point (x) of the sample on this channel

    u16 CurrentSampleVolumeFadeOut[16];         // the fadeout volume of the sample on this channel  ( 0..0x8000 )

    s32 CurrentSamplePortamento[16];        // the period pitch  (for 1xx, 2xx, 3xx, E1x, E2x, X1x, X2x)
    s32 CurrentSamplePortaDest[16];         // the period pitch to achieve (for 3xx and 5xx)

    u8 CurrentGlissandoType[16];            // the glissando style (0 = fretless; 1 = round to nearest)

    u8 CurrentChannelLastNote[16];          // the number of the last note triggered in this channel        [1 - 96]
    u8 CurrentChannelLastInstrument[16];    // the number of the last instrument triggered in this channel  [1 - 128]

    u8 CurrentChannelIsArpeggioedNote[16];  // flag: the note is being arpeggioed

    u8 CurrentLoopBegin[16];                // which line to loop to when meeting E6x effect
    u8 CurrentLoopCounter[16];              // how many times I'm looping
    u8 CurrentLoopEnd[16];                  // the line this looping ends

    s8 CurrentVibratoValue[16];             // the vibrato value we should add
    u8 CurrentVibratoType[16];              // which waveform are we using      [0..2, 4..6]
    u8 CurrentVibratoPoint[16];             // where we are in the vibrato wave [0..63]

    s8 CurrentTremoloVolume[16];            // the volume we should add/sub because of tremolo
    u8 CurrentTremoloType[16];              // which tremolo waveform are we using [0..2, 4..6]
    u8 CurrentTremoloPoint[16];             // where we are in the tremolo wave    [0..63]

    u8 CurrentTremorPoint[16];              // where we are in the tremor square wave  [0..x+y+2]
    u8 CurrentTremorMuting[16];             // 1 = Tremor is muting.

    u8  CurrentAutoVibratoPoint[16];        // modulator wave is 8 bit for 1 period
    u32 CurrentAutoVibratoSweep[16];        // values 0..0x10000

    s8 CurrentFinetuneOverride[16];         // this is the value for overriding finetune
    u8 CurrentFinetuneOverrideOn[16];       // the flag...

    u8 Effect1xxMemory[16]; // the memory for the 1xx effect             [0x00..0xFF]
    u8 Effect2xxMemory[16]; // the memory for the 2xx effect             [0x00..0xFF]
    u8 Effect3xxMemory[16]; // the memory for the 3xx effect (and Mx)    [0x00..0xFF]
    u8 Effect4xxMemory[16]; // the memory for the 4xx effect (and Vx/Sx) [0x00..0xFF]
    u8 Effect7xxMemory[16]; // the memory for the 7xx effect             [0x00..0xFF]
    u8 Effect9xxMemory[16]; // the memory for the 9xx effect (YES! has memory!) [0x00..0xFF]
    u8 EffectAxyMemory[16]; // the memory for the Axy (and 5xx,6xx) effect [0x01..0xF0]
    u8 EffectE1xMemory[16]; // the memory for the E1x effect            [1..0xF]
    u8 EffectE2xMemory[16]; // the memory for the E2x effect            [1..0xF]
    u8 EffectEAxMemory[16]; // the memory for the EAx (and EBx) effect  [1..0xF]
    u8 EffectHxyMemory[16]; // the memory for the Hxy effect            [0x01..0xF0]
    u8 EffectPxyMemory[16]; // the memory for the Pxy effect            [0x01..0xF0]
    u8 EffectRxyMemory[16]; // the memory for the Rxy effect            [0x00..0xFF]
    u8 EffectTxyMemory[16]; // the memory for the Txy effect            [0x11..0xFF]
    u8 EffectX1xMemory[16]; // the memory for the X1x effect            [1..0xF]
    u8 EffectX2xMemory[16]; // the memory for the X2x effect            [1..0xF]

    u32 Silence;            // ... a silent 4-bytes sample

    // -

    char ModuleName[20];
    char TrackerName[20];

} XM7_ModuleManager_Type;

#ifdef ARM7

/// Perform initialization of the library.
///
/// This function should be called just once, before calling any other function.
void XM7_Initialize(void);

/// This function makes the module start playing.
///
/// It doesn't turn on the speakers, nor does it set the master volume: all that
/// is up to you. The following parameter should be passed to the function: a
/// pointer to a XM7_ModuleManager_Type structure where an XM module should have
/// already been loaded.
///
/// @param TheModule Module to be played.
void XM7_PlayModule(XM7_ModuleManager_Type *TheModule);

/// This function makes the module start playing from a given position.
///
/// It works like XM7_PlayModule() but requires one more parameter: the position
/// (in the *Pattern Order Table*) where the replay should start. Using this
/// function and tracking XMs in 'sections' you can actually pack many tunes
/// sharing the same instruments in a single XM file. When it's required to
/// switch to a different tune you would then simply stop the module and play
/// the same module again just starting from a different position.
///
/// @param TheModule Module to be played.
/// @param position Position in Pattern Order Table.
void XM7_PlayModuleFromPos(XM7_ModuleManager_Type *TheModule, u8 position);

/// This function stops the module.
///
/// It abruptly interrupts every sample of the module being played.
void XM7_StopModule(void);

#endif // ARM7

// ARM9 functions (... well, you can use them even on ARM7 if you want...)

/// Load an XM into a `XM7_ModuleManager_Type` structure.
///
/// Both parameters are pointers; the first one should point to an already
/// allocated structure where this function will load the XM module, whereas the
/// second is the pointer to a copy in memory of a whole XM file. This function
/// uses `malloc()` to allocate space for patterns, instruments and samples into
/// the heap. Unlike the other functions, this function does return a value,
/// which is 0 when the loading is successful and a different value when the
/// loading has a different outcome.
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
/// @param XMModule_ Pointer to the XM file in RAM.
/// @return Error code.
XM7_Error XM7_LoadXM(XM7_ModuleManager_Type *Module, const void *XMModule_);

/// This function frees all the allocated memory thus unloading the module.
///
/// It doesn't deallocate the `XM7_ModuleManager_Type` structure.
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
void XM7_UnloadXM(XM7_ModuleManager_Type *Module);

/// Load a MOD into a `XM7_ModuleManager_Type` structure.
///
/// Both parameters are pointers; the first one should point to an already
/// allocated structure where this function will load the module, whereas the
/// second is the pointer to a copy in memory of a whole MOD file. This function
/// uses `malloc()` to allocate space for patterns, instruments and samples into
/// the heap. Unlike the other functions, this function does return a value,
/// which is 0 when the loading is successful and a different value when the
/// loading has a different outcome.
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
/// @param MODModule_ Pointer to the MOD file in RAM.
/// @return Error code.
XM7_Error XM7_LoadMOD(XM7_ModuleManager_Type* Module, const void* MODModule_);

/// This function frees all the allocated memory thus unloading the module.
///
/// It doesn't deallocate the `XM7_ModuleManager_Type` structure.
///
/// This function is just an alias of XM7_UnloadXM(), for your convenience.
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
void XM7_UnloadMOD(XM7_ModuleManager_Type *Module);

/// Setup the replay style of the module.
///
/// This function sets some parameters that affect the way the module will be
/// reproduced, mainly because there are some differencies in some effect
/// behaviour if used in XM or MOD modules. Actually this simply affects the
/// effect `0xy` (Arpeggio) and activates/deactivates the 'on-the-fly sample
/// change' feature.
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
/// @param style Replay style.
void XM7_SetReplayStyle(XM7_ModuleManager_Type* Module, XM7_ReplayStyles style);

/// This function configures how the panning is managed in the reproduction.
///
/// XM format has panning effects, instrument panning settings and even panning
/// envelopes, whereas MOD format has virtually none and originally (on Amiga
/// machines) mapped channels directly to the left speaker OR to the right
/// speaker only, following the rule that the first channel goes right and the
/// second goes left, then the opposite for the next two and swap again for the
/// fifth and sixth and so on (R-L-L-R-R-L-L-R ...).
///
/// Then, if the XM7_PanningStyles used is `XM7_PANNING_TYPE_AMIGA`, you can
/// specity a value for the displacement, this will be the 'distance' from the
/// originally assigned speaker, maximum value is 127 (which actually will
/// completely swap channels...).
///
/// @param Module Pointer to an allocated XM7_ModuleManager_Type structure.
/// @param style Panning style.
/// @param displacement Displacement style.
void XM7_SetPanningStyle(XM7_ModuleManager_Type* Module, XM7_PanningStyles style,
                         XM7_PanningDisplacementStyles displacement);

#ifdef __cplusplus
}
#endif

#endif // LIBXM7_H__
