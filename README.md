# Cant Hear You!
This project tries to solve a problem which i have encountered a lot of times
during work at the company. So while programming, I do like a lot of people,
listen to music. That alone is of course not the issue, but it is an issue that
I use in-ears instead of bulky headphones. These are hard to spot, not only for
people who sit behind me but also people in front of me, who see only the upper
half of my head. This leads to situations where colleagues are trying to talk to
me, unaware that I'm not able to hear them. In best case they notice that I may
can't hear them, in worst case they think I actively ignore them.

However, to solve this, I thought it would be nice to have some visual sign for
others to signal them that I'm listening to music and probably can not hear
them.

This visual sign will be a digispark attiny85 with V-USB lib for direct USB
communication and a LED which is showing in some way that I'm listening to some
music.

## How it works
On linux it is pretty much straight-forward to check if there is any sound
running on a specific device because... on linux everything is a file... right?
So yeah, under `/proc/asound` are the configured cards of the ALSA Driver
stored which also contains a `status` file which shows if there is any sound
being played or not (for details see https://docs.kernel.org/sound/designs/procfile.html).


## How to build

	sudo apt update && sudo apt install -y \
		build-essential \
		gcc-avr \
		avr-libc \
		libusb-1.0-0-dev \

Additionally you also have to install on your host *micronucleus* to be able to
upload the firmware to your attiny85. For installation instructions take a look
at their github repo: https://github.com/micronucleus/micronucleus
