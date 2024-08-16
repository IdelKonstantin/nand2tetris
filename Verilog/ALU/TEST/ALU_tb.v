module add_16 (
    input [15:0] a,
   input [15:0] b,
    output [15:0] y
);

assign y = a + b;
   
endmodule

module and_16 (
    input [15:0] a,
   input [15:0] b,
    output [15:0] y
);

assign y = a & b;
   
endmodule

module not_16 (
    input [15:0] a,
    output [15:0] y
);

assign y = ~a;
   
endmodule

module mux2in1_16 (

    input [15:0] a,
    input [15:0] b,
    input sel,
    output [15:0] y
);

assign y = (sel) ? b : a;

endmodule

module or8in1 (
    input [7:0] a,
    output y
);

assign y = |a;

endmodule

module ALU 
(
  input [15:0] x,
  input [15:0] y,
  input zx, zy, nx, ny, f, no,
  output [15:0] out,
  output zr,
  output ng
);

wire [15:0] zxMuxOut, zyMuxOut;
wire [15:0] notZxMuxOut, notZyMuxOut;
wire [15:0] nxMuxOut, nyMuxOut;
wire [15:0] addOut, andOut;
wire [15:0] fMuxOut, noFMuxOut;

wire highOr, lowOr;

mux2in1_16 zxMux(.a(x), .b(16'b0), .sel(zx), .y(zxMuxOut));
not_16 notzxMux(.a(zxMuxOut), .y(notZxMuxOut));
mux2in1_16 nxMux(.a(zxMuxOut), .b(notZxMuxOut), .sel(nx), .y(nxMuxOut));


mux2in1_16 zyMux(.a(y), .b(16'b0), .sel(zy), .y(zyMuxOut));
not_16 notZyMux(.a(zyMuxOut), .y(notZyMuxOut));
mux2in1_16 nyMux(.a(zyMuxOut), .b(notZyMuxOut), .sel(ny), .y(nyMuxOut));

add_16 assNxNy(.a(nxMuxOut), .b(nyMuxOut), .y(addOut));
and_16 andNxNy(.a(nxMuxOut), .b(nyMuxOut), .y(andOut));

mux2in1_16 fMux(.a(andOut), .b(addOut), .sel(f), .y(fMuxOut));
not_16 notFMux(.a(fMuxOut), .y(noFMuxOut));

mux2in1_16 noMux(.a(fMuxOut), .b(noFMuxOut), .sel(no), .y(out));

assign ng = out[15];

or8in1 orHighBits(.a(out[7:0]), .y(highOr));
or8in1 orLowBits(.a(out[15:8]), .y(lowOr));

assign anyBitsTrue = highOr | lowOr;
assign zr = ~anyBitsTrue;

endmodule

module ALU_tb();

  integer file;

  reg [15:0] x;
  reg [15:0] y;
  reg zx;
  reg nx;
  reg zy;
  reg ny;
  reg f;
  reg no;
  wire [15:0] out;
  wire zr;
  wire ng;

  ALU ALU(
        .x(x),
        .y(y),
        .zx(zx),
        .nx(nx),
        .zy(zy),
        .ny(ny),
        .f(f),
        .no(no),
        .out(out),
        .zr(zr),
        .ng(ng)
      );

  task display;
    #1 $fwrite(file,
               "| %16b | %16b | %1b | %1b | %1b | %1b | %1b | %1b | %16b | %1b | %1b |\n",
               x,
               y,
               zx,
               nx,
               zy,
               ny,
               f,
               no,
               out,
               zr,
               ng);
  endtask

  initial
    begin
      $dumpfile("ALU_tb.vcd");
      $dumpvars(0, ALU_tb);
      file = $fopen("ALU.out","w");
      $fwrite(file, "|        x         |        y         |zx |nx |zy |ny | f |no |       out        |zr |ng |\n");

      x = 16'b0000000000000000;  // x ==0
      y = 16'b1111111111111111;  // y = -1

      // Compute=0
      zx = 1;
      nx =0;
      zy=1;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute 1
      zx=1;
      nx=1;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute -1
      zx=1;
      nx=1;
      zy=1;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =0;
      no=0;
      display();

      // Compute y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =0;
      no=0;
      display();

      // Compute !x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =0;
      no=1;
      display();

      // Compute !y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =0;
      no=1;
      display();

      // Compute -x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute -y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=1;
      display();

      // Compute x + 1
      zx=0;
      nx=1;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute y + 1
      zx=1;
      nx=1;
      zy=0;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute x - 1
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =1;
      no=0;
      display();

      // Compute y - 1
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x + y
      zx=0;
      nx=0;
      zy=0;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x - y
      zx=0;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=1;
      display();

      // Compute y - x
      zx=0;
      nx=0;
      zy=0;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute x & y
      zx=0;
      nx=0;
      zy=0;
      ny=0;
      f =0;
      no=0;
      display();

      // Compute x | y
      zx=0;
      nx=1;
      zy=0;
      ny=1;
      f =0;
      no=1;
      display();

      x=16'b000000000010001;  // x = 17
      y=16'b000000000000011;  // y =  3

      // Compute=0
      zx=1;
      nx=0;
      zy=1;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute 1
      zx=1;
      nx=1;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute -1
      zx=1;
      nx=1;
      zy=1;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =0;
      no=0;
      display();

      // Compute y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =0;
      no=0;
      display();

      // Compute !x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =0;
      no=1;
      display();

      // Compute !y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =0;
      no=1;
      display();

      // Compute -x
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute -y
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=1;
      display();

      // Compute x + 1
      zx=0;
      nx=1;
      zy=1;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute y + 1
      zx=1;
      nx=1;
      zy=0;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute x - 1
      zx=0;
      nx=0;
      zy=1;
      ny=1;
      f =1;
      no=0;
      display();

      // Compute y - 1
      zx=1;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x + y
      zx=0;
      nx=0;
      zy=0;
      ny=0;
      f =1;
      no=0;
      display();

      // Compute x - y
      zx=0;
      nx=1;
      zy=0;
      ny=0;
      f =1;
      no=1;
      display();

      // Compute y - x
      zx=0;
      nx=0;
      zy=0;
      ny=1;
      f =1;
      no=1;
      display();

      // Compute x & y
      zx=0;
      nx=0;
      zy=0;
      ny=0;
      f =0;
      no=0;
      display();

      // Compute x | y
      zx=0;
      nx=1;
      zy=0;
      ny=1;
      f =0;
      no=1;
      display();
      $finish;
    end

endmodule