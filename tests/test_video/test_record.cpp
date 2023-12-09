#include "video/record.hpp"
#include <QObject>
#include <QTest>
#include <gtest/gtest.h>
#include <helpers/path.hpp>
#include <qcoreapplication.h>
#include <qvideosink.h>

using path = std::filesystem::path;
using dir_iterator = std::filesystem::directory_iterator;

extern const std::filesystem::path root;
const std::filesystem::path debug_video = root / "static" / "sample.mp4";

class TestRecorder : public QObject {
  using ms = std::chrono::milliseconds;

 private:
  Q_OBJECT
  path tmp_dir;
  QString debug_video_qstr;

 private slots:

  void initTestCase() {
    debug_video_qstr = QString::fromStdString(debug_video.string());
    tmp_dir = std::filesystem::temp_directory_path();
    tmp_dir /= "test_record";
  }

  /**
   * @brief Test ImageSaver::save
   *
   * Start a debug video, and share its sink with an ImageSaver. After calling
   * the save method, 1 image should be saved in the temporary directory.
   */
  void imageSaverSave() {
    MediaPlayer player(debug_video_qstr);
    QVideoSink *sink = player.getVideoSink();
    ImageSaver saver(sink, tmp_dir);

    player.start();
    QTest::qWait(250);
    QCOMPARE(player.getState(), VideoState::Start);

    saver.save();
    player.stop();
    QCOMPARE(player.getState(), VideoState::Stop);
    ASSERT_EQ(number_of_files(tmp_dir), 1);
  }

  void cleanup() { std::filesystem::remove_all(tmp_dir); }
};

TEST(testRecord, testRecorder) {
  TestRecorder test;
  ASSERT_EQ(QTest::qExec(&test), 0);
}
#include "test_record.moc"
