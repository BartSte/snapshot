#include <QString>
#include <QTest>
#include <gtest/gtest.h>

class TestMediaPlayer : public QObject {
  Q_OBJECT
 private slots:
  void toUpper();
};

void TestMediaPlayer::toUpper() {
  QString str = "Hello";
  QCOMPARE(str.toUpper(), QString("HELLO"));
}

TEST(testConnect, toUpper) {
  TestMediaPlayer test;
  QTest::qExec(&test);
}
#include "test_connect.moc"
