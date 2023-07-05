#include <viewer.h>
#include <gtest/gtest.h>
#include <qgraphicsview.h>
#include <qwidget.h>

TEST(ViewerTest, BasicAssertions) {
  QWidget widget;
  QGraphicsView graphics_view(&widget);
  VideoViewer viewer(&graphics_view);
  EXPECT_EQ(viewer.x, 0);
}
