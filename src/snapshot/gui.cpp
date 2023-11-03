#include <gui/mainwindow.hpp>
#include <qapplication.h>
#include <string>

#include "./gui.hpp"

const char *Gui::noCameraText = "No camera available";
const char *Gui::pixmapPath = ":/disconnected.png";

/**
 * @brief Constructor
 *
 */
Gui::Gui(int argc, char *argv[]) : QApplication(argc, argv), window() {
  window.scene.setPixmap(pixmapPath);
  window.scene.setText(noCameraText);
}

/**
 * @brief setVideo
 *
 * Set the video stream to the given camera.
 *
 * @param camera The camera id
 */
void Gui::setVideo(const std::string &camera) { window.scene.setVideo(camera); }

/**
 * @brief show
 *
 * Show the GUI.
 */
void Gui::show() { window.show(); }
