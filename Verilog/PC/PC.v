module PC 
(
	input wire [15:0] in,
	input wire reset, load, inc, clk,
	output wire [15:0] out
);

wire [15:0] prevOut, incOut, incOrPrev, loadOrIncOrPrev, resetOrLoadOrIncOrPrev;

inc_16(.a(prevOut), .y(incOut));
mux2in1_16(.a(prevOut), .b(incOut), .sel(inc), .y(incOrPrev));
mux2in1_16(.a(incOrPrev), .b(in), .sel(load), .y(loadOrIncOrPrev));
mux2in1_16(.a(loadOrIncOrPrev), .b(16'b0), .sel(reset), .y(resetOrLoadOrIncOrPrev));

register(.clk(clk), .load(1), .in(resetOrLoadOrIncOrPrev), .out(prevOut));

assign out = prevOut;

endmodule