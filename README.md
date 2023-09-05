WORK IN PROGRESS

# TODO:

- [ ] Immidiate opening of gui, do not wait till connected
- [ ] display connection status
- [ ] Enable reconnection when disconnected

## Dependencies

- [Qt6](https://www.qt.io/download)
  - base
  - multimedia (ffmpeg)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)
- [cxxopts](https://github.com/jarro2783/cxxopts)
- boost
- spdlog

# Development

## Running the rtsp stream

```bash
./bin/rtsp-stream extra/sample.mp4 rtsp://localhost:8090/stream
```

## Running the udp stream

```bash
./bin/udp-stream extra/sample.mp4 udp://localhost:8090
```
