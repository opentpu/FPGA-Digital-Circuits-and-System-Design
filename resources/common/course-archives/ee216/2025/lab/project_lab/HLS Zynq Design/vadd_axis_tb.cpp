#include "vadd_axis.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main() {
	stream<ap_axiu<64, 0, 0, 0>> stream_in;
	stream<ap_axis<32, 0, 0, 0>> stream_out;
	ap_axiu<64, 0, 0, 0> axis_in;
	ap_axis<32, 0, 0, 0> axis_out;
	ap_uint<64> data_in;
	ap_int<32> data_out;

	int N = 1024;
	int ref[N];
	for (int i = 0; i < N; i++) {
		ap_int<32> a = rand() % 256;
		ap_int<32> b = rand() % 256;
		data_in.range(63, 32) = a.range();
		data_in.range(31, 0) = b.range();
		ref[i] = a + b;
		axis_in.data = data_in;
		if (i == N - 1) {
			axis_in.last = 1;
		} else {
			axis_in.last = 0;
		}
		stream_in.write(axis_in);
	}

	vadd_axis(stream_in, stream_out, N);

	int last = 0;
	int i = 0;
	while (last == 0) {
		axis_out = stream_out.read();
		data_out = axis_out.data;
		last = axis_out.last;
		if (data_out != ref[i]) {
			cout << "fail" << endl;
			break;
		}
		i++;
	}
	cout << "pass" << endl;

	return 0;
}
