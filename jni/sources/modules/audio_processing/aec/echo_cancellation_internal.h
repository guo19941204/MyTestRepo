/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_MODULES_AUDIO_PROCESSING_AEC_ECHO_CANCELLATION_INTERNAL_H_
#define WEBRTC_MODULES_AUDIO_PROCESSING_AEC_ECHO_CANCELLATION_INTERNAL_H_

#include "modules/audio_processing/aec/aec_core.h"

typedef struct {
  int delayCtr;
  int sampFreq;
  int splitSampFreq;
  int scSampFreq;
  float sampFactor;  // scSampRate / sampFreq
  short nlpMode;
  short autoOnOff;
  short activity;
  short skewMode;
  int bufSizeStart;
  int knownDelay;

  short initFlag;  // indicates if AEC has been initialized

  // Variables used for averaging far end buffer size
  short counter;
  int sum;
  short firstVal;
  short checkBufSizeCtr;

  // Variables used for delay shifts
  short msInSndCardBuf;
  short filtDelay;  // Filtered delay estimate.
  int timeForDelayChange;
  int ECstartup;
  int checkBuffSize;
  short lastDelayDiff;

#ifdef WEBRTC_AEC_DEBUG_DUMP
  void* far_pre_buf_s16;  // Time domain far-end pre-buffer in int16_t.
  FILE* bufFile;
  FILE* delayFile;
  FILE* skewFile;
#endif

  // Structures
  void* resampler;

  int skewFrCtr;
  int resample;  // if the skew is small enough we don't resample
  int highSkewCtr;
  float skew;

  void* far_pre_buf;  // Time domain far-end pre-buffer.

  int lastError;

  aec_t* aec;
} aecpc_t;

#endif  // WEBRTC_MODULES_AUDIO_PROCESSING_AEC_ECHO_CANCELLATION_INTERNAL_H_
