#include <QString>
#include <QTest>
#include <chrono>
#include <filesystem>
#include <gtest/gtest.h>
#include <qapplication.h>
#include <video/connect.hpp>

using ms = std::chrono::milliseconds;
using VideoPtr = std::optional<std::unique_ptr<BaseVideo>>;

extern const std::filesystem::path root;
const std::filesystem::path debug_video = root / "static" / "sample.mp4";

class TestMediaPlayer : public QObject {

 private:
  Q_OBJECT
  std::unique_ptr<QApplication> app;

 private slots:

  void initTestCase() {
    int argc = 0;
    app = std::make_unique<QApplication>(argc, nullptr);
  }

  /**
   * @brief Tests the start function.
   *
   * A MediaPlayer object is created and the start function is called using a
   * sample video file. Only the MediaPlayer is tested (not the Camera) as
   * using a debug camera for testing in a CI environment is not worth the
   * effort.
   */
  void start() {
    QString file_qstr = QString::fromStdString(debug_video.string());
    MediaPlayer player(file_qstr);

    player.start();
    QTest::qWait(250);
    QCOMPARE(player.getState(), VideoState::Start);

    player.stop();
    QTest::qWait(250);
    QCOMPARE(player.getState(), VideoState::Stop);
  }

  /**
   * @brief Tests the timeout function.
   *
   * A MediaPlayer is created with a non-existing file. The timeout is set
   * to 1000ms. The player should be in the Search state for 1000ms and then
   * go to the Stop state.
   */
  void timeout() {
    MediaPlayer player(QString("non_existing_file.mp4"), ms(1000));

    player.start();
    QTest::qWait(100);
    QCOMPARE(player.getState(), VideoState::Search);

    QTest::qWait(1000);
    QCOMPARE(player.getState(), VideoState::Stop);
  }
};

TEST(testConnect, TestMediaPlayer) {
  TestMediaPlayer test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}

/**
 * @brief Tests the videoFactory function.
 *
 * The videoFactory function should return a MediaPlayer object when a file or
 * a stream is passed as an argument. It should return a Camera object when a
 * name of a camera is passed as an argument. In this case, there is no camera
 * available, so the function should return an empty optional.
 */
TEST(testConnect, TestVideoFactory) {
  VideoPtr null = videoFactory("non-existing-camera");
  VideoPtr file = videoFactory(debug_video.string());
  VideoPtr stream = videoFactory("rtsp://localhost:1234/test");

  ASSERT_EQ(null.has_value(), false);

  try {
    MediaPlayer *file_ptr = dynamic_cast<MediaPlayer *>(file->get());
  } catch (std::bad_cast &e) {
    FAIL() << "file is not a MediaPlayer";
  }

  try {
    MediaPlayer *stream_ptr = dynamic_cast<MediaPlayer *>(stream->get());
  } catch (std::bad_cast &e) {
    FAIL() << "stream is not a MediaPlayer";
  }
}
#include "test_connect.moc"
