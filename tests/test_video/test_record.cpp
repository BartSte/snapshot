#include <QTest>
#include <QObject>
#include <gtest/gtest.h>

class TestRecorder : public QObject {
  Q_OBJECT

 private slots:
  void start() { ASSERT_EQ(1, 1); }
};

TEST(testRecord, TestRecorderClass) {
  TestRecorder test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}
#include "test_record.moc"
