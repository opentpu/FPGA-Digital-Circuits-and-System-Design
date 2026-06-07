#include "hls_stream.h"
#include "ap_axi_sdata.h"
#define MAX_SIZE 1024

using namespace hls;

int vadd_axis(
	stream<ap_axiu<64, 0, 0, 0>>& stream_in,
	stream<ap_axis<32, 0, 0, 0>>& stream_out,
	int size
);
