#ifdef __cplusplus
#import "whisper.h"
#import "rn-whisper.h"
#endif

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioQueue.h>

#define NUM_BUFFERS 3
#define DEFAULT_MAX_AUDIO_SEC 30

typedef struct {
    __unsafe_unretained id mSelf;

    int jobId;
    NSDictionary* options;

    bool isTranscribing;
    bool isRealtime;
    bool isCapturing;
    bool isStoppedByAction;
    int maxAudioSec;
    int nSamplesTranscribing;
    NSMutableArray<NSValue *> *shortBufferSlices;
    NSMutableArray<NSNumber *> *sliceNSamples;
    bool isUseSlices;
    int sliceIndex;
    int transcribeSliceIndex;
    int audioSliceSec;

    AudioQueueRef queue;
    AudioStreamBasicDescription dataFormat;
    AudioQueueBufferRef buffers[NUM_BUFFERS];

    void (^transcribeHandler)(int, NSString *, NSDictionary *);
} RNWhisperContextRecordState;

@interface RNWhisperContext : NSObject {
    struct whisper_context * ctx;
    RNWhisperContextRecordState recordState;
}

+ (instancetype)initWithModelPath:(NSString *)modelPath;
- (struct whisper_context *)getContext;
- (OSStatus)transcribeRealtime:(int)jobId
    options:(NSDictionary *)options
    onTranscribe:(void (^)(int, NSString *, NSDictionary *))onTranscribe;
- (int)transcribeFile:(int)jobId
    audioData:(float *)audioData
    audioDataCount:(int)audioDataCount
    options:(NSDictionary *)options;
- (void)stopTranscribe:(int)jobId;
- (bool)isCapturing;
- (bool)isTranscribing;
- (NSDictionary *)getTextSegments;
- (void)invalidate;

@end
