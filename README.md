# README

> WORK IN PROGRESS!  
> This project is still under development. The code is still experimental and
> is subject to change.

## Table of Contents

- [Summary](#summary)
  - [Features](#features)
- [Dependencies](#dependencies)
- [Building](#building)
  - [Linux](#linux)
  - [Raspberry Pi OS](#raspberry-pi-os)
  - [Windows](#windows)
  - [Usage](#usage)
    - [List the available cameras](#list-the-available-cameras)
    - [Display a video](#display-a-video)
    - [Record a video stream](#record-a-video-stream)
    - [Set the logging level](#set-the-logging-level)
    - [Using the configuration file](#using-the-configuration-file)
- [Development](#development)
  - [Build](#build)
  - [Running the rtsp stream](#running-the-rtsp-stream)
  - [Running the udp stream](#running-the-udp-stream)
- [License](#license)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Summary

`Snapshot` is an application that allows you to view and record video in a set
of snapshots. This is useful when you want to make a timelapse of a live video.
The application has a command line interface and a graphical user interface. It
can be configured using a configuration file, or by using the command line
options. For more information, about the run `snapshot --help`, or continue
reading.

The project is written in C++ and uses the [Qt6 framework](https://www.qt.io/).

### Features

In short, `snapshot` contains the following features:

- [x] List the available cameras.
- [x] Display a video stream:
  - [x] using RTSP, UDP or HTTP.
  - [x] from a file.
  - [x] from a (built-in) camera.
- [ ] Record a video in a set of snapshots at a given interval.
- [x] Configure the application using a configuration file.
- [x] Has a command line interface.
- [ ] Runs on linux (including raspberry pi) and windows.

## Dependencies

Ensure the following dependencies are installed:

| Dependency                                                    | Version  |
| ------------------------------------------------------------- | -------- |
| [Qt6](https://www.qt.io/download): base & multimedia (ffmpeg) | ≥ 6.6.0  |
| [cxxopts](https://github.com/jarro2783/cxxopts)               | ≥ 3.1.1  |
| [boost](https://www.boost.org/)                               | ≥ 1.83   |
| [spdlog](https://github.com/gabime/spdlog)                    | ≥ 1.12.0 |

For building the project, the following dependencies are required:

- [git](https://git-scm.com/)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

## Building

The next steps explain how to build this project from source. The following
sections explain how to build the project on different environments: Linux,
Raspberry Pi OS and Windows. Despite the fact that Raspberry Pi OS is a linux
distribution, it is discussed separately, because it requires some extra steps.
It is assumed that the dependencies are installed.

For all environments, cloning the repository is the first step:

```bash
git clone https://github.com/BartSte/snapshot
```

### Linux

Create a `build` directory by running the following command:

```bash
cmake -G "Ninja" -S . -B ./build -DCMAKE_BUILD_TYPE=Release
```

This creates a `build` directory with a `Makefile` in it. To build the project,
run the following command:

```bash
cmake --build ./build
```

After this, the `snapshot` executable can be found in the `build/bin` directory.

### Raspberry Pi OS

- **TODO**

### Windows

- **TODO**

### Usage

The `snapshot` executable can be run from the command line. The following
sections explain how to use the application. You can also run `snapshot --help`
to get more information about the usage.

#### List the available cameras

By running the following command, a list of available cameras is shown:

```bash
snapshot --list
```

this outputs something like:

```bash
Index | Name                            | ID          | Is Default |
0     | HP TrueVision HD Camera: HP Tru | /dev/video0 | true       |
```

which means that there is one camera available, with index `0`, name `HP
TrueVision HD Camera: HP Tru`, id `/dev/video0` and is the default camera.

#### Display a video

If we want to display a video stream from the default camera, we can run the
following command:

```bash
snapshot --gui
```

If a camera is found, it will opens a window with the video stream from the
default camera. You can also specify the camera to use. By doing:

```bash
snapshot --gui --camera "HP TrueVision HD Camera: HP Tru"
```

This will display the video stream from the camera with the name that matches
the name given by the `--list` command in the previous section.

If we want to display a video stream from a file, we can run the following:

```bash
snapshot --gui --camera sample.mp4
```

If we want to display a video stream from the url `rtsp://localhost:8090/stream`,
we can run the following:

```bash
snapshot --gui --camera rtsp://localhost:8090/stream
```

Note that we can feed the `--camera` option with a file, a url or a camera name.

#### Record a video stream

- **TODO**

#### Set the logging level

By default, the logging level is set to `info`. If we want to set the logging
level to `debug`, we can run the following command:

```bash
snapshot --log-level debug <other options>
```

The following logging levels are available: `trace`, `debug`, `info`,
`warning`, `error`, and `critical`.

#### Using the configuration file

Instead of using the command line options, we can also use a configuration file.
By default, the application looks for a file called `config.json` in the
`$HOME/.config/snapshot` directory. If no configuration file is found, the
application uses the default configuration file:

```json
{
  "list": false,
  "gui": false,
  "camera": "default",
  "loglevel": "warning",
  "pattern": "[%Y-%m-%d %H:%M:%S.%e] [%l] [%s:%# @ %!] %v"
}
```

For example, by setting the `gui` option to `true`, the application will open
the gui by default. This is equivalent to running `snapshot --gui`. If you want
to put your configuration file in a different location, you can specify the
location using the `--config` option:

```bash
snapshot --config /path/to/config.json
```

## Development

If you want to contribute to this project, you can follow the next steps to to
build the project from source. Furthermore, some information is given about the
debugging and testing the project.

### Build

Follow the steps in the [Building](#building) section to build the project.
Instead of the `Release` build type, use the `Debug` build type.

### Running debug mode

By running the following command, the application is run in debug mode:

```bash
snapshot --debug
```

This opens the gui and plays the sample.mp4 file from the `src/static` folder.

### Running the rtsp stream

By running the following command, a video stream is published using the rtsp
protocol:

```bash
./bin/rtsp-stream src/static/sample.mp4 rtsp://localhost:8090/stream
```

This stream can be viewed using the following command:

```bash
snapshot -g -c rtsp://localhost:8090/stream
```

This displays the video stream using the GUI.

### Running the udp stream

Similar to [running the rtsp stream](#running-the-rtsp-stream), an udp stream
can be published using the following command:

```bash
./bin/udp-stream src/static/sample.mp4 udp://localhost:8090
```

This stream can be viewed using the following command:

```bash
snapshot -g -c udp://localhost:8090
```

## License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE)
file for details.

### Troubleshooting

If you encounter any issues, please report them on the issue tracker at:
[snapshot issues](https://github.com/BartSte/snapshot/issues)

# Contributing

Contributions are welcome! Please see [CONTRIBUTING](./CONTRIBUTING.md) for
more information.

# TODO:

- [ ] Add recording snapshot at an interval

  - The recorder can take a pointer to a QVideoSink of the gui, the gui owns
    this pointer.
  - The recorder can create its own QVideoSink on the stack when no gui is
    avaiable and use this for recording.
  - Create a save function that saves a frame to disk.
  - Create a timer that calls the save function at a given interval.

- [ ] Immediately opening of gui, do not wait till connected
- [ ] When connection is lost, what happens? Display connection status
- [ ] Enable reconnection when disconnected?
