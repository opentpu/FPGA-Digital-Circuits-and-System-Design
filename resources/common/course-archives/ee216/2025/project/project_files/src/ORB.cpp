#include "ORB.h"

static int idxTree[256][100] = {0};
static int cntTree[256] = {0};

ap_fixed<ANGLE_WIDTH, 3> IC_Angle_31(xf::cv::Window<WIND_SIZE, WIND_SIZE, ap_uint<8>>& window){
    ap_int<M_WIDTH> m_01, m_10, val_plus, val_minus, v_sum;
    ap_fixed<ANGLE_WIDTH, 3> ret;
    int umax[16] = {15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 9, 8, 6, 3};
    m_01 = 0;
    m_10 = 0;
    for (int u = -15; u <= 15; ++u) {
        m_10 += u * window(15, 15 + u);
    }
    for (int v = 1; v <= 15; ++v) {
        v_sum = 0;
        int d = umax[v];
        for (int u = -d; u <= d; ++u) {
            val_plus = window(15+v, 15+u);
            val_minus = window(15-v, 15+u);
            v_sum += (val_plus - val_minus);
            m_10 += u * (val_plus + val_minus);
        }
        m_01 += v * v_sum;
    }
    ap_fixed<M_WIDTH, M_WIDTH> m_01_fixed = m_01;
    ap_fixed<M_WIDTH, M_WIDTH> m_10_fixed = m_10;
    ret = hls::atan2<M_WIDTH, M_WIDTH>(m_01_fixed, m_10_fixed);
    return ret;
}

ap_uint<256> descriptor(xf::cv::Window<WIND_SIZE, WIND_SIZE, ap_uint<8>>& window, ap_fixed<ANGLE_WIDTH, 3> angle) {
    ap_uint<8> t0, t1;
    ap_uint<256> desc;
    ap_int<6> x0, x1, y0, y1, x0_rotated, y0_rotated, x1_rotated, y1_rotated;
    ap_fixed<ANGLE_WIDTH-3+2, 2> cosine, sine;
    cosine = hls::cos<ANGLE_WIDTH, 3>(angle);
    sine = hls::sin<ANGLE_WIDTH, 3>(angle);

    loop_descriptor:
    for (int i = 0; i < 256; i++) {
        x0 = bit_pattern_31[i*4];
        y0 = bit_pattern_31[i*4 + 1];
        x1 = bit_pattern_31[i*4 + 2];
        y1 = bit_pattern_31[i*4 + 3];
        x0_rotated = hls::round(x0*cosine-y0*sine);
        y0_rotated = hls::round(x0*sine+y0*cosine);
        x1_rotated = hls::round(x1*cosine-y1*sine);
        y1_rotated = hls::round(x1*sine+y1*cosine);
        t0 = window(HALF_WIND_SIZE + y0_rotated, HALF_WIND_SIZE + x0_rotated);
        t1 = window(HALF_WIND_SIZE + y1_rotated, HALF_WIND_SIZE + x1_rotated);
        desc.set_bit(i, (t0 < t1));
    }
    return desc;
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_input(
    hls::stream<ap_axiu<W, 1, 1, 1>>& src,
    hls::stream<ap_uint<W>>& dst,
    int height,
    int width
) {
    for (int i = 0; i < height*width; i++) {
        dst.write(src.read().data);
    }
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_stream2xfMat(
    hls::stream<ap_uint<W>>& src,
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& dst
) {
    for (int i = 0; i < dst.cols * dst.rows; i++) {
        dst.write(i, src.read());
    }
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_xfMat2stream(
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& src,
    hls::stream<ap_uint<W>>& dst
) {
    for (int i = 0; i < src.cols * src.rows; i++) {
        dst.write(src.read(i));
    }
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_resize(
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& src,
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC>& dst
) {
    xf::cv::resize<
        XF_INTERPOLATION_BILINEAR,
        TYPE,
        HEIGHT,
        WIDTH,
        HEIGHT,
        WIDTH,
        NPC,
        8
    >(src, dst);
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_blur(
    hls::stream<ap_uint<W>>& img_in,
    hls::stream<ap_uint<W>>& img_out,
    hls::stream<ap_uint<W>>& blur_out,
    int height, 
    int width
) {
    xf::cv::LineBuffer<7, WIDTH, ap_uint<8>> line_buffer;
    xf::cv::Window<7, 7, ap_uint<8>> window_buffer, window;
    ap_uint<W> pixel_rd, pixel_wr;
    ap_uint<16> acc;
    int read_idx = 0;
    int write_idx = 0;
    static ap_fixed<16, 2> kernel[7*7] = {
        0.00492233, 0.00919613, 0.01338028, 0.01516185, 0.01338028, 0.00919613, 0.00492233, 
        0.00919613, 0.01718062, 0.02499766, 0.02832606, 0.02499766, 0.01718062, 0.00919613, 
        0.01338028, 0.02499766, 0.03637138, 0.04121417, 0.03637138, 0.02499766, 0.01338028, 
        0.01516185, 0.02832606, 0.04121417, 0.04670178, 0.04121417, 0.02832606, 0.01516185, 
        0.01338028, 0.02499766, 0.03637138, 0.04121417, 0.03637138, 0.02499766, 0.01338028, 
        0.00919613, 0.01718062, 0.02499766, 0.02832606, 0.02499766, 0.01718062, 0.00919613, 
        0.00492233, 0.00919613, 0.01338028, 0.01516185, 0.01338028, 0.00919613, 0.00492233
    };

    loop_init_window_row:
    for (int i = 0; i < 7; i++) {
        loop_init_window_col:
        for (int j = 0; j < 7; j++) {
            window_buffer.insert_pixel(0, i, j);
            window.insert_pixel(0, i, j);
        }
    }
    
    loop_init_line:
    for (int j = 0 ; j < width; j++) {
        for (int i = 0; i < 7; i++) {
            line_buffer.val[i][j] = 0;
        } 
    }

    loop_blur:
    for (int cnt = 0; cnt < (height+4)*width; cnt++) {
        int i = cnt / width;
        int j = cnt % width;
        if (i < height) {
            pixel_rd = img_in.read();
            img_out.write(pixel_rd);
            read_idx++;
        } else {
            pixel_rd = 0;
        }
        line_buffer.shift_pixels_up(j);
        line_buffer.insert_bottom_row(pixel_rd, j);
        window_buffer.shift_pixels_left();
        window.shift_pixels_left();
        for (int k = 0; k < 7; k++) {
            window_buffer.insert_pixel(line_buffer(k, j), k, 6);
            if (j == 3) {
                for (int jj = 0; jj < 3; jj++) {
                    window.insert_pixel(0, k, jj);
                }
                for (int jj = 3; jj < 7; jj++) {
                    window.insert_pixel(window_buffer(k, jj), k, jj);
                }
            } else if (j > 3) {
                window.insert_pixel(line_buffer(k, j), k, 6);
            } else {
                window.insert_pixel(0, k, 6);
            }
        }
        if (read_idx > 3*width + 3 && write_idx < width*height) {
            acc = 0;
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 7; j++) {
                    acc += window(i, j)*kernel[i*7+j];
                }
            }
            pixel_wr = acc.range(W-1, 0);
            blur_out.write(pixel_wr);
            write_idx++;
        }
    }
}

template<int W, int HEIGHT, int WIDTH>
void process_rBRIEF(
    hls::stream<ap_uint<W>>& img_in,
    hls::stream<ap_uint<W>>& img_blur,
    hls::stream<ap_uint<W>>& mask_in,
    hls::stream<DescOut>& desc_out,
    int height,
    int width
) {
    ap_uint<W> img_pixel, blur_pixel, mask, response;

    xf::cv::LineBuffer<WIND_SIZE, WIDTH, ap_uint<W>> img_line, blur_line;
    xf::cv::Window<WIND_SIZE, WIND_SIZE, ap_uint<W>> img_window, blur_window;
    xf::cv::LineBuffer<WIND_SIZE, WIDTH, ap_uint<W>> mask_line;
    xf::cv::Window<WIND_SIZE, WIND_SIZE, ap_uint<W>> mask_window;

    ap_fixed<ANGLE_WIDTH, 3> angle;
    DescOut data_write;

    loop_row:
    for (int i = 0; i < height; i++) {
        loop_col:
        for (int j = 0; j < width; j++) {  
            mask = mask_in.read();
            img_pixel = img_in.read();
            blur_pixel = img_blur.read();

            img_line.shift_pixels_up(j);
            img_line.insert_bottom_row(img_pixel, j);
            blur_line.shift_pixels_up(j);
            blur_line.insert_bottom_row(blur_pixel, j);
            mask_line.shift_pixels_up(j);
            mask_line.insert_bottom_row(mask, j);

            img_window.shift_pixels_left();
            blur_window.shift_pixels_left();
            mask_window.shift_pixels_left();

            for (int k = 0; k < WIND_SIZE; k++) {
                img_window.insert_pixel(img_line(k, j), k, WIND_SIZE-1);
                blur_window.insert_pixel(blur_line(k, j), k, WIND_SIZE-1);
                mask_window.insert_pixel(mask_line(k, j), k, WIND_SIZE-1);
            }

            if (i >= WIND_SIZE-1 && j >= WIND_SIZE-1 && mask_window(HALF_WIND_SIZE, HALF_WIND_SIZE)) {
                data_write.x = j - HALF_WIND_SIZE;
                data_write.y = i - HALF_WIND_SIZE;
                data_write.valid = 1;
                data_write.response = mask_window(HALF_WIND_SIZE, 0);
                angle = IC_Angle_31(img_window);
                data_write.descriptor = descriptor(blur_window, angle);
                desc_out.write(data_write); 
            }
        }
    }
    data_write.valid = 0;
    desc_out.write(data_write);
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_output(
    hls::stream<DescOut>& src,
    hls::stream<ap_axiu<16*2+8+256, 1, 1, 1>>& dst,
    int& length
) {
    ap_axiu<16*2+8+256, 1, 1, 1> dst_axi;
    DescOut data;
    ap_uint<1> flag = 1;
    int cnt = 0;
    while (flag == 1) {
        data = src.read();
        flag = data.valid;
        if (flag == 1) {
            dst_axi.data.range(16*2+8+256-1, 16+8+256) = data.x;
            dst_axi.data.range(16+8+256-1, 8+256) = data.y;
            dst_axi.data.range(8+256-1, 256) = data.response;
            dst_axi.data.range(256-1, 0) = data.descriptor;
            dst_axi.last = 0;
            dst_axi.keep = -1;
            idxTree[data.response][cntTree[data.response]] = cnt;
            cntTree[data.response]++;
            cnt++;
            dst.write(dst_axi);
        }
    }
    length = cnt;
}

template<int W, int TYPE, int HEIGHT, int WIDTH, int NPC>
void process_ORB(
    hls::stream<ap_axiu<W, 1, 1, 1>>& src,
    hls::stream<ap_axiu<16*2+8+256, 1, 1, 1>>& dst,
    int height, 
    int width,
    int height_new,
    int width_new,
    int threshold,
    int& length
) {
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC> img_mat(height, width);
    xf::cv::Mat<TYPE, HEIGHT, WIDTH, NPC> resized_mat(height_new, width_new);
    hls::stream<ap_uint<W>> img, resized_img, img1, img2, img3, mask2, mask3, blur1, blur2, blur3;
    hls::stream<DescOut> desc;

    #pragma HLS stream variable=img depth=2
    #pragma HLS stream variable=resized_img depth=2
    #pragma HLS stream variable=img1 depth=4*WIDTH
    #pragma HLS stream variable=img2 depth=4*WIDTH
    #pragma HLS stream variable=img3 depth=4*WIDTH
    #pragma HLS stream variable=mask2 depth=2
    #pragma HLS stream variable=mask3 depth=2
    #pragma HLS stream variable=blur1 depth=2
    #pragma HLS stream variable=blur2 depth=4*WIDTH
    #pragma HLS stream variable=blur3 depth=4*WIDTH
    #pragma HLS stream variable=desc depth=2

    #pragma HLS DATAFLOW
    process_input<W, TYPE, HEIGHT, WIDTH, NPC>(src, img, height, width);
    process_stream2xfMat<W, TYPE, HEIGHT, WIDTH, NPC>(img, img_mat);
    process_resize<W, TYPE, HEIGHT, WIDTH, NPC>(img_mat, resized_mat);
    process_xfMat2stream<W, TYPE, HEIGHT, WIDTH, NPC>(resized_mat, resized_img);
    process_blur<W, TYPE, HEIGHT, WIDTH, NPC>(resized_img, img1, blur1, height_new, width_new);
    process_fast<W, HEIGHT, WIDTH, NPC>(img1, img2, blur1, blur2, mask2, height_new, width_new, threshold);
    process_nms<W, HEIGHT, WIDTH, NPC>(img2, img3, blur2, blur3, mask2, mask3, height_new, width_new);
    process_rBRIEF<W, HEIGHT, WIDTH>(img3, blur3, mask3, desc, height_new, width_new);
    process_output<W, TYPE, HEIGHT, WIDTH, NPC>(desc, dst, length);
}

int ORB_accel(
    hls::stream<ap_axiu<8, 1, 1, 1>>& src,
    hls::stream<ap_axiu<16*2+8+256, 1, 1, 1>>& dst,
    int height,
    int width,
    int height_new,
    int width_new,
    int threshold
) {
    #pragma HLS interface axis port=src depth=80*60
    #pragma HLS interface axis port=dst depth=80*60
    #pragma HLS interface s_axilite port=height
    #pragma HLS interface s_axilite port=width
    #pragma HLS interface s_axilite port=height_new
    #pragma HLS interface s_axilite port=width_new
    #pragma HLS interface s_axilite port=threshold
    #pragma HLS interface s_axilite port=return

    int nFeatures;

    process_ORB<8, XF_8UC1, 1080, 1920, XF_NPPC1>(
        src,
        dst,
        height,
        width,
        height_new,
        width_new,
        threshold,
        nFeatures
    );

    ap_axiu<16*2+8+256, 1, 1, 1> dst_axi;
    int cnt = 0;
    for (int val = 0; val < 256; val++) {
        for (int i = 0; i < cntTree[255-val]; i++) {
            if (cnt < nFeatures) {
                dst_axi.data = idxTree[255-val][i];
                dst_axi.keep = -1;
                if (cnt == nFeatures - 1) {
                    dst_axi.last = 1;
                } else {
                    dst_axi.last = 0;
                }
                dst.write(dst_axi);
                cnt++;
            }
        }
        cntTree[255-val] = 0;
    }

    return nFeatures;
}