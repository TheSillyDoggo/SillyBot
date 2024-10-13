/*#include "VideoRecorder.hpp"

extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}

void initializeFFmpeg() {
    avformat_network_init();
    // Add other FFmpeg initialization code here
}

AVFormatContext* fmt_ctx = nullptr;
AVCodecContext* codec_ctx = nullptr;
AVStream* video_st = nullptr;
SwsContext* sws_ctx = nullptr;
AVFrame* frame = nullptr;
AVPacket pkt;

void setupEncoder(const char* filename, int width, int height, float frameRate) {
    avformat_alloc_output_context2(&fmt_ctx, nullptr, nullptr, filename);
    if (!fmt_ctx) {
        std::cerr << "Could not create output context." << std::endl;
        return;
    }

    const AVCodec* codec = avcodec_find_encoder(AV_CODEC_ID_H264);
    if (!codec) {
        std::cerr << "Codec not found." << std::endl;
        return;
    }

    video_st = avformat_new_stream(fmt_ctx, codec);
    if (!video_st) {
        std::cerr << "Could not create stream." << std::endl;
        return;
    }

    codec_ctx = avcodec_alloc_context3(codec);
    codec_ctx->codec_id = codec->id;
    codec_ctx->width = width;
    codec_ctx->height = height;
    codec_ctx->time_base = {1, (int)frameRate};
    codec_ctx->framerate = { (int)frameRate, 1 };
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P;

    if (fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
        codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        std::cerr << "Could not open codec." << std::endl;
        return;
    }

    avcodec_parameters_from_context(video_st->codecpar, codec_ctx);

    av_dump_format(fmt_ctx, 0, filename, 1);

    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        if (avio_open(&fmt_ctx->pb, filename, AVIO_FLAG_WRITE) < 0) {
            std::cerr << "Could not open output file." << std::endl;
            return;
        }
    }

    if (avformat_write_header(fmt_ctx, nullptr) < 0) {
        std::cerr << "Error occurred when opening output file." << std::endl;
        return;
    }

    frame = av_frame_alloc();
    frame->format = codec_ctx->pix_fmt;
    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;
    av_image_alloc(frame->data, frame->linesize, codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt, 32);

    sws_ctx = sws_getContext(width, height, AV_PIX_FMT_RGBA, width, height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);
}

bool VideoRecorder::initWithPlayLayer(GJGameLevel* level)
{
    if (!CCNode::init())
        return false;
    
    auto pl = PlayLayer::scene(level, false, false);
    gameLayer = getChildOfType<PlayLayer>(pl, 0);
    gameLayer->retain();
    pl->removeAllChildrenWithCleanup(false);
    gameLayer->release();

    gameLayer->m_started = true;
    gameLayer->startGame();

    renderFrames(30, 30);
    
    this->addChild(gameLayer);
    return true;
}

VideoRecorder* VideoRecorder::createWithPlayLayer(GJGameLevel* level)
{
    auto pRet = new VideoRecorder();

    if (pRet && pRet->initWithPlayLayer(level))
    {
        pRet->autorelease();
        return pRet;
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void VideoRecorder::renderFrames(int frames, float frameRate)
{
    const char* outputFileName = "asdf.mp4";

    initializeFFmpeg();
    setupEncoder(outputFileName, 1920, 1080, frameRate);

    auto tex = CCRenderTexture::create(1920, 1080);

    for (size_t i = 0; i < frames; i++) {
        CCScheduler::get()->update(1.0f / frameRate);

        tex->begin();
        this->visit();
        tex->end();

        // Capture frame data
        auto image = tex->newCCImage();
        auto pixelData = image->getData();
        int srcStride = 4 * 1920; // 4 bytes per pixel (RGBA)

        uint8_t *srcData[1] = { pixelData };
        sws_scale(sws_ctx, srcData, &srcStride, 0, 1080, frame->data, frame->linesize);

        frame->pts = i;
        av_init_packet(&pkt);
        pkt.data = nullptr;
        pkt.size = 0;

        log::info("frame: {}", i);

        if (avcodec_send_frame(codec_ctx, frame) < 0) {
            std::cerr << "Error sending frame." << std::endl;
            break;
        }

        while (avcodec_receive_packet(codec_ctx, &pkt) == 0) {
            av_interleaved_write_frame(fmt_ctx, &pkt);
            av_packet_unref(&pkt);
        }
    }

    // Finalize and clean up
    av_write_trailer(fmt_ctx);
    avcodec_free_context(&codec_ctx);
    av_frame_free(&frame);
    av_packet_unref(&pkt);
    sws_freeContext(sws_ctx);
    if (!(fmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        avio_closep(&fmt_ctx->pb);
    }
    avformat_free_context(fmt_ctx);

    tex->release();
}*/