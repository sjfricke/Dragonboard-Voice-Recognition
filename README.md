# Dragonboard Voice Recognition

This is a demo/example of how to get offline voice recognition working on a Qualcomm Dragonboard

***

There are various OS, Audio drivers, and libraries combination to use, but here are the current examples this repo has to offer

- [Linux - ALSA - PocketSphinx](#linux---alsa---pocketsphinx)
- [Linux - PulseAudio - PocketSphinx](#linux---pulseaudio---pocketsphinx)
- [Linux - Google Speech API](#linux---google-speech-api)
- [Android - PocketSphinx](#android---pocketsphinx)
- [Android - Google Speech API](#android---google-speech-api)

***

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

### Custom Dictionary File

The default set of words is **really large** and also **really inaccurate** so I would strongly advise adding your own custom set of words

- First create a set of sentences and words in a text file like [this](Linux-ALSA-PocketSphinx/demo_word_list.txt)
- Use the [CMU online tool](http://www.speech.cs.cmu.edu/tools/lmtool-new.html) to create a `.dic` and `.lm` file
- Call the `.dic` and `.lm` when running the program

### Run

First you need to make sure you set `LD_LIBRARY_PATH` and `PKG_CONFIG_PATH` system variables

- `export LD_LIBRARY_PATH=/usr/local/lib`
- `export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig`

[For code examples of how to run PocketSphinx](Linux-ALSA-PocketSphinx)

There are three ways to run it via command line:

- With default library
	- `pocketsphinx_continuous -adcdev plughw:0,2 -inmic yes`
- With a .wav file
	- `pocketsphinx_continuous -adcdev plughw:0,2 -infile <wav.file>`
- With your custom dictionary, [here are the words in this set](Linux-ALSA-PocketSphinx/demo_word_list.txt)
	- `pocketsphinx_continuous -adcdev plughw:0,2 -inmic yes -dict ./Linux-ALSA-PocketSphinx/demo.dic -lm ./Linux-ALSA-PocketSphinx/demo.lm`

***

## Linux - PulseAudio - PocketSphinx

IN PROGRESS

## Linux - Google Speech API

IN PROGRESS

***

## Android - PocketSphinx

IN PROGRESS

***

## Android - Google Speech API

IN PROGRESS

