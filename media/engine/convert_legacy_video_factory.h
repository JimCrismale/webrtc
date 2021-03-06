/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef MEDIA_ENGINE_CONVERT_LEGACY_VIDEO_FACTORY_H_
#define MEDIA_ENGINE_CONVERT_LEGACY_VIDEO_FACTORY_H_

#include <memory>
#include <utility>

#include "media/engine/webrtcvideodecoderfactory.h"
#include "media/engine/webrtcvideoencoderfactory.h"
#include "rtc_base/deprecation.h"
#include "rtc_base/system/rtc_export.h"

namespace webrtc {
class VideoEncoderFactory;
class VideoDecoderFactory;
}  // namespace webrtc

namespace cricket {

RTC_EXPORT std::unique_ptr<webrtc::VideoEncoderFactory>
DEPRECATED_ConvertVideoEncoderFactory(
    std::unique_ptr<WebRtcVideoEncoderFactory> external_encoder_factory);

RTC_EXPORT std::unique_ptr<webrtc::VideoDecoderFactory>
DEPRECATED_ConvertVideoDecoderFactory(
    std::unique_ptr<WebRtcVideoDecoderFactory> external_decoder_factory);

// Adds internal SW codecs, simulcast, SW fallback wrappers, and converts to the
// new type of codec factories. The purpose of these functions is to provide an
// easy way for clients to migrate to the API with new factory types.
// TODO(magjed): Remove once old factories are gone, webrtc:7925.
RTC_DEPRECATED RTC_EXPORT inline std::unique_ptr<webrtc::VideoEncoderFactory>
ConvertVideoEncoderFactory(
    std::unique_ptr<WebRtcVideoEncoderFactory> external_encoder_factory) {
  return DEPRECATED_ConvertVideoEncoderFactory(
      std::move(external_encoder_factory));
}

RTC_DEPRECATED RTC_EXPORT inline std::unique_ptr<webrtc::VideoDecoderFactory>
ConvertVideoDecoderFactory(
    std::unique_ptr<WebRtcVideoDecoderFactory> external_decoder_factory) {
  return DEPRECATED_ConvertVideoDecoderFactory(
      std::move(external_decoder_factory));
}

}  // namespace cricket

#endif  // MEDIA_ENGINE_CONVERT_LEGACY_VIDEO_FACTORY_H_
