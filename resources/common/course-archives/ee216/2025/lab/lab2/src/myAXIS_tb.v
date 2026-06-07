module myAXIS_tb ();

reg ACLK;
reg ARESERTn;
reg S_AXIS_TVALID;
reg S_AXIS_TLAST;
reg [31:0] S_AXIS_TDATA;
reg  M_AXIS_TREADY;
wire S_AXIS_TREADY;
wire M_AXIS_TVALID;
wire M_AXIS_TLAST;
wire [31:0] M_AXIS_TDATA;

myAXIS myAXIS(
    .ACLK(ACLK),
    .ARESERTn(ARESERTn),
    .S_AXIS_TVALID(S_AXIS_TVALID),
    .S_AXIS_TLAST(S_AXIS_TLAST),
    .M_AXIS_TREADY(M_AXIS_TREADY),
    .S_AXIS_TREADY(S_AXIS_TREADY),
    .M_AXIS_TVALID(M_AXIS_TVALID),
    .M_AXIS_TLAST(M_AXIS_TLAST),
    .S_AXIS_TDATA(S_AXIS_TDATA),
    .M_AXIS_TDATA(M_AXIS_TDATA)
);

always #5 ACLK=~ACLK;

initial begin
    ACLK <= 1;
    ARESERTn <= 0;
    S_AXIS_TVALID <= 0;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 0;
    M_AXIS_TREADY <= 0;
    #10;
    ARESERTn <= 1;
    S_AXIS_TVALID <= 0;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 0;
    M_AXIS_TREADY <= 1;
    #20;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 1;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 2;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 3;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 0;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= -1;
    M_AXIS_TREADY <= 1;
    #20;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 4;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 5;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 6;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 7;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 1;
    S_AXIS_TLAST <= 1;
    S_AXIS_TDATA <= 8;
    M_AXIS_TREADY <= 1;
    #10;
    S_AXIS_TVALID <= 0;
    S_AXIS_TLAST <= 0;
    S_AXIS_TDATA <= 0;
    M_AXIS_TREADY <= 0;
    #100;
end

endmodule