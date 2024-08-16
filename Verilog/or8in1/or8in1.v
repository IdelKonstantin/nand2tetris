module or8in1 (
    input [7:0] a,
    output y
);

    assign y = |a;

endmodule