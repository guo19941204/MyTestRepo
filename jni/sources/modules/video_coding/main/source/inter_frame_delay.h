/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_VIDEO_CODING_INTER_FRAME_DELAY_H_
#define WEBRTC_MODULES_VIDEO_CODING_INTER_FRAME_DELAY_H_

#include "typedefs.h"

namespace webrtc
{

class VCMInterFrameDelay
{
public:
    VCMInterFrameDelay(int64_t currentWallClock);

    // Resets the estimate. Zeros are given as parameters.
    void Reset(int64_t currentWallClock);

    // Calculates the delay of a frame with the given timestamp.
    // This method is called when the frame is complete.
    //
    // Input:
    //          - timestamp         : RTP timestamp of a received frame
    //          - *delay            : Pointer to memory where the result should be stored
    //          - currentWallClock  : The current time in milliseconds.
    //                                Should be -1 for normal operation, only used for testing.
    // Return value                 : true if OK, false when reordered timestamps
    bool CalculateDelay(WebRtc_UWord32 timestamp,
                        WebRtc_Word64 *delay,
                        int64_t currentWallClock);

    // Returns the current difference between incoming timestamps
    //
    // Return value                 : Wrap-around compensated difference between incoming
    //                                timestamps.
    WebRtc_UWord32 CurrentTimeStampDiffMs() const;

private:
    // Controls if the RTP timestamp counter has had a wrap around
    // between the current and the previously received frame.
    //
    // Input:
    //          - timestmap         : RTP timestamp of the current frame.
    void CheckForWrapArounds(WebRtc_UWord32 timestamp);

    WebRtc_Word64         _zeroWallClock; // Local timestamp of the first video packet received
    WebRtc_Word32         _wrapArounds;   // Number of wrapArounds detected
    // The previous timestamp passed to the delay estimate
    WebRtc_UWord32        _prevTimestamp;
    // The previous wall clock timestamp used by the delay estimate
    WebRtc_Word64         _prevWallClock;
    // Wrap-around compensated difference between incoming timestamps
    WebRtc_Word64         _dTS;
};

} // namespace webrtc

#endif // WEBRTC_MODULES_VIDEO_CODING_INTER_FRAME_DELAY_H_
