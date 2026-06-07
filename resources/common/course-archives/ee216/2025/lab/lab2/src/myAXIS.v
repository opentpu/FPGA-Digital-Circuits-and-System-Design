module myAXIS (
    input ACLK,
    input ARESERTn,
    input S_AXIS_TVALID,
    output S_AXIS_TREADY,
    input S_AXIS_TLAST,
    input [31:0] S_AXIS_TDATA,
    output M_AXIS_TVALID,
    input M_AXIS_TREADY,
    output M_AXIS_TLAST,
    output [31:0] M_AXIS_TDATA
);

reg [31:0] maxis_tdata;
reg maxis_tvalid, maxis_tlast, saxis_tready;
assign M_AXIS_TDATA = maxis_tdata;
assign M_AXIS_TVALID = maxis_tvalid;
assign M_AXIS_TLAST = maxis_tlast;
assign S_AXIS_TREADY = saxis_tready;

reg tlast_delay;
always@(posedge ACLK) begin
    if (~ARESERTn) begin
        maxis_tvalid <= 0;
        saxis_tready <= 0;
        maxis_tdata <= 0;
        maxis_tlast <= 0;
        tlast_delay <= 0;
    end
    else begin
        saxis_tready <= 1;
        tlast_delay <= S_AXIS_TLAST;
        if (S_AXIS_TVALID && (~tlast_delay)) begin
            maxis_tvalid <= 1;
            maxis_tdata <= S_AXIS_TDATA * 2;
        end
        else begin
            maxis_tvalid <= 0;
            maxis_tdata <= -1;
        end
        if (S_AXIS_TLAST) begin
            maxis_tlast <= 1;
        end
        else begin
            maxis_tlast <= 0;
        end
    end
end
    
endmodule