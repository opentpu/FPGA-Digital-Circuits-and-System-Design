`timescale 1ns / 1ps

module myFIR(
    input clk,
    input rst_n,
    input S_AXIS_TVALID,
    input S_AXIS_TLAST,
    input [7:0] S_AXIS_TDATA,
    output S_AXIS_TREADY,
    input M_AXIS_TREADY,
    output M_AXIS_TVALID,
    output M_AXIS_TLAST,
    output [31:0] M_AXIS_TDATA
    
);
parameter signed [4:0] h0 = 5'd1;
parameter signed [4:0] h1 = -5'd2;
parameter signed [4:0] h2 = 5'd3;
parameter signed [4:0] h3 = -5'd4;
parameter signed [4:0] h4 = 5'd5;
parameter signed [4:0] h5 = -5'd6;
parameter signed [4:0] h6 = 5'd7;
parameter signed [4:0] h7 = -5'd8;

endmodule
