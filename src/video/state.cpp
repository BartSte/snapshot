#include <spdlog/spdlog.h>

#include "./state.hpp"

using PlaybackState = QMediaPlayer::PlaybackState;

const std::map<PlaybackState, VideoState> STATE_MAP = {
    {PlaybackState::StoppedState, VideoState::Stopped},
    {PlaybackState::PlayingState, VideoState::Started},
    {PlaybackState::PausedState, VideoState::Paused}};

/**
 * @brief convertState
 *
 * Convert a bool to a VideoState.
 *
 * @param active True if the state is active, false otherwise.
 * @return The VideoState.
 */
VideoState convertState(bool active) {
  if (active) {
    return VideoState::Started;
  } else {
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
VideoState convertState(const QMediaPlayer::PlaybackState &state) {
  return STATE_MAP.at(state);
}
