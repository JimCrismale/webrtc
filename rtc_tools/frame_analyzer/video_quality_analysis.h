/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef RTC_TOOLS_FRAME_ANALYZER_VIDEO_QUALITY_ANALYSIS_H_
#define RTC_TOOLS_FRAME_ANALYZER_VIDEO_QUALITY_ANALYSIS_H_

#include <string>
#include <utility>
#include <vector>

#include "api/video/i420_buffer.h"
#include "rtc_tools/video_file_reader.h"

namespace webrtc {
namespace test {

struct AnalysisResult {
  AnalysisResult() {}
  AnalysisResult(int frame_number, double psnr_value, double ssim_value)
      : frame_number(frame_number),
        psnr_value(psnr_value),
        ssim_value(ssim_value) {}
  int frame_number;
  double psnr_value;
  double ssim_value;
};

struct ResultsContainer {
  ResultsContainer();
  ~ResultsContainer();

  std::vector<AnalysisResult> frames;
  int max_repeated_frames;
  int max_skipped_frames;
  int total_skipped_frames;
  int decode_errors_ref;
  int decode_errors_test;
};

// A function to run the PSNR and SSIM analysis on the test file. The test file
// comprises the frames that were captured during the quality measurement test.
// There may be missing or duplicate frames. Also the frames start at a random
// position in the original video. We should provide a statistics file along
// with the test video. The stats file contains the connection between the
// actual frames in the test file and their bar code number. There is one file
// for the reference video and one for the test video. The stats file should
// be in the form 'frame_xxxx yyyy', where xxxx is the consecutive
// number of the frame in the test video, and yyyy is the barcode number.
// The stats file could be produced by
// tools/barcode_tools/barcode_decoder.py. This script decodes the barcodes
// integrated in every video and generates the stats file. If three was some
// problem with the decoding there would be 'Barcode error' instead of yyyy.
// The stat files are used to compare the right frames with each other and
// to calculate statistics.
void RunAnalysis(const rtc::scoped_refptr<webrtc::test::Video>& reference_video,
                 const rtc::scoped_refptr<webrtc::test::Video>& test_video,
                 const char* stats_file_reference_name,
                 const char* stats_file_test_name,
                 int width,
                 int height,
                 ResultsContainer* results);

// Compute PSNR for an I420 buffer (all planes). The max return value (in the
// case where the test and reference frames are exactly the same) will be 48.
double Psnr(const rtc::scoped_refptr<I420BufferInterface>& ref_buffer,
            const rtc::scoped_refptr<I420BufferInterface>& test_buffer);

// Compute SSIM for an I420 buffer (all planes). The max return value (in the
// case where the test and reference frames are exactly the same) will be 1.
double Ssim(const rtc::scoped_refptr<I420BufferInterface>& ref_buffer,
            const rtc::scoped_refptr<I420BufferInterface>& test_buffer);

// Prints the result from the analysis in Chromium performance
// numbers compatible format to stdout. If the results object contains no frames
// no output will be written.
void PrintAnalysisResults(const std::string& label, ResultsContainer* results);

// Similar to the above, but will print to the specified file handle.
void PrintAnalysisResults(FILE* output,
                          const std::string& label,
                          ResultsContainer* results);

// The barcode number that means that the barcode could not be decoded.
const int DECODE_ERROR = -1;

// Clusters the frames in the file. First in the pair is the frame number and
// second is the number of frames in that cluster. So if first frame in video
// has number 100 and it is repeated 3 after each other, then the first entry
// in the returned vector has first set to 100 and second set to 3.
// Decode errors between two frames with same barcode, then it interprets
// the frame with the decode error as having the same id as the two frames
// around it. Eg. [400, DECODE_ERROR, DECODE_ERROR, 400] is becomes an entry
// in return vector with first==400 and second==4. In other cases with decode
// errors like [400, DECODE_ERROR, 401] becomes three entries, each with
// second==1 and the middle has first==DECODE_ERROR.
std::vector<std::pair<int, int> > CalculateFrameClusters(
    FILE* file,
    int* num_decode_errors);

// Calculates max repeated and skipped frames and prints them to stdout in a
// format that is compatible with Chromium performance numbers.
void GetMaxRepeatedAndSkippedFrames(const std::string& stats_file_ref_name,
                                    const std::string& stats_file_test_name,
                                    ResultsContainer* results);

// Gets the next line from an open stats file.
bool GetNextStatsLine(FILE* stats_file, char* line);

// Calculates the size of a I420 frame if given the width and height.
int GetI420FrameSize(int width, int height);

// Extract the sequence of the frame in the video. I.e. if line is
// frame_0023 0284, we will get 23.
int ExtractFrameSequenceNumber(std::string line);

// Checks if there is 'Barcode error' for the given line.
bool IsThereBarcodeError(std::string line);

// Extract the frame number in the reference video. I.e. if line is
// frame_0023 0284, we will get 284.
int ExtractDecodedFrameNumber(std::string line);

}  // namespace test
}  // namespace webrtc

#endif  // RTC_TOOLS_FRAME_ANALYZER_VIDEO_QUALITY_ANALYSIS_H_
