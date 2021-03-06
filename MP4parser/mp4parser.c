#include "stdafx.h"
#include "mp4.h"
#include "malloc.h"
#include "string.h"


typedef struct {
   int64_t start_offset;
   uint32_t type;
   uint64_t size;
} box_t;

typedef struct {
   int first_byte;
   int second_byte;
   int third_byte;
   int four_byte;
   int offset;
   int data_len;
} sps_type;

int main(int argc, char* argv[])
{
   box_t mdat,moov;
   mp4_box_t *box,*root;
   stream_t* sb = NULL;
   sps_type sps_header, pps_header;
   int buff[1024];
   int pre_buff[1024];
   int tmp_buff[1024];
   int buff_value = 0;
   int len = 0;
   int buff_size = 0;

   long int frame_size = 0;
   int mdat_frame_size = 0;
   long int offset = 0;
   long int next_frame_size = 0;
   int mdat_len_header = 8;
   int mdat_frame_len_byte = 3;
   int shift = 8;
   int header_len = 4;


   sb = create_buf_file_stream();
   if (stream_open(sb, "pitpull.mp4", MODE_READ) == 0)
      return -1;

    //read mdat box info	
    root = MP4_BoxGetRoot(sb);
    box = MP4_BoxGet( root, "moov" ); 
    
    memcpy(&moov.start_offset, &box->i_pos, sizeof(int64_t));
    memcpy(&moov.type, &box->i_type, sizeof(uint32_t));
    memcpy(&moov.size, &box->i_shortsize, sizeof(int64_t));
    //printf("moov Start offset %d end offset %d Size %d type %x \n", moov.start_offset, moov.start_offset + moov.size, moov.size, moov.type); 

    memcpy(&mdat.start_offset, &box->p_next->i_pos, sizeof(int64_t));
    memcpy(&mdat.type, &box->p_next->i_type, sizeof(uint32_t));
    memcpy(&mdat.size, &box->p_next->i_shortsize, sizeof(int64_t));
    //printf("mdat Start offset %d end offset %d Size %d type %x \n", mdat.start_offset, mdat.start_offset + mdat.size, mdat.size, mdat.type); 

    MP4_BoxFree( root );
    stream_close(sb);
    destory_file_stream(sb);

    sb = create_buf_file_stream();
    if (stream_open(sb, "pitpull.mp4", MODE_READ) == 0)
      return -1;

	
    while (1) {
    //read moov buff 
 	len = buf_file_read(sb, buff, 1); 
	if(offset <= moov.start_offset + moov.size) {
		//buff_value = ((buff[0] & 0xff000000) >> 24 )  | ((buff[0] & 0x00ff0000) >> 8) | ((buff[0] & 0x0000ff00) << 8) | ((buff[0] & 0x000000ff) << 24);
		//printf("%x",buff[0]);

		// avcC type offset	
		if( buff[0] == 0x43 && sps_header.first_byte == 0x63 && sps_header.second_byte == 0x76 && sps_header.third_byte == 0x61 ) 
		{
			sps_header.offset = offset;
			//printf("sps_header 61 76 63 43 offset : %d \n",sps_header.offset);
		}	
		sps_header.third_byte = sps_header.second_byte;
		sps_header.second_byte = sps_header.first_byte;
		sps_header.first_byte = buff[0];
		if(sps_header.offset != 0) 
		{
			if(offset == sps_header.offset + 5) 
			{
				buff[0] = 0;
			} 
			if(offset == sps_header.offset + 6) 
			{
				buff[0] = 0;
			} 
			if(offset == sps_header.offset + 7) 
			{
				sps_header.data_len =  buff[0] << 8;
				buff[0] = 0;
			} 
			if(offset == sps_header.offset + 8 )
			{ 	
				sps_header.data_len += buff[0];
				buff[0] = 1;
				//printf("%d\n",sps_header.data_len);
			}
			if(offset >= ( sps_header.offset + 5 ) && offset <= ( sps_header.offset + sps_header.data_len + 8 ))
			{
				printf("%x ",buff[0]);
			}	

	
			if(offset == ( sps_header.offset + sps_header.data_len + 9 ))
			{
				buff[0] = 0;
			}
			if(offset == ( sps_header.offset + sps_header.data_len + 10 ) )
			{
				pps_header.data_len =  buff[0] << 8;
				buff[0] = 0;
			}
			if(offset == ( sps_header.offset + sps_header.data_len + 11 ) )
			{
				pps_header.data_len += buff[0];
				buff[0] = 0;
				//printf("%d\n",pps_header.data_len);
			}
			if(offset >= ( sps_header.offset + sps_header.data_len + 9 ) && offset <= ( sps_header.offset + sps_header.data_len + 12 + pps_header.data_len)) 
			{	
				if(offset == ( sps_header.offset + sps_header.data_len + 12 ))
				{
					tmp_buff[0] = 1;			
					printf("%x ",tmp_buff[0]);

				} else if(offset >= ( sps_header.offset + sps_header.data_len + 12 ) && offset <= ( sps_header.offset + sps_header.data_len + 12 + pps_header.data_len))
					printf("%x ",pre_buff[0]);
				else 
					printf("%x ",pre_buff[0]);
				pre_buff[0] = buff[0];
			}
		}	
	}	
	
	/*
	//mdat to naul format
	if(offset == (frame_size + mdat.start_offset + mdat_len_header + next_frame_size)) 
	{ 
		mdat_frame_size = mdat_frame_len_byte; 
	}
	if(offset >= mdat.start_offset + mdat_len_header && offset <= mdat.start_offset + mdat.size) 
	{ 	
		if(mdat_frame_size >= 0) 
		{
			frame_size = frame_size | (buff[0] << (mdat_frame_size * shift));
			buff[0] = 0;
			if(mdat_frame_size == 0) 
			{
				buff[0] = 1;
				next_frame_size = next_frame_size + frame_size + header_len;
				frame_size = 0;
				//printf("%d\n",count);
			} 
			mdat_frame_size--;
		} 
		printf("%x ",buff[0]);
	}	
	*/
	if(len == 0)
		break;
	offset++;
					
   }

   stream_close(sb);
   destory_file_stream(sb);

   return 0;
}
