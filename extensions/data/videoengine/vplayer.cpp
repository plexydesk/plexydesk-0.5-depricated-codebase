/*******************************************************************************
* This file is part of PlexyDesk.
*  Maintained by : Siraj Razick <siraj@kde.org>
*  Authored By  : Siraj Razick <siraj@kde.org>
*
*  PlexyDesk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  PlexyDesk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with PlexyDesk. If not, see <http://www.gnu.org/licenses/lgpl.html>
*******************************************************************************/

#include "vplayer.h"

#ifdef _MSC_VER
extern "C"
{
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
}
#else
#include <ffmpeg/avcodec.h>
#include <ffmpeg/avformat.h>
#endif

namespace PlexyDesk{

	class VPlayer::Private
	{
	public:
		Private ()
		{
		}
		~Private ()
		{
		}
		AVFormatContext * pFormatCtx;
		int videoStream;
		AVCodecContext * pCodecCtx;
		AVCodec * pCodec;
		AVFrame * pFrame;
		AVFrame * pFrameRGB;
		AVPacket packet;
		int frameFinished;
		int numBytes;
		uint8_t * buffer;
		QLabel * video;
		QTimer * vidtimer;
		QImage * currentFrame;
	};

	VPlayer::VPlayer (QObject * parent):QObject (parent), d (new Private)
	{
		init ();
		d->vidtimer = new QTimer(this);
		connect(d->vidtimer,SIGNAL(timeout()),this,SLOT(decode()));
		d->currentFrame = 0;
	}

	VPlayer::~VPlayer ()
	{
	}

	void VPlayer::decode()
	{
		if(av_read_frame(d->pFormatCtx, &d->packet)>= 0 )
		{
			if (d->packet.stream_index==d->videoStream)
			{
				avcodec_decode_video(d->pCodecCtx, d->pFrame, &d->frameFinished,d->packet.data, d->packet.size);

				if (d->frameFinished)
				{
					img_convert((AVPicture *)d->pFrameRGB, PIX_FMT_RGBA32, (AVPicture*)d->pFrame,PIX_FMT_YUV420P,d->pCodecCtx->width,d->pCodecCtx->height);
					d->currentFrame = new QImage(d->pFrameRGB->data[0],d->pCodecCtx->width,d->pCodecCtx->height,QImage::Format_ARGB32);
					//d->video->setPixmap(QPixmap::fromImage(*d->currentFrame));
					emit frameReady(*d->currentFrame);
					//		delete d->currentFrame;
				}
				else
				{
					qDebug("Video not ready");
				}
			}
		}
		else
		{
			emit videoDone();
			d->vidtimer->stop();
		}	

		av_free_packet(&d->packet);
	}

	void VPlayer::init ()
	{
		av_register_all ();
		// d->vidtimer->start(500);
	}

	void VPlayer::setFileName (const QString & name)
	{
		QFile *file = new QFile (name);
		// init();    

		if (file->exists ())
		{
			qDebug () << "Loading Media from " << name << endl;

			if (av_open_input_file (&d->pFormatCtx, name.toLatin1 (), NULL, 0, NULL)
				!= 0)
			{
				qDebug () << "av_open_input_file" << "Failed" << endl;
			}

			if (av_find_stream_info (d->pFormatCtx) < 0)
			{
				qDebug () << "av_find_stream_info" << "Failed" << endl;
			}

			dump_format(d->pFormatCtx, 0, name.toLatin1(), 0);

			d->videoStream = -1;
			unsigned int i;
			for (i=0; i < d->pFormatCtx->nb_streams; i++) {
				if (d->pFormatCtx->streams[i]->codec->codec_type==CODEC_TYPE_VIDEO) {
					d->videoStream = i;
					break;
				}
			}

			if (d->videoStream  == -1) {
				qDebug()<<"Null Video"<<endl;
				return ; 
			}

			d->pCodecCtx=d->pFormatCtx->streams[d->videoStream]->codec;
			d->pCodec=avcodec_find_decoder(d->pCodecCtx->codec_id);
			if (d->pCodec==NULL) {
				qDebug("No Suitable Codec Found Sorry");
			}

			if (avcodec_open(d->pCodecCtx, d->pCodec)<0) {
				qDebug("Can't open Codec") ; // Could not open codec
			}

			d->pFrame=avcodec_alloc_frame();

			d->pFrameRGB=avcodec_alloc_frame();

			d->numBytes=avpicture_get_size(PIX_FMT_RGBA32, d->pCodecCtx->width, d->pCodecCtx->height);

			qDebug()<<"Byte Allocatoin = "<<d->numBytes<<endl;
			d->buffer=(uint8_t *)av_malloc(d->numBytes*sizeof(uint8_t));

			avpicture_fill((AVPicture *)d->pFrameRGB, d->buffer, PIX_FMT_RGBA32, d->pCodecCtx->width, d->pCodecCtx->height);

			i=0;
		}
		else
		{
			qDebug ("File Dose not Exisit");
			return;
		}

		delete file;

		d->vidtimer->start(12);
	}
}

#include "vplayer.moc"
