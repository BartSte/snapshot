#include "./videoscene.h"
#include <iostream>
#include <qgraphicsscene.h>
#include <qwidget.h>

/**
 * @brief VideoScene::VideoScene
 *
 * @param mainWindow A pointer to a QWidget.
 */
VideoScene::VideoScene(QWidget *mainWindow)
    : QGraphicsScene(mainWindow),

      constRatio(0.8),
      textOffset(10),
      textFontSize(16),
      textFont("Arial"),

      pixmapItem(new QGraphicsPixmapItem()),
      textItem(new QGraphicsTextItem()) {

  setPixmap(":/images/disconnected.png");
  setText("Disconnected");
}

/**
 * @brief VideoScene::setPixmap
 *
 * @param path The path to the image.
 * Set the image to be displayed in the scene.
 */
void VideoScene::setPixmap(std::string path) {
  QString qpath = QString::fromStdString(path);
  QPixmap pixmap(qpath);
  pixmapItem->setPixmap(pixmap);
  addItem(pixmapItem);
}

/**
 * @brief VideoScene::setText
 *
 * @param text The text to be displayed.
 * Set the text to be displayed in the scene.
 */
void VideoScene::setText(std::string text) {
  QString qtext = QString::fromStdString(text);
  QFont font(textFont, textFontSize);

  textItem->setPlainText(qtext);
  textItem->setFont(font);
  addItem(textItem);
}

/**
 * @brief VideoScene::resizeEvent
 * @param event The resize event.
 * Resize the QGraphicsPixmapItem and the QGraphicsTextItem when the scene is
 * resized.
 */
void VideoScene::scalePixmap() {
  QPixmap image = pixmapItem->pixmap();

  float widthRatio = sceneRect().width() / image.size().width();
  float heightRatio = sceneRect().height() / image.size().height();
  float aspectRatio = std::min(widthRatio, heightRatio);
  float clippedRatio = std::min(aspectRatio, 1.0f);

  pixmapItem->setScale(clippedRatio * constRatio);
}

/**
 * @brief VideoScene::centerPixmap
 * Center the QGraphicsPixmapItem in the scene.
 */
void VideoScene::centerPixmap() {
  float scale = pixmapItem->scale();
  QPixmap image = pixmapItem->pixmap();

  float widthImage = image.size().width() * scale;
  float heightImage = image.size().height() * scale;
  float x = (sceneRect().width() - widthImage) / 2;
  float y = (sceneRect().height() - heightImage) / 2;

  pixmapItem->setPos(x, y);
}

/**
 * @brief VideoScene::positionText
 * Position the text below the QGraphicsPixmapItem, in the center of the scene.
 */
void VideoScene::setPosText() {
  float scale = pixmapItem->scale();
  QPixmap image = pixmapItem->pixmap();

  float widthText = textItem->boundingRect().width();
  float heightImage = image.size().height() * scale;

  float x = (sceneRect().width() / 2) - (widthText / 2);
  float y = (sceneRect().height() - heightImage) / 2 + heightImage;

  textItem->setPos(x, y + textOffset);
}

/**
 * @brief VideoScene::~VideoScene
 * Destructor.
 */
VideoScene::~VideoScene() {
  delete pixmapItem;
  delete textItem;
}
