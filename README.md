# Snapshot

[![Tests](https://github.com/BartSte/snapshot/actions/workflows/tests.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/tests.yml)  
[![Release](https://github.com/BartSte/snapshot/actions/workflows/release.yml/badge.svg)](https://github.com/BartSte/snapshot/actions/workflows/release.yml)

<!--toc:start-->

- [Summary](#summary)
  - [Features](#features)
  - [Limitations](#limitations)
- [Installation](#installation)
  - [Stand-alone tarball](#stand-alone-tarball)
  - [From source](#from-source)
  - [Raspberry Pi OS](#raspberry-pi-os)
- [Usage](#usage)
  - [List the available cameras](#list-the-available-cameras)
  - [Display a video](#display-a-video)
  - [Record a video stream](#record-a-video-stream)
  - [Using the configuration file](#using-the-configuration-file)
  - [Set the logging level](#set-the-logging-level)
- [Development](#development)
  - [Test](#test)
  - [Tarball](#tarball)
  - [Debugging](#debugging)
- [License](#license)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [FAQ](#faq)
<!--toc:end-->

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

- 64 bit Linux systems only
- glibc $\geq$ 2.35 (when using the tarball)
- X11, Wayland are supported. EGLFS is support is added, but not tested.
- Tested on Ubuntu 22.04, Arch linux and Raspberry Pi OS (bullseye).

## Installation

The following sections explain how to install the project. You have two
options: using the pre-built binaries. The easiest way is to use the pre-built
binaries, i.e., the stand-alone tarball.

### Stand-alone tarball

> Requires that glibc $\geq$ 2.35

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
  ├── runtime/
  └── snapshot
```

the `runtime` directory contains files you should not touch. The `snapshot`
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

Extra information about the tarball is provided in [Development](#development).

### From source

In this section, the steps to build the project from source are explained. In
short:

- You can use the `setup` script to install the dependencies.
- A static or shared Qt build can be selected.
- A Qt account is needed in order to use the `setup` script.
- Using the `configure` script, the build can be configured.

#### Dependencies

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

#### Installing dependencies

> The `setup` script only supports Arch Linux and Ubuntu 22.04. If you are
> using a different distribution, you need to install the dependencies
> yourself.

The following sections explain how to set up the environment for building the
project. Since you need a specific version of Qt6, it is recommended to use the
online installer of Qt which is included in this project.

> Using the local installer is recommended as it installs the correct version
> in a subdirectory of the project. This way, the system Qt version is not
> affected.

Do the following:

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

#### Configure the build

Ensure that the root of the project is your current work directory. Run the
`configure` script to configure the build:

```bash
./scripts/configure
```

This script creates a `build` directory containing the ninja build files. You
can pass `-- -DDBUILD_TESTING=ON` to the script to enable the tests. Here, all
arguments after the `--` are passed to `cmake` directly.

#### Build

Build the project by running the following command:

```bash
cmake --build ./build
```

This creates the `snapshot` executable in the `build/bin` directory.

#### Install

Install the project by running the following command:

```bash
sudo cmake --install ./build
```

This will install the project in the `/opt/snapshot` directory. The `/opt`
directory is chosen as `snapshot` is a self-contained application. The installed
files tree is as follows:

```bash
/opt/snapshot
 ├── runtime/
 │   └── libs, bins, and qt plugins
 └── snapshot
```

Where `snapshot` is the entry point of the application. If you want to add the
`snapshot` executable to your `PATH`, you can create a symlink to
`/usr/local/bin`:

```bash
sudo ln -s /opt/snapshot/snapshot /usr/local/bin/snapshot
```

Or you can add the `/opt/snapshot` directory to your `PATH`.

If you have no root access, you can install the project in you home directory by
using the `--prefix` option. For example, if you want to install the project in
the `$HOME/opt/snapshot` directory, you can run the following:

```bash
cmake --install ./build --prefix $HOME/opt/snapshot
```

This will install the project in the `$HOME/opt/snapshot` directory, instead of
the `/opt/snapshot` directory.

### Raspberry Pi OS

- **TODO**

## Usage

The `snapshot` executable should be executed from the command line. The
following sections explain how to use the application. You can also run
`snapshot --help` to get more information.

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

### Set the logging level

By default, the logging level is set to `warning`. If we want to set the
logging level to `debug`, we can run the following command:

```bash
snapshot --log-level debug <other options>
```

The following logging levels are available: `trace`, `debug`, `info`,
`warning`, `error`, and `critical`.

## Development

If you want to contribute to this project, you should start by reading
[CONTRIBUTING](./CONTRIBUTING.md), and installing the project [from
source](#from-source). This section provides information about: the tarball, the
unit tests, and ways to debug the application.

### Test

Googletest and QTest are used for building the unit tests and the end-to-end
tests. To build the tests, the `BUILD_TESTING` option must be set to `ON` when
configuring cmake:

```bash
./scripts/configure --build_type Debug -- -DBUILD_TESTING=ON
```

this will use the `Debug` build type with the tests enabled. To run the tests,
run the following command:

```bash
ctest --test-dir ./build
```

### Tarball

Additional information about the stand-alone tarball is provided here.

- `snapshot` is an executable in the form of a bash script. This script:
  - sets the `LD_LIBRARY_PATH` to the `lib` directory and runs the `snapshot`
    binary.
  - resolves the path to the script if it is referenced using a symlink.
- the `runtime` directory contains shared libraries and binaries that the
  `snapshot` script depends on. In case the project is built with the `--shared`
  option, the `runtime` folder will also contain subdirectories with the
  libraries of the Qt plugins. The `runtime` directory does not include the
  standard `c/c++` libraries (`libc`, `libstdc++`, `libm`, `libdbm`,
  `libpthread`) together with the dynamic linker as they are assumed to be
  present on the system.
- `snapshot_test` is an executable in the form of a bash script. This script is
  only present when the project is built with the `cmake` option
  `-DBUILD_TESTING=ON`. If so, when running the `snapshot_test` script, all
  tests are executed.

### Debugging

You can use one of the following ways to run the application with a video stream:

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
more information. Also, read the section on [Development](#development) for
additional information.

## FAQ

#### WSL2 fontconfig

On WSL2, if you encounter the following error:

```bash
Fontconfig error: Cannot load default config file
```

install fontconfig on your system as is describe on the [website of Qt](https://doc.qt.io/qt-6/qt-embedded-fonts.html)

#### Hardware acceleration

Issues with your graphics card need to be resolved by the user itself, as this
application has no control over it. The following pages from the arch wiki are
very useful when you encounter issues with hardware acceleration:

- [General](https://wiki.archlinux.org/title/Hardware_video_acceleration)
- [Intel](https://wiki.archlinux.org/title/Intel_graphics)
- [Nvidia](https://wiki.archlinux.org/title/NVIDIA)
- [Nouveau](https://wiki.archlinux.org/title/Nouveau)
- [AMD](https://wiki.archlinux.org/title/AMDGPU)

