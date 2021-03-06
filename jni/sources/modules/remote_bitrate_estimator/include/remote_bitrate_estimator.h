/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

// This class estimates the incoming available bandwidth.

#ifndef WEBRTC_MODULES_REMOTE_BITRATE_ESTIMATOR_INCLUDE_REMOTE_BITRATE_ESTIMATOR_H_
#define WEBRTC_MODULES_REMOTE_BITRATE_ESTIMATOR_INCLUDE_REMOTE_BITRATE_ESTIMATOR_H_

#include <map>

#include "common_types.h"
#include "typedefs.h"

namespace webrtc {

// RemoteBitrateObserver is used to signal changes in bitrate estimates for
// the incoming streams.
class RemoteBitrateObserver {
 public:
  // Called when a receive channel group has a new bitrate estimate for the
  // incoming streams.
  virtual void OnReceiveBitrateChanged(unsigned int bitrate) = 0;

  virtual ~RemoteBitrateObserver() {}
};

class RemoteBitrateEstimator {
 public:
  enum EstimationMode {
    kMultiStreamEstimation,
    kSingleStreamEstimation
  };

  virtual ~RemoteBitrateEstimator() {}

  static RemoteBitrateEstimator* Create(RemoteBitrateObserver* observer,
                                        const OverUseDetectorOptions& options,
                                        EstimationMode mode);

  // Stores an RTCP SR (NTP, RTP timestamp) tuple for a specific SSRC to be used
  // in future RTP timestamp to NTP time conversions. As soon as any SSRC has
  // two tuples the RemoteBitrateEstimator will switch to multi-stream mode.
  virtual void IncomingRtcp(unsigned int ssrc, uint32_t ntp_secs,
                            uint32_t ntp_frac, uint32_t rtp_timestamp) = 0;

  // Called for each incoming packet. The first SSRC will immediately be used
  // for overuse detection. Subsequent SSRCs will only be used when at least
  // two RTCP SR reports with the same SSRC have been received.
  virtual void IncomingPacket(unsigned int ssrc,
                              int packet_size,
                              int64_t arrival_time,
                              uint32_t rtp_timestamp) = 0;

  // Triggers a new estimate calculation.
  virtual void UpdateEstimate(unsigned int ssrc, int64_t time_now) = 0;

  // Set the current round-trip time experienced by the streams going into this
  // estimator.
  virtual void SetRtt(unsigned int rtt) = 0;

  // Removes all data for |ssrc|.
  virtual void RemoveStream(unsigned int ssrc) = 0;

  // Returns true if a valid estimate exists and sets |bitrate_bps| to the
  // estimated bitrate in bits per second.
  virtual bool LatestEstimate(unsigned int ssrc,
                              unsigned int* bitrate_bps) const = 0;
};

}  // namespace webrtc

#endif  // WEBRTC_MODULES_REMOTE_BITRATE_ESTIMATOR_INCLUDE_REMOTE_BITRATE_ESTIMATOR_H_
