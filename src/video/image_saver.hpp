#pragma once

#include <filesystem>
#include <qvideoframe.h>

class ImageSaver {

 protected:
  std::filesystem::path directory;

 public:
  explicit ImageSaver(std::filesystem::path directory);
  ~ImageSaver();

  void save(const QVideoFrame &frame);
};
