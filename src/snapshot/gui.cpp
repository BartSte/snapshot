#include <gui/mainwindow.hpp>
#include <qobject.h>
#include <qpalette.h>
#include <string>

#include "./gui.hpp"
#include "video/connect.hpp"

const char *Gui::noCameraText = "No camera available";
const char *Gui::pixmapPath = ":/disconnected.png";

/**
 * @brief Constructor
 *
 */
Gui::Gui(QObject *parent) : QObject(parent), window() {
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
void Gui::setVideo(std::unique_ptr<BaseVideo> camera) {
  window.scene.setVideo(std::move(camera));
}

/**
 * @brief show
 *
 * Show the GUI.
 */
void Gui::show() { window.show(); }
