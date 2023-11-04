#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include <spdlog/spdlog.h>

#include "./state.hpp"

/**
 * @brief convertState
 *
 * Convert a bool to a VideoState.
 *
 * @param active True if the state is active, false otherwise.
 * @return The VideoState.
 */
const VideoState convertState(bool active) {
  if (active) {
    SPDLOG_INFO("VideoState::Started");
    return VideoState::Started;
  } else {
    SPDLOG_INFO("VideoState::Stopped");
    return VideoState::Stopped;
  }
}

/**
 * @brief convertState
 *
 * Convert the QMediaPlayer::PlaybackState to a VideoState.
 *
 * @param state The QMediaPlayer::PlaybackState.
 * @return The VideoState.
 */
const VideoState convertState(const QMediaPlayer::PlaybackState &state) {
  switch (state) {
  case QMediaPlayer::PlaybackState::StoppedState:
    SPDLOG_INFO("VideoState::Stopped");
    return VideoState::Stopped;
  case QMediaPlayer::PlaybackState::PlayingState:
    SPDLOG_INFO("VideoState::Started");
    return VideoState::Started;
  case QMediaPlayer::PlaybackState::PausedState:
    SPDLOG_INFO("VideoState::Paused");
    return VideoState::Paused;
  }
}
