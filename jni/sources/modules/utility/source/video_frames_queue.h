/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_UTILITY_SOURCE_VIDEO_FRAMES_QUEUE_H_
#define WEBRTC_MODULES_UTILITY_SOURCE_VIDEO_FRAMES_QUEUE_H_

#ifdef WEBRTC_MODULE_UTILITY_VIDEO

#include "engine_configurations.h"
#include "list_wrapper.h"
#include "typedefs.h"

namespace webrtc {
class VideoFrame;

class VideoFramesQueue
{
public:
    VideoFramesQueue();
    ~VideoFramesQueue();

    // Put newFrame (last) in the queue.
    WebRtc_Word32 AddFrame(const VideoFrame& newFrame);

    // Return the most current frame. I.e. the frame with the highest
    // VideoFrame::RenderTimeMs() that is lower than
    // TickTime::MillisecondTimestamp().
    VideoFrame* FrameToRecord();

    // Set the render delay estimate to renderDelay ms.
    WebRtc_Word32 SetRenderDelay(WebRtc_UWord32 renderDelay);

protected:
    // Make ptrOldFrame available for re-use. I.e. put it in the empty frames
    // queue.
    WebRtc_Word32 ReturnFrame(VideoFrame* ptrOldFrame);

private:
    // Don't allow the buffer to expand beyond KMaxNumberOfFrames VideoFrames.
    // 300 frames correspond to 10 seconds worth of frames at 30 fps.
    enum {KMaxNumberOfFrames = 300};

    // List of VideoFrame pointers. The list is sorted in the order of when the
    // VideoFrame was inserted into the list. The first VideoFrame in the list
    // was inserted first.
    ListWrapper    _incomingFrames;
    // A list of frames that are free to be re-used.
    ListWrapper    _emptyFrames;

    // Estimated render delay.
    WebRtc_UWord32 _renderDelayMs;
};
} // namespace webrtc
#endif // WEBRTC_MODULE_UTILITY_VIDEO
#endif  // WEBRTC_MODULES_UTILITY_SOURCE_VIDEO_FRAMES_QUEUE_H_
