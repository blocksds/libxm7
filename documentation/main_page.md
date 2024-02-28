LibXM7 documentation {#mainpage}
====================

## Introduction

A C library to play XM (and MOD) modules on Nintendo DS using only ARM7
resources (thus freeing your ARM9 from that task).

## API documentation

- @ref libxm7_types
- @ref libxm7_arm9
- @ref libxm7_arm7

## libXM7 Replay features

The current version of libXM7 (1.06) can play XMs featuring the following
characteristics:

- Up to 16 channels.
- Distinct volume and effect column for each channel.
- Module length up to 256 patterns.
- Module can contain up to 256 different patterns of variable length from 1 to
  256 lines.
- Up to 128 instruments, each one of which:

  - Has up to 16 different 8 or 16 bit samples, each one having a forward
    (normal) or a ping-pong loop optionally.
  - May have a volume envelope with up to 12 points, optionally with a
    sustain point and/or a loop, plus a release (fade out) rate.
  - May have a panning envelope with up to 12 points, optionally with a
    sustain point and/or a loop.
  - May have a vibrato definition with speed, depth and sweep.

- Both *linear frequency table* mode (XM native) and *Amiga frequency table*
  mode (MOD native) are supported for completeness, even if the latter is very
  uncommon nowadays

libXM7 has **complete** effect support. For reference, these are the effects:

- `0xy` Arpeggio
- `1xx` Portamento up (M)
- `2xx` Portamento down (M)
- `3xx` Portamento to note (M)
- `4xy` Vibrato (M)
- `5xy` Portamento to note with volume slide (M)
- `6xy` Vibrato with volume slide (M)
- `7xy` Tremolo (M)
- `8xx` Set note panning
- `9xx` Set sample offset (M)
- `Axy` Slide note volume up or down (M)
- `Bxx` Jump to position
- `Cxx` Set note volume
- `Dxx` Pattern break
- `E1x` Fine portamento up (M)
- `E2x` Fine portamento down (M)
- `E3x` Glissando control
- `E4x` Vibrato control
- `E5x` Instrument finetune
- `E6x` Pattern loop
- `E7x` Tremolo control
- `E8x` Set note panning
- `E9x` Retrig note
- `EAx` Slide note fine volume up (M)
- `EBx` Slide note fine volume down (M)
- `ECx` Note cut
- `EDx` Note delay
- `EEx` Pattern delay
- `Fxx` Set song speed
- `Gxx` Set global volume
- `Hxy` Slide global volume up or down (M)
- `Kxx` Key off
- `Lxx` Set envelope position
- `Pxy` Panning slide (M)
- `Rxy` Retrig note with volume slide (M)
- `Txy` Tremor (M)
- `X1x` Extra fine portamento up (M)
- `X2x` Extra fine portamento down (M)

(M) Means that this effect has "memory".

The volume column effects support is also **complete**. For your reference the
effects follows:

- `xx` Set note volume (`0x10-0x50`)
- `Dx` Slide note volume down (`0x60-0x6F`)
- `Ux` Slide note volume up (`0x70-0x7F`)
- `dx` Slide note fine volume down (`0x80-0x8F`)
- `ux` Slide note fine volume up (`0x90-0x9F`)
- `Sx` Set vibrato speed (`0xA0-0xAF`)
- `Vx` Vibrato (`0xB0-0xBF`)
- `Px` Set note panning (`0xC0-0xCF`)
- `Lx` Slide note panning left (`0xD0-0xDF`)
- `Rx` Slide note panning right (`0xE0-0xEF`)
- `Mx` Portamento to note (`0xF0-0xFF`)

There's a very good document explaining XM effects (and volume column effects)
usage [here](https://milkytracker.org/docs/MilkyTracker.html), on MilkyTracker
website.

The library can play also MODs featuring the following characteristics:

- `M.K.`, `M!K!` and `FLT4` module subtypes 4 channels modules
- `OCTA`, `CD81` and `FLT8` (\*) module subtypes 8 channels modules
- `TDZx` module subtype 1 to 3 channels modules
- `xCHN` module subtype 2 to 9 channels modules
- `xxCH` module subtype 10 to 16 channels modules
- Distinct effect column for each channel
- Module length up to 128 patterns
- Module can contain up to 256 different patterns of fixed length of 64 lines
  each.
- Up to 31 instruments, one 8 bits sample each, having a forward (normal) loop
  optionally.

Of course libXM7 has **complete** MOD effect support (because they're a subset
of XM effects).

(\*) Yeah, I mean `FLT8` really.

## Getting the most of your tune

Since the library doesn't manipulate the samples data directly but simply uses
the Nintendo DS hardware 16 digital channels to replay samples, there are some
things you should be aware of when composing tunes especially for this system.
First of all, remember that hardware mixing among the channels on a Nintendo DS
will result in a 10bits/32kHz stereo sound being reproduced. It's not poor
quality, but it isn't exactly CD quality either.  In the mixing process the
samples don't get resampled when pitched so make sure you disable this feature
in your tracker (*sample interpolation*) if you want to simulate the output you
would get on the DS as better as possible.

The DS hardware also wants all the figures to be in 32bit boundaries, which
means that the samples should conform to some rules:

- Length of samples *without loop* should be *multiple* of 4 for 8 bits samples
  and should be even for 16 bits samples.

- The length of the **non-repeating** part of samples *with loops* (of any kind)
  and the length of the **repeating** part of samples *with forward loops*
  should be as stated above.

- Length of the **repeating** part of 8 bits samples *with ping-pong loops*
  should be *even*. No problems with *ping-pong loops* of 16 bits samples (this
  happens because DS hardware doesn't support ping-pong loops, so these loops
  are converted into forward loops by the loading routines).

A side effect (*detuning*) is possible when these rules are ignored, and it
could be perceivable on samples with loops, especially when the repeating part
of the sample is very short.

Finally, it's impossible for the DS hardware to start playing a sample that has
a loop from inside the loop from other position but the beginning. This means
that you should put some care in preparing your samples having loops if you plan
to use the `9xx` (*Set sample offset*) effect on them. Anyway there are no
problems if you simply plan to skip samples of the **non-repeating** part of a
sample with loop.

## How to use libXM7 files

The library consists of one header file (`libxm7.h`) and two archive files
(`libxm77.a` and `libxm79.a`). You should add the path with the header file to
your include paths, say the `include/libxm7` directory. On the other hand, you
should add the `libs` directory to your linker paths. Then you should modify the
makefiles for your project: locate the line where the libraries are listed
(`LIBS := -l<library> -l<library>`) and add `-lxm77` in the ARM7 makefile and
`LIBDIRS := path/to/libxm7/libs`. Add also `-lmx79` in the ARM9 makefile if you
plan to use both libXM7 library parts. Make sure that `-lxm77` appears *before*
`-lnds7` and that `-lxm79` appears *before* `-lnds9`.

## Inside libXM7 library

libXM7 is a combined library: it has one part that is linked to the ARM9
executable and another part that is linked to the ARM7 executable. However the
library implements no means of communication between the processors. So it's up
to the programmer to choose a suitable way to communicate, let's say, to the
ARM7 where the ARM9 has loaded the module that the former should play. Also, the
programmer is supposed to ensure that the `XM7_StopModule()` gets executed on
ARM7 before `XM7_UnloadXM()` gets executed on ARM9. By the way, you can even
write your own module loader (for instance one that reads directly from FAT) and
not use the libXM7 ARM9 part of that library. So, since the ARM7 part is the
core of this library, it can be used independently.

libnds comes with a really handy interface for FIFO based Interprocessor
Communication. The example sourcecode included above uses this interface.

The ARM7 part of the library interacts directly with the Nintendo DS hardware.
More precisely, it interacts with:

- As many audio channels as needed for the tune starting from channel 15 on,
  allocating channels 'backwards' so that if your module has 6 channels then
  channels 10 through 15 are busy and the first DS hardware channels, from 0 to
  9, are free. They will remain completely untouched by the library so that you
  can use your libnds sound functions directly without any modification needed.

- The timer number 0 on ARM7: it's the heartbeat that imposes the correct speed
  to the module. Of course, the library sets the corresponding interrupt too.

As already mentioned, it doesn't interact with anything else, in particular:

- It doesn't turn on or off the speakers.

- It doesn't raise or lower the main volume.

So you have to take care of both according to your needs.
