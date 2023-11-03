WORK IN PROGRESS

## Summary

### Features

## Dependencies

Ensure the following dependencies are installed:

- [Qt6](https://www.qt.io/download)
  - base
  - multimedia (ffmpeg)
- [cxxopts](https://github.com/jarro2783/cxxopts)
- boost
- spdlog

For building the project, the following dependencies are required:

- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)

## Building

### Linux

- TODO

### Raspberry Pi OS

- TODO

### Windows

- Not yet supported

### MacOS

- Not yet supported

### Usage

## Development

### Running the rtsp stream

```bash
./bin/rtsp-stream src/static/sample.mp4 rtsp://localhost:8090/stream
```

### Running the udp stream

```bash
./bin/udp-stream src/static/sample.mp4 udp://localhost:8090
```

## License

## Contributing

# TODO:

- [ ] Add recording snapshot at an interval
- [ ] How to add cameras to the config?
- [ ] Immediately opening of gui, do not wait till connected
- [ ] display connection status
- [ ] When connection is lost, what happens?
- [ ] Enable reconnection when disconnected
- [ ] Refactor all modules
- [ ] Write README.md
