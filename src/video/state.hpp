#pragma once

#include <qmediaplayer.h>

enum class VideoState {
  Stopped = 0,
  Paused = 1,
  Started = 2,
};

const VideoState convertState(bool is_active);
const VideoState convertState(const QMediaPlayer::PlaybackState &state);
