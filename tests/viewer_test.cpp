#include <videoscene.h>
#include <gtest/gtest.h>
#include <qgraphicsview.h>
#include <qwidget.h>

TEST(VideoSceneTest, BasicAssertions) {
  QWidget widget;
  QGraphicsView graphics_view(&widget);
  VideoScene viewer(&widget);
  EXPECT_EQ(0, 0);
}
