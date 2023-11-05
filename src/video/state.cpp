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
    spdlog::info("VideoState::Started");
    return VideoState::Started;
  } else {
    spdlog::info("VideoState::Stopped");
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
    spdlog::info("VideoState::Stopped");
    return VideoState::Stopped;
  case QMediaPlayer::PlaybackState::PlayingState:
    spdlog::info("VideoState::Started");
    return VideoState::Started;
  case QMediaPlayer::PlaybackState::PausedState:
    spdlog::info("VideoState::Paused");
    return VideoState::Paused;
  }
}
