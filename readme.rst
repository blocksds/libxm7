======
libxm7
======

A C library to play XM (and MOD) modules on Nintendo DS using only ARM7
resources (thus freeing your ARM9 from that task).

Homepage: http://wcms.teleion.it/users/cgq/nds/libxm7/

This is released as-is, under the MIT license.

- Demos of `version 0.81 <https://www.youtube.com/watch?v=DOTtW3GKVTs>`_ and
  `version 0.93 <https://vimeo.com/3364671>`_. Thanks, Raina!

- `AngunaDS <http://www.tolberts.net/anguna/>`_ (the *first* homebrew featuring
  libXM7)

`Changelog <./changelog.txt>`_

libXM7 Replay features
======================

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
  - May have a volume envelope with up to 12 points, optionally with a sustain
    point and/or a loop, plus a release (fade out) rate.
  - May have a panning envelope with up to 12 points, optionally with a sustain
    point and/or a loop.
  - May have a vibrato definition with speed, depth and sweep.

- Both *linear frequency table* mode (XM native) and *Amiga frequency table*
  mode (MOD native) are supported for completeness, even if the latter is very
  uncommon nowadays

libXM7 has **complete** effect support. For reference, these are the effects:

- ``0xy`` Arpeggio
- ``1xx`` Portamento up (M)
- ``2xx`` Portamento down (M)
- ``3xx`` Portamento to note (M)
- ``4xy`` Vibrato (M)
- ``5xy`` Portamento to note with volume slide (M)
- ``6xy`` Vibrato with volume slide (M)
- ``7xy`` Tremolo (M)
- ``8xx`` Set note panning
- ``9xx`` Set sample offset (M)
- ``Axy`` Slide note volume up or down (M)
- ``Bxx`` Jump to position
- ``Cxx`` Set note volume
- ``Dxx`` Pattern break
- ``E1x`` Fine portamento up (M)
- ``E2x`` Fine portamento down (M)
- ``E3x`` Glissando control
- ``E4x`` Vibrato control
- ``E5x`` Instrument finetune
- ``E6x`` Pattern loop
- ``E7x`` Tremolo control
- ``E8x`` Set note panning
- ``E9x`` Retrig note
- ``EAx`` Slide note fine volume up (M)
- ``EBx`` Slide note fine volume down (M)
- ``ECx`` Note cut
- ``EDx`` Note delay
- ``EEx`` Pattern delay
- ``Fxx`` Set song speed
- ``Gxx`` Set global volume
- ``Hxy`` Slide global volume up or down (M)
- ``Kxx`` Key off
- ``Lxx`` Set envelope position
- ``Pxy`` Panning slide (M)
- ``Rxy`` Retrig note with volume slide (M)
- ``Txy`` Tremor (M)
- ``X1x`` Extra fine portamento up (M)
- ``X2x`` Extra fine portamento down (M)

(M) Means that this effect has "memory".

The volume column effects support is also **complete**. For your reference the
effects follows:

- ``xx`` Set note volume (``0x10-0x50``)
- ``Dx`` Slide note volume down (``0x60-0x6F``)
- ``Ux`` Slide note volume up (``0x70-0x7F``)
- ``dx`` Slide note fine volume down (``0x80-0x8F``)
- ``ux`` Slide note fine volume up (``0x90-0x9F``)
- ``Sx`` Set vibrato speed (``0xA0-0xAF``)
- ``Vx`` Vibrato (``0xB0-0xBF``)
- ``Px`` Set note panning (``0xC0-0xCF``)
- ``Lx`` Slide note panning left (``0xD0-0xDF``)
- ``Rx`` Slide note panning right (``0xE0-0xEF``)
- ``Mx`` Portamento to note (``0xF0-0xFF``)

There's a very good document explaining XM effects (and volume column effects)
usage `here <https://milkytracker.org/docs/MilkyTracker.html>`_, on MilkyTracker
website.

The library can play also MODs featuring the following characteristics:

- ``M.K.``, ``M!K!`` and ``FLT4`` module subtypes 4 channels modules
- ``OCTA``, ``CD81`` and ``FLT8`` (*) module subtypes 8 channels modules
- ``TDZx`` module subtype 1 to 3 channels modules
- ``xCHN`` module subtype 2 to 9 channels modules
- ``xxCH`` module subtype 10 to 16 channels modules
- Distinct effect column for each channel
- Module length up to 128 patterns
- Module can contain up to 256 different patterns of fixed length of 64 lines
  each.
- Up to 31 instruments, one 8 bits sample each, having a forward (normal) loop
  optionally.

Of course libXM7 has **complete** MOD effect support (because they're a subset of
XM effects).

(*) Yeah, I mean ``FLT8`` really.

Getting the most of your tune
=============================

Since the library doesn't manipulate the samples data directly but simply uses
the Nintendo DS hardware 16 digital channels to replay samples, there are some
things you should be aware of when composing tunes especially for this system.
First of all, remember that hardware mixing among the channels on a Nintendo DS
will result in a 10bits/32kHz stereo sound being reproduced. It's not poor
quality, but it isn't exactly CD quality either. In the mixing process the
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
to use the ``9xx`` (*Set sample offset*) effect on them. Anyway there are no
problems if you simply plan to skip samples of the **non-repeating** part of a
sample with loop.

How to use libXM7 files
=======================

The library consists of one header file (``libxm7.h``) and two archive files
(``libxm77.a`` and ``libxm79.a``). You should unpack the header file in a
subdirectory of your include path, say the ``include/libxm7`` directory. On the
other hand, the archive files should go to your ``libnds`` library path, where
your other archive files already are. Then you should modify the makefiles for
your project: locate the line where the libraries are listed
(``LIBS := -l<library> -l<library>``) and add ``-lxm77`` in the ARM7 makefile.
Add also ``-lmx79`` in the ARM9 makefile if you plan to use both libXM7 library
parts. Make sure that ``-lxm77`` appears *before* ``-lnds7`` and that ``-lxm79``
appears *before* ``-lnds9``.

How to use libXM7 ARM7 functions
================================

First of all, you should include ``libxm7.h`` in your ARM7 source file. Doing it
will give you access to the defined functions:

.. code:: c

    void XM7_Initialize(void)

This function should be called just once, before calling any other function. It
performs some important necessary initialization.

.. code:: c

    void XM7_PlayModule(XM7_ModuleManager_Type* TheModule)

This function makes the module start playing. It doesn't turn on the speakers,
nor does it set the master volume: all that is up to you. The following
parameter should be passed to the function: a pointer to a
``XM7_ModuleManager_Type`` structure where an XM module should have already been
loaded.

.. code:: c

    void XM7_PlayModuleFromPos(XM7_ModuleManager_Type* TheModule, u8 position)

This function makes the module start playing from a given position. It works
like the previous one but requires one more parameter: the position (in the
*Pattern Order Table*) where the replay should start. Using this function and
tracking XMs in 'sections' you can actually pack many tunes sharing the same
instruments in a single XM file. When it's required to switch to a different
tune you would then simply stop the module and play the same module again just
starting from a different position.

.. code:: c

    void XM7_StopModule(void)

This function stops the module and it abruptly interrupts every sample of the
module being played.

How to use libXM7 ARM9 functions
================================

First, include ``libxm7.h`` in your ARM9 source file. The defined functions are:

.. code:: c

    u16 XM7_LoadXM(XM7_ModuleManager_Type*, XM7_XMModuleHeader_Type*)

This function loads an XM into the ``XM7_ModuleManager_Type`` structure that allows
the player to work. Both parameters are pointers; the first one should point to
an already allocated structure where this function will load the XM module,
whereas the second is the pointer to a copy in memory of a whole XM file. This
function uses ``malloc()`` to allocate space for patterns, instruments and
samples into the heap. Unlike the other functions, this function does return a
value, which is 0 when the loading is successful and a different value when the
loading has a different outcome. Error codes are discussed further on.

.. code:: c

    u16 XM7_LoadMOD(XM7_ModuleManager_Type*, XM7_MODModuleHeader_Type*)

This function loads a MOD into the ``XM7_ModuleManager_Type`` structure that
allows the player to work. Both parameters are pointers; the first one should
point to an already allocated structure where this function will load the
module, whereas the second is the pointer to a copy in memory of a whole MOD
file. This function uses ``malloc()`` to allocate space for patterns,
instruments and samples into the heap. Unlike the other functions, this function
does return a value, which is 0 when the loading is successful and a different
value when the loading has a different outcome. Error codes are discussed
further on.

.. code:: c

    void XM7_UnloadXM(XM7_ModuleManager_Type*)

This function frees all the allocated memory thus unloading the module. However,
it doesn't deallocate the ``XM7_ModuleManager_Type`` structure.

.. code:: c

    void XM7_UnloadMOD(XM7_ModuleManager_Type*)

This function is simply an alias of the ``XM7_UnloadXM()`` function, for your
convenience.

.. code:: c

    void XM7_SetReplayStyle(XM7_ModuleManager_Type* Module, u8 style)

This function sets some parameters that affect the way the module will be
reproduced, mainly because there are some differencies in some effect behaviour
if used in XM or MOD modules. Actually this simply affects the effect ``0xy``
(*Arpeggio*) and activates/deactivates the 'on-the-fly sample change' feature.
For your convenience some constants are defined:

.. code:: c

    #define XM7_REPLAY_STYLE_XM_PLAYER
    #define XM7_REPLAY_STYLE_MOD_PLAYER
    #define XM7_REPLAY_ONTHEFLYSAMPLECHANGE_FLAG

    // Currently an alias of XM7_REPLAY_STYLE_XM_PLAYER, the default for XM
    // modules.
    #define XM7_REPLAY_STYLE_FT2

    // The default for MOD modules, it's both XM7_REPLAY_STYLE_MOD_PLAYER and
    // XM7_REPLAY_ONTHEFLYSAMPLECHANGE_FLAG.
    #define XM7_REPLAY_STYLE_PT

    void XM7_SetPanningStyle(XM7_ModuleManager_Type* Module, u8 style, u8 displacement)

This function configures how the panning will be managed in the reproduction. XM
format has panning effects, instrument panning settings and even panning
envelopes, whereas MOD format has virtually none and originally (on Amiga
machines) mapped channels directly to the left speaker OR to the right speaker
only, following the rule that the first channel goes right and the second goes
left, then the opposite for the next two and swap again for the fifth and sixth
and so on (R-L-L-R-R-L-L-R ...). For your convenience again there are some
defined constants for the style:

.. code:: c

    // Panning will be driven by the module. Default for XMs.
    #define XM7_PANNING_TYPE_NORMAL

    // Panning will be driven by the channel number. Default for MODs.
    #define XM7_PANNING_TYPE_AMIGA

Then, if you select ``XM7_PANNING_TYPE_AMIGA``, you can specity a value for the
displacement, this will be the 'distance' from the originally assigned speaker,
maximum value is 127 (which actually will completely swap channels...). The
following common constants are defined:

.. code:: c

    // The panning as it was meant on Amiga.
    #define XM7_HARD_PANNING_DISPLACEMENT

    // All the channels will be centered.
    #define XM7_MONO_PANNING_DISPLACEMENT

    // Default. Panning mix: 1/3 of the left channels volume goes to the right
    // speaker and 1/3 of the right channels volume goes to the left speaker.
    // Quite common setting among MOD players.
    #define XM7_DEFAULT_PANNING_DISPLACEMENT

``XM7_LoadXM()`` and ``XM7_LoadMOD()`` error codes
==================================================

Here's the list of the error code defines for your reference. They are all fatal
errors; the loading will be interrupted and it won't be possible to play the
module. The ``XM7_UnloadXM()`` function (or its alias ``XM7_UnloadMOD()``)
should be called anyway to free the already allocated memory when the error code
is greater than ``0x07``.

.. code:: c

    #define XM7_ERR_NOT_A_VALID_MODULE              0x01
    #define XM7_ERR_UNKNOWN_MODULE_VERSION          0x02
    #define XM7_ERR_UNSUPPORTED_NUMBER_OF_CHANNELS  0x03
    #define XM7_ERR_UNSUPPORTED_PATTERN_HEADER      0x08
    #define XM7_ERR_INCOMPLETE_PATTERN              0x09
    #define XM7_ERR_UNSUPPORTED_INSTRUMENT_HEADER   0x10
    #define XM7_ERR_NOT_ENOUGH_MEMORY               0x100

Inside libXM7 library
=====================

libXM7 is a combined library: it has one part that is linked to the ARM9
executable and another part that is linked to the ARM7 executable. However the
library implements no means of communication between the processors. So it's up
to the programmer to choose a suitable way to communicate, let's say, to the
ARM7 where the ARM9 has loaded the module that the former should play. Also, the
programmer is supposed to ensure that the ``XM7_StopModule()`` gets executed on
ARM7 before ``XM7_UnloadXM()`` gets executed on ARM9. By the way, you can even
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

- The timer number 1 on ARM7: it's the heartbeat that imposes the correct speed
  to the module. Of course, the library sets the corresponding interrupt too.

As already mentioned, it doesn't interact with anything else, in particular:

- It doesn't turn on or off the speakers.

- It doesn't raise or lower the main volume.

So you have to take care of both according to your needs.

Contacts
========

Feel free to contact me through e-mail (my e-address is
``<my_nickname_goes_here>@yahoo.com``) or, even better, leave a GitHub issue in
this repository.  Bugs or replay accuracy problem reporting and every kind of
feedback is always welcome!

Acknowledgements
================

This work would have never seen the light if it wasn't for the following:

- devkitARM & libnds
- my friend (c)runX 's help in teaching me what's an XM, back in 1997.
- `raina <http://modarchive.org/index.php?request=view_profile&query=80599>`_'s
  priceless work (if you think that this library accuracy is good then you
  should know it's mainly because of his help!) and wonderful modules.
- `Strobe <http://modarchive.org/index.php?request=view_profile&query=69549>`_'s
  great help in finding lots of bugs while implementing new effects, and his
  cool modules.
- Rhinostrich, setrodox, Magic Fred, Romeo Knight, ogge and `Kmuland
  <http://modarchive.org/index.php?request=view_profile&query=81380>`_ for their
  modules and the permission to use them.

Sverx, 2011-01-27. Last modified 2023-03-28 by AntonioND.
