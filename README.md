# Dragonboard Voice Recognition

This is a demo/example of how to get offline voice recognition working on a Qualcomm Dragonboard

***

There are various OS, Audio drivers, and libraries combination to use, but here are the current examples this repo has to offer

- [Linux - ALSA - PocketSphinx](#linux---ALSA---PocketSphinx)

## Linux - ALSA - PocketSphinx

> Tested on Linaro 4.9.56
>
> Assuming general knowledge of getting around a linux command line

### Update Linux

First we need to update and install a few tools

- `sudo apt update`
- `sudo apt upgrade`
- `sudo apt install autoconf libtool automake bison python-dev swig libasound2-dev`

### Get PocketSphinx and SphinxBase

- `git clone https://github.com/cmusphinx/pocketsphinx.git`
- `git clone https://github.com/cmusphinx/sphinxbase.git`

### Remove PulseAudio library header

You can run `dpkg -l | grep libpulse-dev` to see if you actually have PulseAudio dev libs currently

We need to make sure PulseAudio is off because we are using ALSA and Sphixbase checks in that order so you either need to:

- run `apt purge libpulse-dev` to remove PulseAudio library headers (**Note** this is not actually uninstalling PulseAudio that may be in use)

or

- Change `configure.ac` in sphinxbase to not have `AC_CHECK_HEADER(pulse/pulseaudio.h` so it doesn't check for it over ALSA

### Setting up

- `cd sphinxbase`
- `./autogen.sh`
- `make`
- `sudo make install`
- `cd ../pocketsphinx`
- `./autogen.sh`
- `make`
- `sudo make install`

## Run

- `export LD_LIBRARY_PATH=/usr/local/lib`
- `export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig`

- `pocketsphinx_continuous -adcdev plughw:0,2 -infile <wav.file>`
- `pocketsphinx_continuous -adcdev plughw:0,2 -inmic yes`
    - Note with ALSA it will complain about buffer size being overwritten too fast, this is not an issue when running a custom program

- [how to upload a custom recognition file](http://www.speech.cs.cmu.edu/tools/lmtool-new.html)
- `pocketsphinx_continuous -adcdev plughw:0,2 -inmic yes -dict <custom_dictionary.dic> -lm <custom_language_model.lm>`
