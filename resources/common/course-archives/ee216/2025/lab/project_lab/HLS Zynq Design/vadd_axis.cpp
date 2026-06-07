#include "vadd_axis.h"

int vadd_axis(
		stream<ap_axiu<64, 0, 0, 0>>& stream_in,
		stream<ap_axis<32, 0, 0, 0>>& stream_out,
		int size
) {
#pragma HLS INTERFACE axis port=stream_in depth=1024
#pragma HLS INTERFACE axis port=stream_out depth=1024
#pragma HLS INTERFACE s_axilite port=size
#pragma HLS INTERFACE s_axilite port=return
	ap_int<32> a, b;
	ap_axiu<64, 0, 0, 0> axis_in;
	ap_axis<32, 0, 0, 0> axis_out;
	ap_uint<64> data_in;
	ap_int<32> data_out;
	for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT max=1024 min=1024
#pragma HLS PIPELINE
		axis_in = stream_in.read();
		data_in = axis_in.data;
		a.range(31, 0) = data_in.range(31, 0);	// low 32 bits
		b.range(31, 0) = data_in.range(63, 32);	// high 32 bits
		data_out = a + b;
		axis_out.data = data_out;
		axis_out.keep = -1;
		if (i == size - 1) {
			axis_out.last = 1;
		} else {
			axis_out.last = 0;
		}
		stream_out.write(axis_out);
	}
	return 0;
}
