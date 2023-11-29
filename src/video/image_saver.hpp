#pragma once

#include <boost/filesystem.hpp>
#include <qvideoframe.h>

class ImageSaver {

 protected:
  boost::filesystem::path directory;

 public:
  explicit ImageSaver(boost::filesystem::path directory);
  ~ImageSaver();

  void save();
};
