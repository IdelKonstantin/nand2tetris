module inc_16
(
    input [15:0] a,
    output [15:0] y
);

add_16(.a(a), .b(16'b0000_0000_0000_0001), .y(y));

endmodule