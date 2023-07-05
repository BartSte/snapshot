#include <viewer.h>
#include <gtest/gtest.h>
#include <qgraphicsview.h>
#include <qwidget.h>

TEST(ViewerTest, BasicAssertions) {
  QWidget widget;
  QGraphicsView graphics_view(&widget);
  VideoViewer viewer(&widget, &graphics_view);
  EXPECT_EQ(0, 0);
}
