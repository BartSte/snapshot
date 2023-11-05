#pragma once

#include <qmediaplayer.h>

enum class VideoState {
  Stopped = 0,
  Paused = 1,
  Started = 2,
};

VideoState convertState(bool is_active);
VideoState convertState(const QMediaPlayer::PlaybackState &state);
