# README

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
  - Issues:

    - On WSL I get the following error:

    ```
    snapshot-0.0.0-Linux/snapshot -g
    [2024-01-09 18:18:41.573] [warning] [: @ ] No cameras found
    MESA: error: ZINK: vkCreateInstance failed (VK_ERROR_INCOMPATIBLE_DRIVER)
    libEGL warning: egl: failed to create dri2 screen
    ```

    - On ubuntu, fresh install, I get the following error:

    ```
    barts@zbook:~$ ./snapshot-0.0.0-Linux/snapshot -h
      1012:     __vdso_timeSegmentation fault
    ```

- [ ] Cross compile for raspberry pi
