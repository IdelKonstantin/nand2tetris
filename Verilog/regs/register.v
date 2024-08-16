module register (

    input wire clk,
    input wire load,
    input wire [15:0] in,
    output reg [15:0] out
);

    always @(posedge clk) begin
        if (load) begin
            out <= in;
        end
    end

endmodule