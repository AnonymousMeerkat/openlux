openlux
=======

Adjusts screen color temperature, similar to [f.lux](https://justgetflux.com/), and [redshift](http://jonls.dk/redshift/).

Primary differences from its competitors:

* One-shot only - suited for instant temperature changes or scheduled via `cron`
* Simple - no location-based automatic temperature shifting
* iOS support (requires jailbroken iDevice)

## Prerequisites ##

Required software:

 * CMake (>=2.6, compilation only)
 * GCC or Clang (compilation only)
 * One of these:
   * X11, with support for the Xf86VM extension (header files needed for compilation)
   * iOS >= 4

## Installation ##

If you wish to compile for iOS, check the wiki for instructions.

Compilation:

    mkdir build
    cd build

    cmake ..
    make

The generated program will be located in `build/src/openlux`. If you wish to install:

    sudo make install

## Examples ##

Run `openlux -h` for a list of command-line options.

```sh
openlux                      # Sets the screen color temperature to 3400K (the default)
openlux -k 1000              # Sets the color temperature to 1000K
openlux -k 2000 -b 0         # Sets color temperature to 2000K, but removes all blue light
openlux -k 2000 -b 255       # Ditto, but blue is set to 255 (maximum value, gives the screen a magenta-ish tone)
openlux -r 130 -g 150 -b 100 # Gives the screen a dark swamp green tint (Kelvin value is ignored)
openlux -k 40000             # Sets the screen color temperature to 40000K
openlux -i                   # Resets the screen color temperature
openlux -a 10800000 -d 60000 -k 2000
                             # Ramps the color temperature gradually to 2000K over 3 hours (10800000 msecs) in
                             # steps of 1 minute (60000 msecs)
```

I personally like using `openlux -k 10000` during the day (very relaxing for the eyes!), and `openlux -k 2300 -b 40` during the night.
