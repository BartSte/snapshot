#include "snapshotapp.hpp"
#include <QObject>
#include <QTest>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qvideosink.h>
#include <video/record.hpp>

using path = std::filesystem::path;
using dir_iterator = std::filesystem::directory_iterator;

class TestRecorder : public QObject {
  using ms = std::chrono::milliseconds;

 private:
  Q_OBJECT
  path tmp_dir;
  QString debug_video_qstr;
  std::unique_ptr<QApplication> app;

 private slots:

  void initTestCase() {
    int argc = 0;
    app = std::make_unique<QApplication>(argc, nullptr);
  }

  void init() {
    debug_video_qstr = QString::fromStdString(App::debug_video.string());
    tmp_dir = std::filesystem::temp_directory_path();
    tmp_dir /= "test_record";
    std::filesystem::remove_all(tmp_dir);
  }

  /**
   * @brief Test ImageSaver::save
   *
   * Start a debug video, and share its sink with an ImageSaver. After calling
   * the save method, 1 image should be saved in the temporary directory.
   */
  void testImageSaverSave() {
    MediaPlayer player(debug_video_qstr);
    QVideoSink *sink = player.getVideoSink();
    ImageSaver saver(sink, tmp_dir);

    player.start();
    QTest::qWait(250);
    QCOMPARE(player.getState(), VideoState::Start);

    saver.save();
    player.stop();
    QCOMPARE(player.getState(), VideoState::Stop);
    ASSERT_EQ(numberOfItems(tmp_dir), 1);
    ASSERT_EQ(numberOfFilesRecursive(tmp_dir), 1);
  }

  /**
   * @brief Test Recorder::start
   *
   * The recorder should save 1 image and timeout after 1 second. A MediaPlayer
   * is used to provide a QVideoSink with frames.
   */
  void testRecorderStart() {
    MediaPlayer player(debug_video_qstr);
    QVideoSink *sink = player.getVideoSink();
    Recorder recorder(sink, tmp_dir);

    player.start();
    QTest::qWait(250);
    QCOMPARE(player.getState(), VideoState::Start);

    recorder.start(ms(1000), ms(1000));
    QCOMPARE(recorder.getState(), RecorderState::Start);

    // The recorder should timeout after 1 second and go to the stop state.
    QTest::qWait(1100);
    QCOMPARE(recorder.getState(), RecorderState::Stop);

    player.stop();
    QCOMPARE(player.getState(), VideoState::Stop);

    // 1 image should be saved in the temporary directory.
    QCOMPARE(numberOfItems(tmp_dir), 1);
    ASSERT_EQ(numberOfFilesRecursive(tmp_dir), 1);
  }

  /**
   * @brief Test Recorder::start
   *
   * A nullptr is passed as the QVideoSink. The recorder should do nothing (it
   * logs a warning). When stop is called, the recorder should go to the stop
   * state.
   */
  void testRecorderNullptr() {
    Recorder recorder(nullptr, tmp_dir);

    recorder.start(ms(1000), ms(5000));
    QCOMPARE(recorder.getState(), RecorderState::Start);

    QTest::qWait(250);
    recorder.stop();
    QCOMPARE(recorder.getState(), RecorderState::Stop);
  }

  void cleanup() { std::filesystem::remove_all(tmp_dir); }
};

TEST(testRecord, testRecorder) {
  TestRecorder test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}
#include "test_record.moc"
