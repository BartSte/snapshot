# README

[![Tests](https://github.com/BartSte/snapshot/actions/workflows/tests.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/tests.yml)  
[![Release](https://github.com/BartSte/snapshot/actions/workflows/release.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/release.yml)

> WORK IN PROGRESS  
> This project is still under development. The code is still experimental and
> is subject to change.

## Table of Contents

- [Summary](#summary)
  - [Features](#features)
  - [Dependencies](#dependencies)
- [Installation](#installation)
  - [Stand-alone tarball](#tarball-standalone)
    - [Linux](#linux)
  - [Build from source](#building)
    - [Linux](#linux)
    - [Raspberry Pi OS](#raspberry-pi-os)
- [Usage](#usage)
  - [List the available cameras](#list-the-available-cameras)
  - [Display a video](#display-a-video)
  - [Record a video stream](#record-a-video-stream)
  - [Using the configuration file](#using-the-configuration-file)
  - [Set the logging level](#set-the-logging-level)
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
configuration file. The project is written in C++ and uses
[Qt6](https://www.qt.io/). For more information, run `snapshot --help`, or
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

### Limitations

- Linux only
- $\text{glibc} \geq 2.35$ (when using the tarball)
- 64 bit systems only
- X11, Wayland are supported. EGLFS is support is added, but not tested.
- Tested on Ubuntu 22.04, Arch linux and Raspberry Pi OS (bullseye).

## Installation

The following sections explain how to install the project. You have two
options: using the pre-built binaries. The easiest way is to use the pre-built
binaries, i.e., the stand-alone tarball.

### Stand-alone tarball

> Requires that $\text{glibc} \geq 2.35$

This is the easiest way to install the application.

Go to the [releases](https://github.com/BartSte/snapshot/releases) page and
download the latest `tar.gz` file. Extract the file using the following
command:

```bash
tar -xvf snapshot-<version>-Linux.tar.gz
```

This creates a `snapshot-<version>` directory with the following structure:

```bash
snapshot-<version>
  ├── bin/
  ├── lib/
  └── snapshot
```

where `bin` and `lib` are directories that you should not touch. The `snapshot`
script is the one that you should use to interact with the application. For
example:

```bash
./snapshot-<version>/snapshot --help
```

will print the help message. For convenience, you can place the
`snapshot-<version>` folder anywhere you like, and create a symlink to the
`snapshot` script in a directory that is in your `PATH`. For example:

```bash
ln -s /path/to/snapshot-<version>/snapshot $HOME/bin/snapshot
```

here it is assumed that the `$HOME/bin` directory is in your `PATH`.

### From source

In this section, the steps to build the project from source are explained. In
short:

- You can use the `setup` script to install the dependencies.
- A static or shared Qt build can be selected.
- A Qt account is needed in order to use the `setup` script.
- Using the `configure` script, the build can be configured.

### Dependencies

Snapshot relies on the following dependencies:

| Dependency                                           | Version     |
| ---------------------------------------------------- | ----------- |
| [Qt6](https://www.qt.io/download): base & multimedia | 6.6         |
| [ffmpeg](https://ffmpeg.org/)                        | 6.0, 6.1    |
| [boost](https://www.boost.org/)                      | $\geq$ 1.74 |
| [spdlog](https://github.com/gabime/spdlog)           | $\geq$ 1.9  |

Here, the Qt6 version is fixed. Other versions of ffmpeg, boost, and spdlog
might work, but are not tested.

For building the project, the following dependencies are required:

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

Building the project was tested using $\text{clang}\geq 14$ and $\text{gcc}\geq
11$

`cxxopts` and `googletest` are also dependencies, but they are included in the
project. The former as a header file in the `3rdparty` directory, and the latter
is installed using `cmake`.

#### Setting up the environment

> The `setup` script only supports Arch Linux and Ubuntu 22.04. If you are
> using a different distribution, you need to install the dependencies
> yourself.

The following sections explain how to set up the environment for building the
project. Since you need a specific version of Qt6, it is recommended to use the
online installer of Qt which is included in this project. Do the following:

- Clone the project:

  ```bash
  git clone https://github.com/BartSte/snapshot
  ```

  and `cd` into root directly the project (containing the `.git` directory).

- Register a Qt account on the [website of Qt](https://www.qt.io).

- Run the `login-qt` script with your credentials:

  ```bash
  ./scripts/login-qt --pw <password> --email <email>
  ```

  This creates a `qtaccount.ini` file at `~/.local/share/Qt` which is used when
  downloading Qt.

- Run the `setup` script to install the dependencies:

  ```bash
  ./scripts/setup
  ```

  You can pass the `--static` (default) or `--shared` option to the script, to
  build Qt as a static or shared library. The `--static` option results in a
  longer build time, when compared do `--shared`, as Qt and ffmpeg are built
  from source. The `--shared` option results in a less build time as pre built
  Qt and ffmpeg libraries are used. However, the `--shared` option results in
  more libraries (\*.so files) that need to be installed on your system.

  Once, the script is done, you can find the Qt libraries in the `3rdparty/Qt`
  directory (ffmpeg is included). If you choose the `--shared` option, you can
  also find the ffmpeg libraries in the `3rdparty/ffmpeg` directory.

- Ensure that the root of the project is your current work directory. Run the
  `configure` script to configure the build:

  ```bash
  ./scripts/configure
  ```

  This script creates a `build` directory containing the ninja build files. You
  can pass `-- -DDBUILD_TESTING=ON` to the script to enable the tests. Here,
  all arguments after the `--` are passed to `cmake` directly.

- Build the project:

  ```bash
  cmake --build ./build
  ```

  This creates the `snapshot` executable in the `build/bin` directory.

- Install the project:

  ```bash
  cmake --install ./build
  ```

  - TODO: test installing the project for both static and shared qt builds.

#### Building

- Needs an update!

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

#### Distribution

##### Tar.gz archive

Contains:

- `snapshot` executable in the form of a bash script. This script sets the
  `LD_LIBRARY_PATH` to the `lib` directory and runs the `snapshot` binary.
- `bin` directory with the `snapshot` binary.
- `lib` directory with the shared libraries that the `snapshot` binary depends
  on.

The `lib` directory does not include the standard c/c++ libraries (`libc`,
`libstdc++`, `libm`, `libdbm`, `libpthread`) toghether with the dynamic linker
as they are assumed to be present on the system. The following libraries are
included, using a general categorization based on common software packages and
systems:

- OpenGL / Graphics Libraries

  - libEGL.so - Part of the OpenGL ES and EGL graphics libraries.
  - libGLX.so - Part of the X11 GLX (OpenGL Extension to the X Window System).
  - libGLdispatch.so - Associated with the OpenGL dispatch library.
  - libOpenGL.so - Part of the OpenGL graphics system.
  - libgbm.so - Mesa's Generic Buffer Management (GBM) for handling graphics buffers.
  - libdrm.so - Direct Rendering Manager (DRM), part of the Linux kernel graphics.
  - libvdpau.so - Video Decode and Presentation API for Unix.

- X11 Libraries

  - libICE.so - Inter-Client Exchange (ICE) protocol library.
  - libSM.so - Session Management library.
  - libX11-xcb.so, libX11.so - X11 client-side library.
  - libXau.so - X11 authorization library.
  - libXdmcp.so - X Display Manager Control Protocol library.
  - libXext.so - X11 extensions library.
  - libXrender.so - X Rendering Extension library.
  - libxcb-\*.so - X protocol C-language Binding (XCB) libraries.
  - libxkbcommon-x11.so, libxkbcommon.so - Keyboard handling libraries for X11.

- Audio Libraries

  - libFLAC.so - Free Lossless Audio Codec library.
  - libasyncns.so - Asynchronous name service query library.
  - libogg.so - Ogg bitstream format library.
  - libopus.so - Opus audio codec library.
  - libpulse.so, libpulsecommon-15.99.so - PulseAudio sound system libraries.
  - libsndfile.so - Library for reading and writing files containing sampled sound.
  - libvorbis.so, libvorbisenc.so - Vorbis audio compression library.

- System Libraries

  - libapparmor.so - AppArmor library.
  - libblkid.so - Block device ID library.
  - libcap.so - POSIX capabilities library.
  - libdbus-1.so - D-Bus message bus system library.
  - libffi.so - Foreign Function Interface library.
  - libgcrypt.so - Cryptographic library.
  - libglib-2.0.so, libgmodule-2.0.so, libgio-2.0.so, libgobject-2.0.so - GLib library of C routines.
  - libsystemd.so - systemd system and service manager library.
  - libudev.so - udev device manager library.

- Compression and Encoding Libraries

  - libbrotlicommon.so, libbrotlidec.so - Brotli compression library.
  - libbz2.so - Bzip2 compression library.
  - libdeflate.so - Deflate compression library.
  - liblz4.so - LZ4 compression library.
  - liblzma.so - LZMA compression library.
  - libz.so, libzstd.so - zlib and Zstandard compression libraries.

- Miscellaneous Libraries
  - libbsd.so - BSD library for various functions.
  - libexpat.so - XML parsing C library.
  - libfmt.so - Formatting library.
  - libfontconfig.so, libfreetype.so - Font libraries.
  - libgcc_s.so, libstdc++.so - GCC low-level support library and standard C++ library.
  - libgmp.so, libnettle.so, libhogweed.so - Cryptographic libraries.
  - libgnutls.so - GNU TLS library.
  - libgpg-error.so - GPG error reporting library.
  - libgraphite2.so, libharfbuzz.so - Font shaping libraries.
  - libicudata.so, libicui18n.so, libicuuc.so - International Components for Unicode libraries.
  - libidn2.so - International domain name library.
  - libjbig.so, libjpeg.so, libpng16.so, libtiff.so, libwebp.so - Image format libraries.
  - libmd.so - Message Digest library.
  - libmount.so, libuuid.so - Device mounting and UUID libraries.
  - libmp3lame.so - MP3 encoding library.
  - libnuma.so - NUMA (Non-Uniform Memory Access) library.
  - libp11-kit.so - PKCS#11 toolkit library.
  - libpcre.so, libpcre2-8.so, libpcre2-16.so - Perl Compatible Regular Expression libraries.
  - libselinux.so - SELinux library.
  - libtasn1.so - ASN.1 structure parsing library.
  - libunistring.so - Unicode string library.
  - libwayland-\*.so - Wayland protocol libraries.

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

- [ ] Configure `cmake --install` for both static and shared qt builds. This is
      needed after building the project from source.
    - [ ] Ensure that the tarball can also be made using the --shared option. 
      - The current issue is that the needed plugins are not added by the
      install command. Therefore, the tarball does not work.
- [ ] Cross compile for raspberry pi
- [ ] Release a 64bit and a 32bit version of the app
- [ ] Add to docs:

  - How to build static and shared
  - How to run the tar archive
  - Requirements for running the app:
    - Tested and released on ubuntu 22.04
    - Also used on Arch linux
    - Older versions of ubuntu might not work, as Qt 6.6 does not support
      older versions of ubuntu.

- [ ] Add to FAQ:

  - WSL + ubuntu:
    App works find, but I get the following errors/warnings:

    - Fontconfig error: Cannot load default config file

      - install fontconfig on system: refer to https://doc.qt.io/qt-6/qt-embedded-fonts.html in faq

  - Issues with your graphics card need to be resolved by the user itself. You
    need to setup your graphics card yourself. Here are some tips:

    For Intel graphics cards:

    - Most Linux-based* distributions already include Intel® Graphics Drivers.
      These drivers are provided and maintained by the Linux* distribution
      vendors and not by Intel, we recommend contacting the Linux\* Operating
      System Vendors.
    - On arch, ffmpeg gave warnings that harware acceleration did not work.
      After installing the `libva-intel-driver` the issue was resolved.

    For Nvidia graphics cards:

    - TODO

    For AMD graphics cards:

    - TODO

    Checkout the arch wiki for more information:

    - [General](https://wiki.archlinux.org/title/Hardware_video_acceleration)
    - [Intel](https://wiki.archlinux.org/title/Intel_graphics)
    - [Nvidia](https://wiki.archlinux.org/title/NVIDIA)
    - [Nouveau](https://wiki.archlinux.org/title/Nouveau)
    - [AMD](https://wiki.archlinux.org/title/AMDGPU)
