# 2025-exercise

## Raspberry Pi operating system
* Please visit this website to install the Raspberry Pi OS to your SD card: https://www.raspberrypi.com/software/
* Note that we will not be running the OS, but do need the boot files that are included in the installation.

## Dependencies:
* (For Part 1) install `cmake` and `mtools`
* (For Part 2) `circle` - add copy of `circle` project to your own project's home directory: https://github.com/rsta2/circle/tree/master
	* For this exercise, we recommend copying rather than cloning because otherwise git will detect that a subdirectory is being tracked. If you choose to clone it, note that you may be tracking a repo within a repo.

## Exercise Notes:
* Tested with Linux (WSL running Ubuntu 22.04.3 LTS)
* Directory paths, code, etc. suggested below are meant for guidance, you may need to adjust them for execution to work on your machine.

#### Part 1 Setup (LED flash)
* This part is based on open source work done by Brian Sidebotham (see included LICENSE file). Some of the instructions below come directly from the repository information, so we recommend browsing through https://github.com/BrianSidebotham/arm-tutorial-rpi
* Use included script to get Raspberry Pi firmware: `bash part-1/firmware/get_firmware_repo.sh`.
* Use included script to get compiler: `part-1/compiler/get_compiler.sh`. This script will also set up a `.compiler_config` file that points other scripts to the compiler toolchain path.
	* Alternatively, if you have an existing compiler toolchain and want to save space, modify `part-1/compiler/template.compiler_config` to your config, then save it as `part-1/compiler/.compiler_config`
* To build image, run `bash part-1/blinkLED/build.sh rpi[version #]`
	* This should execute the compiler toolchain with the appropriate options for your pi version
	* Output should be `.img` file; this will need to be renamed for your Raspberry Pi version (look online to see what kernel image name your Pi is expecting).
* Copy the file to your SD card, insert the card into the Pi, and then plug in the Pi via USB to power it up. If problems occur, delete all other kernel*.img files.

#### Part 2 Setup (TCP request with button):
* This part makes use of the open source `circle` project. Some of the instructions below come directly from the repository information, so we recommend browsing through https://github.com/rsta2/circle
* Get compiler toolchain: 
	* Option: can try to set `path` to compiler downloaded in part 1 (need to set to `[compiler toolchain directory]/bin`)
	* Option: can download from here: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads Ideally use most recently tested version: `arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi.tar.xz`
	* If needed, update your `.gitignore` so you are not tracking your compiler toolchain directory!
* If you choose to download the compiler toolchain:
	* Unpack into `circle` directory: `tar xf [filename.tar.xz]`
	* As mentioned earlier, you will probably need to add the directory with compiler toolchain to `path`; for example: `export PATH="$PATH:/home/[relevant path]/circle/arm-gnu-toolchain-14.3.rel1-x86_64-arm-none-eabi/bin"`
* Edit the `Rules.mk` file in `circle`:
	* Set `RASPPI = [Raspberry Pi version: 1, 2, 3, or 4]`
* `cd` to the `circle` directory and create configuration file: `./configure -r [raspberry pi version number]`
	* Should see message `configuration file successfully created:`
* Then build the project's libraries: `./makeall`
* In `part-2` directory, update the Makefile (ensure this is accurate for your directory configuration): 
	* `CIRCLEHOME = ../circle`
	* `include ../circle/Rules.mk`
* After you make modifications to files in `part-2`, use command to build kernel images: `make` (you may need to `cd` into `part-2` beforehand)
* Copy the `.img` file to the SD card (as mentioned earlier, the correct file name depends on your Raspberry Pi model). In case of problems, run `make` in the `circle/boot` folder and copy the created files onto the SD card and `bcm2712d0.dtbo` into the directory
  `overlays/` of the SD card. 
* You may need to adjust firewall settings to allow TCP request from Pi to reach your own machine
	* WSL users may also need to set up port forwarding: `netsh interface portproxy add v4tov4 listenaddress=0.0.0.0 listenport=5000 connectaddress=localhost connectport=5000`
	* If `localhost` doesn't work, use `ip addr show` to get WSL instance's IP address and use that instead