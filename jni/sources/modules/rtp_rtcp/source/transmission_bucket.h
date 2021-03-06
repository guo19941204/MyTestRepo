/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_RTP_RTCP_TRANSMISSION_BUCKET_H_
#define WEBRTC_MODULES_RTP_RTCP_TRANSMISSION_BUCKET_H_

#include <vector>

#include "typedefs.h"

namespace webrtc
{
class CriticalSectionWrapper;
class RtpRtcpClock;

class TransmissionBucket {
 public:
  TransmissionBucket(RtpRtcpClock* clock);
  ~TransmissionBucket();

  // Resets members to initial state.
  void Reset();

  // Adds packet to be sent.
  void Fill(uint16_t seq_num, uint32_t timestamp, uint16_t num_bytes);

  // Returns true if there is no packet to be sent.
  bool Empty();

  // Updates the number of bytes that can be sent for the next time interval.
  void UpdateBytesPerInterval(uint32_t delta_time_in_ms,
                              uint16_t target_bitrate_kbps);

  // Checks if next packet in line can be transmitted. Returns the sequence
  // number of the packet on success, -1 otherwise. The packet is removed from
  // the vector on success.
  int32_t GetNextPacket();

 private:
  struct Packet {
     Packet(uint16_t seq_number,
            uint32_t time_stamp,
            uint16_t length_in_bytes,
            int64_t now)
       : sequence_number(seq_number),
         timestamp(time_stamp),
         length(length_in_bytes),
         stored_ms(now),
         transmitted_ms(0) {
     }
     uint16_t sequence_number;
     uint32_t timestamp;
     uint16_t length;
     int64_t stored_ms;
     int64_t transmitted_ms;
   };

  bool SameFrameAndPacketIntervalTimeElapsed(const Packet& current_packet);

  bool NewFrameAndFrameIntervalTimeElapsed(const Packet& current_packet);

  RtpRtcpClock* clock_;
  CriticalSectionWrapper* critsect_;
  uint32_t accumulator_;
  int32_t bytes_rem_interval_;
  std::vector<Packet> packets_;
  Packet last_transmitted_packet_;
};
}  // namespace webrtc
#endif  // WEBRTC_MODULES_RTP_RTCP_TRANSMISSION_BUCKET_H_
