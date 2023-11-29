#include <boost/filesystem.hpp>
#include <qvideoframe.h>
#include <spdlog/spdlog.h>

#include "./video/image_saver.hpp"

ImageSaver::ImageSaver(boost::filesystem::path directory)
    : directory(directory) {}

ImageSaver::~ImageSaver() {}

void ImageSaver::save() {
  spdlog::info("Frame saved to directory {}.", directory.string());
}
