//------------------------------------------------------------------
//
//this file is part of Proximodo
//Copyright (C) 2004 Antony BOUCHER ( kuruden@users.sourceforge.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
//------------------------------------------------------------------
// Modifications: (date, author, description)
//
//------------------------------------------------------------------

#include "zlibbuffer.h"
//#include "const.h"
#include <memory>
#include <assert.h>
#include <Windows.h>

#define ZLIB_BLOCK 4096

// #pragma comment(lib, "zlib.lib")

using namespace std;

/* Constructor
 */
CZlibBuffer::CZlibBuffer(bool shrink, bool modeGzip /*= false*/)
{
    freed = true;

	reset(shrink, modeGzip);
}


/* Destructor
 */
CZlibBuffer::~CZlibBuffer()
{
	freemem();
}


/* Free zlib stream's internal state
 */
void CZlibBuffer::freemem() {

    if (!freed) {
        if (shrink) {
            deflateEnd(&stream);
        } else {
            inflateEnd(&stream);
        }
        freed = true;
    }
}


/* Reset zlib stream and make it a deflating/inflating stream
 */
bool CZlibBuffer::reset(bool shrink, bool modeGzip) {
	
	freemem();
    this->shrink = shrink;
    this->modeGzip = modeGzip;
    buffer.clear();
    output.str("");
	SecureZeroMemory(&stream, sizeof(stream));
    stream.zalloc = Z_NULL;
    stream.zfree  = Z_NULL;
    stream.opaque = Z_NULL;
    if (shrink) {
        if (modeGzip) {
            err = deflateInit2(&stream, Z_BEST_SPEED, Z_DEFLATED,
                               31, 8, Z_DEFAULT_STRATEGY);
        } else {
            err = deflateInit(&stream, Z_BEST_SPEED);
        }
    } else {
		if (modeGzip) {
			err = inflateInit2(&stream, 47);
		} else {
			err = inflateInit2(&stream, -15);
		}
    }
    if (err != Z_OK) 
		return false;
    freed = false;
    return true;
}


/* Provide data to the container.
 * As much data as possible is immediately processed.
 */
void CZlibBuffer::feed(const std::string& data)
{
    if (freed)	
		return;		// reset���Ă΂�Ă��Ȃ���΋A��

    buffer += data;
    size_t size = buffer.size();
    size_t remaining = size;
	auto buf1 = std::make_unique<char[]>(ZLIB_BLOCK);
	auto buf2 = std::make_unique<char[]>(ZLIB_BLOCK);
	while ((err == Z_OK || err == Z_BUF_ERROR) && remaining > 0) {
        stream.next_in = (Byte*)buf1.get();
        stream.avail_in = (remaining > ZLIB_BLOCK ? ZLIB_BLOCK : static_cast<uInt>(remaining));
		memcpy_s(buf1.get(), ZLIB_BLOCK, &buffer[size - remaining], stream.avail_in);
        do {
            stream.next_out = (Byte*)buf2.get();
            stream.avail_out = (uInt)ZLIB_BLOCK;
            if (shrink) {
                err = deflate(&stream, Z_NO_FLUSH);
            } else {
                err = inflate(&stream, Z_NO_FLUSH);
            }
			assert((err != Z_NEED_DICT) &&
				   (err != Z_STREAM_ERROR) &&
				   (err != Z_DATA_ERROR) &&
				   (err != Z_MEM_ERROR));

            output << string(buf2.get(), ZLIB_BLOCK - stream.avail_out);
        } while (err == Z_OK && stream.avail_out < ZLIB_BLOCK / 10);
		// ���͂������Ă��Ȃ�������I��
        if (stream.next_in == (Byte*)buf1.get()) 
			break;
        remaining -= ((char*)stream.next_in - buf1.get());
    }
    buffer = buffer.substr(size - remaining);
}


/* Tells the zlib stream that there will be no more input, and ask it
 * for the remaining output bytes.
 */
void CZlibBuffer::dump() {

    if (shrink) {
        size_t size = buffer.size();
		auto buf1 = std::make_unique<char[]>(size);
		auto buf2 = std::make_unique<char[]>(ZLIB_BLOCK);
        stream.next_in = (Byte*)buf1.get();
        stream.avail_in = static_cast<uInt>(size);
        for (size_t i=0; i<size; i++) buf1[i] = buffer[i];
        do {
            stream.next_out = (Byte*)buf2.get();
            stream.avail_out = (uInt)ZLIB_BLOCK;
            err = deflate(&stream, Z_FINISH);
            output << string(buf2.get(), ZLIB_BLOCK - stream.avail_out);
        } while (err == Z_OK);
		freemem();
    } else {
		// �f�o�b�O�����p
		assert( err == Z_OK || err == Z_STREAM_END );
#if 0
		if (!(err == Z_OK || err == Z_STREAM_END)) {
			{
				std::ofstream	fsRaw(Misc::GetExeDirectory() + _T("Raw.txt"), std::ios::out | std::ios::binary);
				fsRaw.write(m_debugRaw.c_str(), m_debugRaw.size());
			}
			{
				std::ofstream	fsDecompressed(Misc::GetExeDirectory() + _T("Decompressed.txt"), std::ios::out | std::ios::binary);
				fsDecompressed.write(m_debugDecompressed.c_str(), m_debugDecompressed.size());
			}
		}
#endif
	}
}


/* Get processed data. It is removed from the container.
 */
std::string CZlibBuffer::read() {

    std::string data = output.str();
    output.str("");
	return data;
}

