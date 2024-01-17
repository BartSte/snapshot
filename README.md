# README

[![Tests](https://github.com/BartSte/snapshot/actions/workflows/tests.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/tests.yml)  
[![Release](https://github.com/BartSte/snapshot/actions/workflows/release.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/release.yml)

> WORK IN PROGRESS!  
> This project is still under development. The code is still experimental and
> is subject to change.

## Table of Contents

- [Summary](#summary)
  - [Features](#features)
- [Dependencies](#dependencies)
- [Installation](#installation)
  - [Building](#building)
    - [Linux](#linux)
    - [Raspberry Pi OS](#raspberry-pi-os)
- [Usage](#usage)
  - [List the available cameras](#list-the-available-cameras)
  - [Display a video](#display-a-video)
  - [Record a video stream](#record-a-video-stream)
  - [Set the logging level](#set-the-logging-level)
  - [Using the configuration file](#using-the-configuration-file)
- [Development](#development)
  - [Build](#build)
  - [Test](#test)
  - [Running a camera](#running-a-camera)
- [License](#license)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Summary

`Snapshot` is an application that allows you to view a video and record it as a
set of snapshots. This is useful when you want to make a time-lapse of a video.
The application has a graphical user interface to view the video and a command
line interface to configure the application. It can also be configured using a
configuration file. The project is written in C++ and uses the [Qt6
framework](https://www.qt.io/). For more information, run `snapshot --help`, or
continue reading.

### Features

In short, `snapshot` contains the following features:

- [x] List the available cameras.
- [x] Display a video stream:
  - [x] using RTSP, UDP or HTTP.
  - [x] from a file.
  - [x] from a (built-in) camera.
- [x] Record a video in a set of snapshots at a given interval and duration.
- [x] Configure the application using a configuration file.
- [x] Has a command line interface.
- [ ] Runs on linux (including raspberry pi).

## Dependencies

Ensure the following dependencies are installed:

| Dependency                                                    | Version  |
| ------------------------------------------------------------- | -------- |
| [Qt6](https://www.qt.io/download): base & multimedia (ffmpeg) | ≥ 6.6.0  |
| [boost](https://www.boost.org/)                               | ≥ 1.83   |
| [spdlog](https://github.com/gabime/spdlog)                    | ≥ 1.12.0 |

For building the project, the following dependencies are required:

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

`cxxopts` and `googletest` are also dependencies, but they are included in the
project. The former as a header file in the `3rdparty` directory, and the latter
is installed using `cmake`.

## Installation

The following sections explain how to install the project. You have two
options: building the project from source, or using the pre built binaries. The
easiest way is to use the pre built binaries.

### From source

The next steps explain how to build this project from source. The following
sections explain how to build the project on different environments: Linux,
Raspberry Pi OS. Despite the fact that Raspberry Pi OS is a linux distribution,
it is discussed separately, because it requires some extra steps. It is assumed
that the dependencies explained in the [Dependencies](#dependencies) section
are installed.

For all environments, cloning the repository is the first step:

```bash
git clone https://github.com/BartSte/snapshot
```

#### Linux

Create a `build` directory by running the following command:

```bash
cmake -G "Ninja" -S . -B ./build -DCMAKE_BUILD_TYPE=Release
```

This creates a `build` directory with a `Makefile` in it. To build the project,
run the following command:

```bash
cmake --build ./build
```

After this, the `snapshot` executable can be found in the `build/bin`
directory. You can install the executable by running the following command:

```bash
cmake --install ./build
```

#### Raspberry Pi OS

- **TODO**

### Pre built binaries

- **TODO**

## Usage

The `snapshot` executable should be runned from the command line. The following
sections explain how to use the application. You can also run `snapshot --help`
to get more information.

### List the available cameras

By running the following command, a list of available cameras is shown:

```bash
snapshot --list
```

this outputs something like:

```bash
Index | Name                    | ID          | Is Default |
0     | HP TrueVision HD Camera | /dev/video0 | true       |
```

which means that there is one camera available, with index `0`, name `HP
TrueVision HD Camera`, id `/dev/video0` and is the default camera.

### Display a video

If we want to display a video stream from the default camera, we can run the
following command:

```bash
snapshot --gui
```

If a camera is found, it will opens a window with the video stream from the
default camera. You can also specify the camera to use. By doing:

```bash
snapshot --gui --camera "HP TrueVision HD Camera"
```

This will display the video stream from the camera with the name that matches
the name given by the `--list` command in the previous section.

If we want to display a video stream from a file, we can run the following:

```bash
snapshot --gui --camera sample.mp4
```

If we want to display a video stream from a URL `rtsp://localhost:8090/stream`,
we can run the following:

```bash
snapshot --gui --camera rtsp://localhost:8090/stream
```

Note that we can feed the `--camera` option with a file, a URL or a camera
name. Lastly, the `--timeout` option can be used to specify how long the app
should be searching for a video at startup, or when the connection is lost. By
default, this is set to `30s`.

### Record a video stream

When you want to record snapshots from a video stream, you can use the
`--record` option. For example, if we want to record a video stream from the
default camera, we can run the following command:

```bash
snapshot --record
```

This will save snapshots of the default camera with the default values for the
`--interval`, `--duration` and `--folder` options, i.e., every second for 10
seconds, for an unlimited time, in the `./snapshot` folder. Most likely, you
want to change these values. For example:

```bash
snapshot --record --interval 5 --duration 60 --folder /path/to/folder
```

this will save snapshots every 5 seconds for 60 seconds in the `/path/to/folder`
folder. If the folder does not exist, it will be created, as long as the parent
folder exists.

To ensure that the disk does not fill up the snapshots when you forget to stop
the recording, the `--max-snapshots` is set to `10e9` bytes (10 GB) by default,
which means that the recording will stop when the current session exceeds this
size.

### Set the logging level

By default, the logging level is set to `warning`. If we want to set the
logging level to `debug`, we can run the following command:

```bash
snapshot --log-level debug <other options>
```

The following logging levels are available: `trace`, `debug`, `info`,
`warning`, `error`, and `critical`.

### Using the configuration file

Instead of using the command line options, a cnfiguration file can be used. By
default, the application looks for a file called `config.json` in the
`$HOME/.config/snapshot` directory. If no configuration file is found, the
application uses the default configuration which is equivalent to the default
values of the command line interface (see `snapshot --help`).

Below an example of a configuration file is shown that has the default values:

```json
{
  "gui": false,
  "list": false,
  "record": false,
  "camera": "default",
  "folder": "./snapshot",
  "timeout": "30s",
  "duration": "0s",
  "interval": "10s",
  "max-bytes": "10e9",
  "loglevel": "warning",
  "pattern": "[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# @ %!] %v"
}
```

Naturally, you can change the values to your liking. For example, by setting
the `gui` option to `true`, the application will open the gui by default. This
is equivalent to running `snapshot --gui`. Note that the command line options
override the configuration file options when they are given.

If you want to put your configuration file in a different location, you can
specify the location using the `--config` option:

```bash
snapshot --config "$HOME/some/other/location/config.json"
```

Note that the `~` character and environment variables are expanded. Environment
variables must be specified using the `${VAR}` or `$VAR` syntax.

## Development

If you want to contribute to this project, you can follow the next steps to to
build the project from source. Furthermore, some information is given about
debugging and testing the project.

### Build

Follow the steps in the [Building](#building) section to build the project.
Instead of the `Release` build type, use the `Debug` build type.

#### Installing Qt6

When using Arch, you can install version >= 6.6.0 of Qt6 using pacman. However,
at the time of this writing, for distributions that hold older versions of Qt6,
like Debian and Ubuntu, you need to install Qt6 yourself. You can do this by
building Qt6 from source, or by using their online installer.

Since the CI on Github uses Ubuntu, Qt6 could not be installed using apt.
Therefore, the online installer is used. The installation is automated using
the `scripts/install-qt` script. This script downloads the online installer and
installs Qt6 in the `3rdparty/Qt` folder. This folder will be picked up by
`cmake` when building the project. You need to authenticate yourself by setting
the `QT_INSTALLER_JWT_TOKEN` environment variable to your jwt token. More
information on getting this token is provided on the [website of
Qt](https://doc.qt.io/qt-6/get-and-install-qt-cli.html).

#### Static build

** TODO **

- Explain how to statically build Qt6 and how to statically link it to
  snapshot.

- I used the following configure command:

```bash
export CC=/usr/bin/gcc
export CXX=/usr/bin/g++
export CXXFLAGS="-static-libstdc++ -static-libgcc"
configure -release -static -no-pch -prefix ~/code/snapshot/3rdparty/Qt/ -no-gstreamer -fontconfig -submodules qtbase,qtmultimedia,qtwayland,qtimageformats -- -S . -B ./build
```

- I appended the linker flag with `-static-libstdc++ -static-libgcc` such that
  the `libstdc++` and `libgcc` libraries are statically linked. However, this has
  no effect. I think the compiler has some issue with statically linking the
  std libs (I read something about using exceptions in your code that causes
  the compiler to add -shared-libgcc and -shared-libstdc++ to the linker flags).
  For now lets ignore this and just use dynamic libs for all deps that are not
  boost, qt or spdlog.

### Test

Googletest and QTest are used for building the unit tests and the end-to-end
tests. To build the tests, the `BUILD_TESTING` option must be set to `ON` when
running cmake. For example:

```bash
cmake -G "Ninja" -S . -B ./build -DCMAKE_BUILD_TYPE=Debug -BUILD_TESTING=ON
```

this will uses the `Debug` build type with the tests enabled. To run the tests,
run the `tests` executable in the `build/bin` directory:

### Running a camera

If you have no camera available, you can use one of the following options to
get a video stream.

#### Using a file

A sample video file is provided in the `tests/static`. It can be played using
the following command:

```bash
snapshot -g -c tests/static/sample.mp4
```

This opens the gui and plays the `sample.mp4` file.

#### Using an http stream

If you have internet access, you can use the following command to play a video
from a public http stream:

```bash
snapshot -g -c http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4
```

This opens the gui and plays the video.

#### Running the rtsp stream

By running the following command, a video stream is published using the rtsp
protocol:

```bash
./scripts/rtsp-stream tests/static/sample.mp4 rtsp://localhost:8090/stream
```

This stream can be viewed using the following command:

```bash
snapshot -g -c rtsp://localhost:8090/stream
```

This displays the video stream using the GUI.

#### Running the udp stream

Similar to [running the rtsp stream](#running-the-rtsp-stream), an udp stream
can be published using the following command:

```bash
./scripts/udp-stream tests/static/sample.mp4 udp://localhost:8090
```

This stream can be viewed using the following command:

```bash
snapshot -g -c udp://localhost:8090
```

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE)
file for details.

## Troubleshooting

If you encounter any issues, please report them on the issue tracker at:
[snapshot issues](https://github.com/BartSte/snapshot/issues)

## Contributing

Contributions are welcome! Please see [CONTRIBUTING](./CONTRIBUTING.md) for
more information.

# TODO:

- [ ] Add pre built binaries to the github releases

  - [x] Link the app statically against Qt6, boost and spdlog
  - [ ] Go through all the remaining dynamic libs using ldd and see which ones
        can also be linked statically.
  - [x] Add the remaining dynamic libs to the same directory tree as the
        executable and using the script that is provided in the Qt [docs](https://doc.qt.io/qt-6/linux-deployment.html)
        to run the app.
  - [ ] Publish the directory tree as an archive as a release. Other more
        sophisticated release methods can be used later.

  - Ffmpeg libs are missing from build? And also other libs? This is the ldd
    afterd the LD_LIBRARY_PATH is set to ./lib:

    ```
    # Assumed to be on system
    linux-vdso.so.1 (0x00007fff26bdf000)
    libm.so.6 => /usr/lib/libm.so.6 (0x00007f822530f000)
    libc.so.6 => /usr/lib/libc.so.6 (0x00007f8224e1e000)
    /lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007f822a7d1000)

    libz.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libz.so.1 (0x00007f8225c5b000)
    libSM.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libSM.so.6 (0x00007f8226035000)
    libEGL.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libEGL.so.1 (0x00007f8225c9e000)
    libGLX.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libGLX.so.0 (0x00007f82275cc000)
    libICE.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libICE.so.6 (0x00007f8226016000)
    libX11.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libX11.so.6 (0x00007f82262c0000)
    libdrm.so.2 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libdrm.so.2 (0x00007f8225c88000)
    libfmt.so.8 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libfmt.so.8 (0x00007f822a7ae000)
    libgbm.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgbm.so.1 (0x00007f8225c77000)
    libxcb.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb.so.1 (0x00007f8225cb1000)
    libffi.so.8 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libffi.so.8 (0x00007f8225b76000)
    libbsd.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libbsd.so.0 (0x00007f8219cff000)
    libXau.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libXau.so.6 (0x00007f8223682000)
    libogg.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libogg.so.0 (0x00007f821663a000)
    liblz4.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/liblz4.so.1 (0x00007f8215896000)
    libXext.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libXext.so.6 (0x00007f8228eec000)
    libjpeg.so.8 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libjpeg.so.8 (0x00007f8225d7f000)
    libtiff.so.5 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libtiff.so.5 (0x00007f8225cf8000)
    libudev.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libudev.so.1 (0x00007f8225ba7000)
    libzstd.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libzstd.so.1 (0x00007f8225536000)
    liblzma.so.5 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/liblzma.so.5 (0x00007f8224650000)
    libopus.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libopus.so.0 (0x00007f82237a2000)
    libwebp.so.7 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libwebp.so.7 (0x00007f8222993000)
    libuuid.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libuuid.so.1 (0x00007f8223691000)
    libjbig.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libjbig.so.0 (0x00007f8222927000)
    libpcre.so.3 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpcre.so.3 (0x00007f821858a000)
    libFLAC.so.8 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libFLAC.so.8 (0x00007f821571c000)
    libpulse.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpulse.so.0 (0x00007f8228f53000)
    libgcc_s.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgcc_s.so.1 (0x00007f8225b83000)
    libexpat.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libexpat.so.1 (0x00007f8219d17000)
    libXdmcp.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libXdmcp.so.6 (0x00007f8222456000)
    libOpenGL.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libOpenGL.so.0 (0x00007f8228ebe000)
    libXrandr.so.2 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libXrandr.so.2 (0x00007f8228f01000)
    libdbus-1.so.3 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libdbus-1.so.3 (0x00007f82260db000)
    libicuuc.so.70 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libicuuc.so.70 (0x00007f8225605000)
    libpng16.so.16 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpng16.so.16 (0x00007f8227591000)
    libstdc++.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libstdc++.so.6 (0x00007f8225000000)
    libvorbis.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libvorbis.so.0 (0x00007f8223775000)
    libX11-xcb.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libX11-xcb.so.1 (0x00007f8226011000)
    libgio-2.0.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgio-2.0.so.0 (0x00007f8228fa8000)
    libxcb-glx.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-glx.so.0 (0x00007f8225cdb000)
    libxcb-shm.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-shm.so.0 (0x00007f8226088000)
    libxcb-xkb.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-xkb.so.1 (0x00007f8226040000)
    libselinux.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libselinux.so.1 (0x00007f8225293000)
    libXrender.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libXrender.so.1 (0x00007f8223c9a000)
    libsystemd.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libsystemd.so.0 (0x00007f8219939000)
    libdeflate.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libdeflate.so.0 (0x00007f8219cdb000)
    libpcre2-8.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpcre2-8.so.0 (0x00007f82184f3000)
    libsndfile.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libsndfile.so.1 (0x00007f8218474000)
    libasyncns.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libasyncns.so.0 (0x00007f8219cd3000)
    libgcrypt.so.20 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgcrypt.so.20 (0x00007f8215758000)
    libfreetype.so.6 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libfreetype.so.6 (0x00007f8226129000)
    libglib-2.0.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libglib-2.0.so.0 (0x00007f82253fc000)
    libharfbuzz.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libharfbuzz.so.0 (0x00007f82261f1000)
    libicui18n.so.70 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libicui18n.so.70 (0x00007f8225800000)
    libpcre2-16.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpcre2-16.so.0 (0x00007f8225bd1000)
    libxcb-sync.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-sync.so.1 (0x00007f822607e000)
    libxcb-util.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-util.so.1 (0x00007f8223688000)
    libicudata.so.70 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libicudata.so.70 (0x00007f8216800000)
    libapparmor.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libapparmor.so.1 (0x00007f82198d2000)
    libbrotlidec.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libbrotlidec.so.1 (0x00007f82291e1000)
    libxcb-icccm.so.4 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-icccm.so.4 (0x00007f8228b57000)
    libxcb-image.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-image.so.0 (0x00007f8228b51000)
    libxcb-randr.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-randr.so.0 (0x00007f822752f000)
    libxcb-shape.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-shape.so.0 (0x00007f822605e000)
    libxkbcommon.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxkbcommon.so.0 (0x00007f8226094000)
    libvorbisenc.so.2 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libvorbisenc.so.2 (0x00007f82236ca000)
    libgraphite2.so.3 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgraphite2.so.3 (0x00007f8222dd9000)
    libgpg-error.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgpg-error.so.0 (0x00007f821563d000)
    libfontconfig.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libfontconfig.so.1 (0x00007f8227547000)
    libxcb-cursor.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-cursor.so.0 (0x00007f8225e00000)
    libxcb-render.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-render.so.0 (0x00007f8226063000)
    libxcb-xfixes.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-xfixes.so.0 (0x00007f8226072000)
    libGLdispatch.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libGLdispatch.so.0 (0x00007f8219d48000)
    libgobject-2.0.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgobject-2.0.so.0 (0x00007f8229181000)
    libwayland-egl.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libwayland-egl.so.1 (0x00007f822a7a9000)
    libxcb-keysyms.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-keysyms.so.1 (0x00007f8227542000)
    libgmodule-2.0.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libgmodule-2.0.so.0 (0x00007f8225b4c000)
    libbrotlicommon.so.1 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libbrotlicommon.so.1 (0x00007f8225b53000)
    libxkbcommon-x11.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxkbcommon-x11.so.0 (0x00007f8228b5e000)
    libwayland-client.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libwayland-client.so.0 (0x00007f82291ef000)
    libwayland-cursor.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libwayland-cursor.so.0 (0x00007f822a79f000)
    libwayland-server.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libwayland-server.so.0 (0x00007f8219923000)
    libxcb-render-util.so.0 => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libxcb-render-util.so.0 (0x00007f822608d000)
    libpulsecommon-15.99.so => /home/barts/Downloads/snapshot-0.0.0-Linux/./lib/libpulsecommon-15.99.so (0x00007f8224d99000)

    # FFmpeg libs are missing...
    libavformat.so.60 => /usr/lib/libavformat.so.60 (0x00007f8228c00000)
    libavcodec.so.60 => /usr/lib/libavcodec.so.60 (0x00007f8227600000)
    libswresample.so.4 => /usr/lib/libswresample.so.4 (0x00007f8228f10000)
    libswscale.so.7 => /usr/lib/libswscale.so.7 (0x00007f8228b69000)
    libavutil.so.58 => /usr/lib/libavutil.so.58 (0x00007f8226400000)

    # Other missing libs?
    libOpenCL.so.1 => /usr/lib/libOpenCL.so.1 (0x00007f822245e000)
    libSvtAv1Enc.so.1 => /usr/lib/libSvtAv1Enc.so.1 (0x00007f8219e00000)
    libXfixes.so.3 => /usr/lib/libXfixes.so.3 (0x00007f8216632000)
    libaom.so.3 => /usr/lib/libaom.so.3 (0x00007f8222e00000)
    libblkid.so.1 => /usr/lib/libblkid.so.1 (0x00007f82198e7000)
    libbluray.so.2 => /usr/lib/libbluray.so.2 (0x00007f8224860000)
    libbrotlienc.so.1 => /usr/lib/libbrotlienc.so.1 (0x00007f82166f1000)
    libbz2.so.1.0 => /usr/lib/libbz2.so.1.0 (0x00007f8225b39000)
    libcairo-gobject.so.2 => /usr/lib/libcairo-gobject.so.2 (0x00007f8218430000)
    libcairo.so.2 => /usr/lib/libcairo.so.2 (0x00007f8223ceb000)
    libcap.so.2 => /usr/lib/libcap.so.2 (0x00007f821661f000)
    libcrypto.so.3 => /usr/lib/libcrypto.so.3 (0x00007f8215a00000)
    libdatrie.so.1 => /usr/lib/libdatrie.so.1 (0x00007f8215627000)
    libdav1d.so.7 => /usr/lib/libdav1d.so.7 (0x00007f8223e24000)
    libfribidi.so.0 => /usr/lib/libfribidi.so.0 (0x00007f821566e000)
    libgdk_pixbuf-2.0.so.0 => /usr/lib/libgdk_pixbuf-2.0.so.0 (0x00007f821606a000)
    libgmp.so.10 => /usr/lib/libgmp.so.10 (0x00007f82247ba000)
    libgnutls.so.30 => /usr/lib/libgnutls.so.30 (0x00007f8224400000)
    libgomp.so.1 => /usr/lib/libgomp.so.1 (0x00007f82158b6000)
    libgsm.so.1 => /usr/lib/libgsm.so.1 (0x00007f8224635000)
    libhogweed.so.6 => /usr/lib/libhogweed.so.6 (0x00007f8216650000)
    libhwy.so.1 => /usr/lib/libhwy.so.1 (0x00007f8216645000)
    libicudata.so.74 => /usr/lib/libicudata.so.74 (0x00007f8213800000)
    libicuuc.so.74 => /usr/lib/libicuuc.so.74 (0x00007f8216400000)
    libidn2.so.0 => /usr/lib/libidn2.so.0 (0x00007f8218452000)
    libjbig.so.2.1 => /usr/lib/libjbig.so.2.1 (0x00007f8215630000)
    libjxl.so.0.8 => /usr/lib/libjxl.so.0.8 (0x00007f8222a00000)
    libjxl_threads.so.0.8 => /usr/lib/libjxl_threads.so.0.8 (0x00007f8225b2f000)
    libmd.so.0 => /usr/lib/libmd.so.0 (0x00007f821660f000)
    libmodplug.so.1 => /usr/lib/libmodplug.so.1 (0x00007f8224aa1000)
    libmount.so.1 => /usr/lib/libmount.so.1 (0x00007f82252bf000)
    libmp3lame.so.0 => /usr/lib/libmp3lame.so.0 (0x00007f8224388000)
    libmpg123.so.0 => /usr/lib/libmpg123.so.0 (0x00007f82167a3000)
    libmvec.so.1 => /usr/lib/libmvec.so.1 (0x00007f8215907000)
    libnettle.so.8 => /usr/lib/libnettle.so.8 (0x00007f8216699000)
    libopencore-amrnb.so.0 => /usr/lib/libopencore-amrnb.so.0 (0x00007f822460c000)
    libopencore-amrwb.so.0 => /usr/lib/libopencore-amrwb.so.0 (0x00007f8225270000)
    libopenjp2.so.7 => /usr/lib/libopenjp2.so.7 (0x00007f822431b000)
    libopenmpt.so.0 => /usr/lib/libopenmpt.so.0 (0x00007f82248bd000)
    libp11-kit.so.0 => /usr/lib/libp11-kit.so.0 (0x00007f821626a000)
    libpango-1.0.so.0 => /usr/lib/libpango-1.0.so.0 (0x00007f8216000000)
    libpangocairo-1.0.so.0 => /usr/lib/libpangocairo-1.0.so.0 (0x00007f8218420000)
    libpangoft2-1.0.so.0 => /usr/lib/libpangoft2-1.0.so.0 (0x00007f8215f34000)
    libpixman-1.so.0 => /usr/lib/libpixman-1.so.0 (0x00007f8215f4d000)
    libpthread.so.0 => /usr/lib/libpthread.so.0 (0x00007f8222451000)
    librav1e.so.0.6 => /usr/lib/librav1e.so.0.6 (0x00007f8222600000)
    librsvg-2.so.2 => /usr/lib/librsvg-2.so.2 (0x00007f8223800000)
    libsnappy.so.1 => /usr/lib/libsnappy.so.1 (0x00007f8224644000)
    libsoxr.so.0 => /usr/lib/libsoxr.so.0 (0x00007f822248e000)
    libspeex.so.1 => /usr/lib/libspeex.so.1 (0x00007f82242fe000)
    libsrt.so.1.5 => /usr/lib/libsrt.so.1.5 (0x00007f82246e8000)
    libssh.so.4 => /usr/lib/libssh.so.4 (0x00007f822467b000)
    libtasn1.so.6 => /usr/lib/libtasn1.so.6 (0x00007f821843b000)
    libthai.so.0 => /usr/lib/libthai.so.0 (0x00007f8215663000)
    libtheoradec.so.1 => /usr/lib/libtheoradec.so.1 (0x00007f82242e4000)
    libtheoraenc.so.1 => /usr/lib/libtheoraenc.so.1 (0x00007f8223cb3000)
    libtiff.so.6 => /usr/lib/libtiff.so.6 (0x00007f821568e000)
    libunistring.so.5 => /usr/lib/libunistring.so.5 (0x00007f82160b0000)
    libva-drm.so.2 => /usr/lib/libva-drm.so.2 (0x00007f82242df000)
    libva-x11.so.2 => /usr/lib/libva-x11.so.2 (0x00007f8223cac000)
    libva.so.2 => /usr/lib/libva.so.2 (0x00007f822369a000)
    libvdpau.so.1 => /usr/lib/libvdpau.so.1 (0x00007f8223ca7000)
    libvorbisfile.so.3 => /usr/lib/libvorbisfile.so.3 (0x00007f8219cc8000)
    libvpl.so.2 => /usr/lib/libvpl.so.2 (0x00007f8222938000)
    libvpx.so.8 => /usr/lib/libvpx.so.8 (0x00007f8224000000)
    libwebpmux.so.3 => /usr/lib/libwebpmux.so.3 (0x00007f8225286000)
    libx264.so.164 => /usr/lib/libx264.so.164 (0x00007f8219a00000)
    libx265.so.199 => /usr/lib/libx265.so.199 (0x00007f8218600000)
    libxcb-dri3.so.0 => /usr/lib/libxcb-dri3.so.0 (0x00007f821662b000)
    libxml2.so.2 => /usr/lib/libxml2.so.2 (0x00007f8224c2f000)
    libxvidcore.so.4 => /usr/lib/libxvidcore.so.4 (0x00007f82224f1000)
    ```

    Above is the build from the ubuntu pipeline. How does it look for a local
    build on Arch? Wel, this:

    ```
    # Assumed to be on system
    linux-vdso.so.1 (0x00007ffd75140000)
    libm.so.6 => /usr/lib/libm.so.6 (0x00007f8072f0a000)
    libc.so.6 => /usr/lib/libc.so.6 (0x00007f807021e000)

    # Bundled libs
    libz.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libz.so.1 (0x00007f8072d88000)
    libts.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libts.so.0 (0x00007f8074b32000)
    libSM.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libSM.so.6 (0x00007f80732ed000)
    libb2.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libb2.so.1 (0x00007f8072ff7000)
    libdl.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libdl.so.2 (0x00007f806e5d5000)
    libX11.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libX11.so.6 (0x00007f80738bd000)
    libGLX.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libGLX.so.0 (0x00007f8076159000)
    libICE.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libICE.so.6 (0x00007f80732cf000)
    libEGL.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libEGL.so.1 (0x00007f807309c000)
    libxcb.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb.so.1 (0x00007f8073054000)
    libdrm.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libdrm.so.2 (0x00007f807303d000)
    libgbm.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libgbm.so.1 (0x00007f807302d000)
    libffi.so.8 => /home/barts/snapshot-0.0.0-Linux/./lib/libffi.so.8 (0x00007f806e5da000)
    libXau.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libXau.so.6 (0x00007f80650d1000)
    libogg.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libogg.so.0 (0x00007f8063360000)
    liblz4.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/liblz4.so.1 (0x00007f8062b5d000)
    libfmt.so.10 => /home/barts/snapshot-0.0.0-Linux/./lib/libfmt.so.10 (0x00007f80779ea000)
    libXext.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libXext.so.6 (0x00007f80764df000)
    libudev.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libudev.so.1 (0x00007f807341a000)
    libjpeg.so.8 => /home/barts/snapshot-0.0.0-Linux/./lib/libjpeg.so.8 (0x00007f80731c7000)
    libtiff.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libtiff.so.6 (0x00007f8073139000)
    libwebp.so.7 => /home/barts/snapshot-0.0.0-Linux/./lib/libwebp.so.7 (0x00007f80730b8000)
    libzstd.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libzstd.so.1 (0x00007f8072da7000)
    liblzma.so.5 => /home/barts/snapshot-0.0.0-Linux/./lib/liblzma.so.5 (0x00007f8072570000)
    libopus.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libopus.so.0 (0x00007f806eebd000)
    libkrb5.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libkrb5.so.3 (0x00007f8065128000)
    libuuid.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libuuid.so.1 (0x00007f80650e3000)
    libgomp.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libgomp.so.1 (0x00007f8063b6a000)
    libpulse.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libpulse.so.0 (0x00007f80765ab000)
    libgcc_s.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libgcc_s.so.1 (0x00007f8072d61000)
    libbz2.so.1.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libbz2.so.1.0 (0x00007f8072d4e000)
    libexpat.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libexpat.so.1 (0x00007f8063ccd000)
    libevdev.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libevdev.so.2 (0x00007f80654d3000)
    libwacom.so.9 => /home/barts/snapshot-0.0.0-Linux/./lib/libwacom.so.9 (0x00007f80650ec000)
    libXdmcp.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libXdmcp.so.6 (0x00007f8063bd1000)
    libFLAC.so.12 => /home/barts/snapshot-0.0.0-Linux/./lib/libFLAC.so.12 (0x00007f8062b17000)
    libXrandr.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libXrandr.so.2 (0x00007f80779b8000)
    libresolv.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libresolv.so.2 (0x00007f80764bf000)
    libOpenGL.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libOpenGL.so.0 (0x00007f8074b73000)
    libpng16.so.16 => /home/barts/snapshot-0.0.0-Linux/./lib/libpng16.so.16 (0x00007f8074b39000)
    libdbus-1.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libdbus-1.so.3 (0x00007f8073451000)
    libinput.so.10 => /home/barts/snapshot-0.0.0-Linux/./lib/libinput.so.10 (0x00007f80733c6000)
    libjasper.so.7 => /home/barts/snapshot-0.0.0-Linux/./lib/libjasper.so.7 (0x00007f8073274000)
    libicuuc.so.74 => /home/barts/snapshot-0.0.0-Linux/./lib/libicuuc.so.74 (0x00007f8072600000)
    libstdc++.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libstdc++.so.6 (0x00007f8070400000)
    libvorbis.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libvorbis.so.0 (0x00007f806e19a000)
    libjbig.so.2.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libjbig.so.2.1 (0x00007f80650d6000)
    libgio-2.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgio-2.0.so.0 (0x00007f80736e4000)
    libxcb-shm.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-shm.so.0 (0x00007f807333e000)
    libxcb-xkb.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-xkb.so.1 (0x00007f80732f8000)
    libX11-xcb.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libX11-xcb.so.1 (0x00007f80732ca000)
    libwebpmux.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libwebpmux.so.3 (0x00007f8073126000)
    libxcb-glx.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-glx.so.0 (0x00007f807307f000)
    libXrender.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libXrender.so.1 (0x00007f806e5f3000)
    libsystemd.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libsystemd.so.0 (0x00007f8063bd9000)
    libpcre2-8.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libpcre2-8.so.0 (0x00007f8063acf000)
    libsndfile.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libsndfile.so.1 (0x00007f8063a48000)
    libasyncns.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libasyncns.so.0 (0x00007f8063a3e000)
    libgcrypt.so.20 => /home/barts/snapshot-0.0.0-Linux/./lib/libgcrypt.so.20 (0x00007f8062b82000)
    libfreetype.so.6 => /home/barts/snapshot-0.0.0-Linux/./lib/libfreetype.so.6 (0x00007f8073616000)
    libharfbuzz.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libharfbuzz.so.0 (0x00007f8073508000)
    libxcb-sync.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-sync.so.1 (0x00007f8073335000)
    libsharpyuv.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libsharpyuv.so.0 (0x00007f80730ae000)
    libicui18n.so.74 => /home/barts/snapshot-0.0.0-Linux/./lib/libicui18n.so.74 (0x00007f8072a00000)
    libicudata.so.74 => /home/barts/snapshot-0.0.0-Linux/./lib/libicudata.so.74 (0x00007f8070800000)
    libpcre2-16.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libpcre2-16.so.0 (0x00007f8072e7a000)
    libglib-2.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libglib-2.0.so.0 (0x00007f80728b4000)
    libk5crypto.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libk5crypto.so.3 (0x00007f80650fa000)
    libkeyutils.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libkeyutils.so.1 (0x00007f806ee7f000)
    libxcb-util.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-util.so.1 (0x00007f80654ca000)
    libbrotlidec.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libbrotlidec.so.1 (0x00007f80764d0000)
    libxkbcommon.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxkbcommon.so.0 (0x00007f807337d000)
    libxcb-icccm.so.4 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-icccm.so.4 (0x00007f8073366000)
    libxcb-image.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-image.so.0 (0x00007f8073360000)
    libxcb-randr.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-randr.so.0 (0x00007f8073349000)
    libxcb-shape.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-shape.so.0 (0x00007f8073317000)
    libwebpdemux.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libwebpdemux.so.2 (0x00007f8073133000)
    libvorbisenc.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libvorbisenc.so.2 (0x00007f806dd55000)
    libgraphite2.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libgraphite2.so.3 (0x00007f806dc51000)
    libgudev-1.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgudev-1.0.so.0 (0x00007f806333d000)
    libgpg-error.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgpg-error.so.0 (0x00007f8062aaf000)
    libfontconfig.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libfontconfig.so.1 (0x00007f80734a2000)
    libxcb-cursor.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-cursor.so.0 (0x00007f807336d000)
    libxcb-xfixes.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-xfixes.so.0 (0x00007f807332c000)
    libxcb-render.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-render.so.0 (0x00007f807331d000)
    libGLdispatch.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libGLdispatch.so.0 (0x00007f8063cf8000)
    libgssapi_krb5.so.2 => /home/barts/snapshot-0.0.0-Linux/./lib/libgssapi_krb5.so.2 (0x00007f80761ac000)
    libgobject-2.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgobject-2.0.so.0 (0x00007f8074b9e000)
    libwayland-egl.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libwayland-egl.so.1 (0x00007f80761a7000)
    libxcb-keysyms.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-keysyms.so.1 (0x00007f807335b000)
    libgthread-2.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgthread-2.0.so.0 (0x00007f8072da2000)
    libkrb5support.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libkrb5support.so.0 (0x00007f806e5e5000)
    libgmodule-2.0.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libgmodule-2.0.so.0 (0x00007f806e133000)
    libbrotlicommon.so.1 => /home/barts/snapshot-0.0.0-Linux/./lib/libbrotlicommon.so.1 (0x00007f806dc74000)
    libxkbcommon-x11.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxkbcommon-x11.so.0 (0x00007f8073374000)
    libwayland-cursor.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libwayland-cursor.so.0 (0x00007f807619d000)
    libwayland-client.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libwayland-client.so.0 (0x00007f807618b000)
    libpulsecommon-16.1.so => /home/barts/snapshot-0.0.0-Linux/./lib/libpulsecommon-16.1.so (0x00007f807282d000)
    libwayland-server.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libwayland-server.so.0 (0x00007f8063bbb000)
    libxcb-render-util.so.0 => /home/barts/snapshot-0.0.0-Linux/./lib/libxcb-render-util.so.0 (0x00007f8073343000)
    libdouble-conversion.so.3 => /home/barts/snapshot-0.0.0-Linux/./lib/libdouble-conversion.so.3 (0x00007f8073015000)

    # FFmpeg libs are missing...
    libavformat.so.60 => /usr/lib/libavformat.so.60 (0x00007f8076200000)
    libavcodec.so.60 => /usr/lib/libavcodec.so.60 (0x00007f8227600000)
    libswresample.so.4 => /usr/lib/libswresample.so.4 (0x00007f8228f10000)
    libswscale.so.7 => /usr/lib/libswscale.so.7 (0x00007f8228b69000)
    libavutil.so.58 => /usr/lib/libavutil.so.58 (0x00007f8226400000)

    # Other missing libs?
    libva.so.2 => /usr/lib/libva.so.2 (0x00007f806e16a000)
    libssh.so.4 => /usr/lib/libssh.so.4 (0x00007f806fac1000)
    libvpx.so.8 => /usr/lib/libvpx.so.8 (0x00007f806f600000)
    libaom.so.3 => /usr/lib/libaom.so.3 (0x00007f806e600000)
    libgsm.so.1 => /usr/lib/libgsm.so.1 (0x00007f8070687000)
    libvpl.so.2 => /usr/lib/libvpl.so.2 (0x00007f806dcfa000)
    libcap.so.2 => /usr/lib/libcap.so.2 (0x00007f806e127000)
    libhwy.so.1 => /usr/lib/libhwy.so.1 (0x00007f806336b000)
    libmd4c.so.0 => /usr/lib/libmd4c.so.0 (0x00007f80734f2000)
    libxml2.so.2 => /usr/lib/libxml2.so.2 (0x00007f8070696000)
    libgmp.so.10 => /usr/lib/libgmp.so.10 (0x00007f806fe06000)
    libsoxr.so.0 => /usr/lib/libsoxr.so.0 (0x00007f806dc97000)
    libidn2.so.0 => /usr/lib/libidn2.so.0 (0x00007f806376a000)
    libmvec.so.1 => /usr/lib/libmvec.so.1 (0x00007f8062d07000)
    libthai.so.0 => /usr/lib/libthai.so.0 (0x00007f8062ad5000)
    libmtdev.so.1 => /usr/lib/libmtdev.so.1 (0x00007f8076152000)
    libsrt.so.1.5 => /usr/lib/libsrt.so.1.5 (0x00007f806fb2e000)
    libdav1d.so.7 => /usr/lib/libdav1d.so.7 (0x00007f806f8e5000)
    libcairo.so.2 => /usr/lib/libcairo.so.2 (0x00007f806f4c7000)
    libjxl.so.0.8 => /usr/lib/libjxl.so.0.8 (0x00007f806e200000)
    libspeex.so.1 => /usr/lib/libspeex.so.1 (0x00007f806eea0000)
    libvdpau.so.1 => /usr/lib/libvdpau.so.1 (0x00007f806f8e0000)
    libmount.so.1 => /usr/lib/libmount.so.1 (0x00007f8063db0000)
    libtasn1.so.6 => /usr/lib/libtasn1.so.6 (0x00007f8063599000)
    libblkid.so.1 => /usr/lib/libblkid.so.1 (0x00007f8062ccb000)
    libbluray.so.2 => /usr/lib/libbluray.so.2 (0x00007f80725a3000)
    librsvg-2.so.2 => /usr/lib/librsvg-2.so.2 (0x00007f806f000000)
    libsnappy.so.1 => /usr/lib/libsnappy.so.1 (0x00007f8072564000)
    libx264.so.164 => /usr/lib/libx264.so.164 (0x00007f8065200000)
    libx265.so.199 => /usr/lib/libx265.so.199 (0x00007f8063e00000)
    libva-drm.so.2 => /usr/lib/libva-drm.so.2 (0x00007f8070682000)
    libva-x11.so.2 => /usr/lib/libva-x11.so.2 (0x00007f807067b000)
    libOpenCL.so.1 => /usr/lib/libOpenCL.so.1 (0x00007f806e13a000)
    libmpg123.so.0 => /usr/lib/libmpg123.so.0 (0x00007f80639e1000)
    libnettle.so.8 => /usr/lib/libnettle.so.8 (0x00007f8063541000)
    libcrypto.so.3 => /usr/lib/libcrypto.so.3 (0x00007f8062e00000)
    libXfixes.so.3 => /usr/lib/libXfixes.so.3 (0x00007f8063353000)
    libdatrie.so.1 => /usr/lib/libdatrie.so.1 (0x00007f8062aa6000)
    libmodplug.so.1 => /usr/lib/libmodplug.so.1 (0x00007f8070090000)
    libopenmpt.so.0 => /usr/lib/libopenmpt.so.0 (0x00007f806feac000)
    libgnutls.so.30 => /usr/lib/libgnutls.so.30 (0x00007f806fc00000)
    libmp3lame.so.0 => /usr/lib/libmp3lame.so.0 (0x00007f806ef88000)
    libopenjp2.so.7 => /usr/lib/libopenjp2.so.7 (0x00007f806ef1b000)
    librav1e.so.0.6 => /usr/lib/librav1e.so.0.6 (0x00007f806de00000)
    libcom_err.so.2 => /usr/lib/libcom_err.so.2 (0x00007f806f496000)
    libp11-kit.so.0 => /usr/lib/libp11-kit.so.0 (0x00007f8063840000)
    libhogweed.so.6 => /usr/lib/libhogweed.so.6 (0x00007f80634f8000)
    libpthread.so.0 => /usr/lib/libpthread.so.0 (0x00007f806335b000)
    libfribidi.so.0 => /usr/lib/libfribidi.so.0 (0x00007f8062ae0000)
    libxvidcore.so.4 => /usr/lib/libxvidcore.so.4 (0x00007f80654f1000)
    libpixman-1.so.0 => /usr/lib/libpixman-1.so.0 (0x00007f8063378000)
    libxcb-dri3.so.0 => /usr/lib/libxcb-dri3.so.0 (0x00007f806334a000)
    libSvtAv1Enc.so.1 => /usr/lib/libSvtAv1Enc.so.1 (0x00007f8065600000)
    libtheoraenc.so.1 => /usr/lib/libtheoraenc.so.1 (0x00007f806e1c8000)
    libtheoradec.so.1 => /usr/lib/libtheoradec.so.1 (0x00007f806ee86000)
    libbrotlienc.so.1 => /usr/lib/libbrotlienc.so.1 (0x00007f806378e000)
    libunistring.so.5 => /usr/lib/libunistring.so.5 (0x00007f80635b0000)
    libpango-1.0.so.0 => /usr/lib/libpango-1.0.so.0 (0x00007f806342b000)
    libvorbisfile.so.3 => /usr/lib/libvorbisfile.so.3 (0x00007f80639d6000)
    libpangoft2-1.0.so.0 => /usr/lib/libpangoft2-1.0.so.0 (0x00007f8062b00000)
    libjxl_threads.so.0.8 => /usr/lib/libjxl_threads.so.0.8 (0x00007f807280f000)
    libcairo-gobject.so.2 => /usr/lib/libcairo-gobject.so.2 (0x00007f80634eb000)
    libopencore-amrwb.so.0 => /usr/lib/libopencore-amrwb.so.0 (0x00007f8072817000)
    libopencore-amrnb.so.0 => /usr/lib/libopencore-amrnb.so.0 (0x00007f806f49e000)
    libgdk_pixbuf-2.0.so.0 => /usr/lib/libgdk_pixbuf-2.0.so.0 (0x00007f80634a5000)
    libpangocairo-1.0.so.0 => /usr/lib/libpangocairo-1.0.so.0 (0x00007f8063495000)
    /lib64/ld-linux-x86-64.so.2 => /usr/lib64/ld-linux-x86-64.so.2 (0x00007f8077a12000)
    ```

    Inspect later whether they are the same...

  - Issues graphics:

    - On WSL I get the following error:

    ```
    snapshot-0.0.0-Linux/snapshot -g
    [2024-01-09 18:18:41.573] [warning] [: @ ] No cameras found
    MESA: error: ZINK: vkCreateInstance failed (VK_ERROR_INCOMPATIBLE_DRIVER)
    libEGL warning: egl: failed to create dri2 screen
    ```

    According to bing:

    ```
    It seems that you are encountering an error while running your Qt app on
    Arch Linux with WSL. The error message you provided indicates that the
    vkCreateInstance function failed with the error code
    VK_ERROR_INCOMPATIBLE_DRIVER. This error is usually caused by a mismatch
    between the Vulkan driver and the hardware it is running on 1.

    One possible solution is to install the latest graphics drivers for your
    system 2. Another possible solution is to try running the app without any
    Vulkan layers at all 3.

    It’s also worth noting that there is an ongoing issue with
    hardware-accelerated Vulkan in WSL2 4. You may want to check if this issue
    is related to your problem.

    I hope this helps!
    ```

    An idea is to check for differences in the output of `pldd` when comparing
    an snapshot app that uses a dynamic Qt6 and one that uses a static Qt6. I
    the past, a dynamic Qt6 worked on WSL, so it is likely that something goes
    wrong during the static linking.

    I think it has to do with support for the GPU. I think it is not possible
    to make a stand-alone structure that support all GPU's. So it is likely to
    be better that the user installs the GPU drivers themselves. I am not sure
    what the best way is to do this. It does explain why the build on the
    ubuntu server fails on my arch machine, but when I build it myself, it
    works. This may also be the case on my WSL+arch machine when I install the
    GPU drivers as is explained
    [here](https://github.com/lutris/docs/blob/master/InstallingDrivers.md#amd--intel).

- [ ] Cross compile for raspberry pi
