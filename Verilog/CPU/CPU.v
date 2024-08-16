module or3in1 (
    input wire A,
    input wire B,
    input wire C,
    output wire Y
);

assign Y = A | B | C;

endmodule

module CPU
(
	//input clk,
	input wire [15:0] inM,
	input wire [15:0] instruction,
	input wire reset,
	
	output wire [15:0] outM,
	output wire writeM,
	output wire [14:0] adressM,
	output wire [14:0] pc
);

wire [15:0] aluOut, aRegIn, aRegOut, xIn, yIn, PCout;
wire jump, loadPC;

//Если в op code==0, то это А-инструкция, ее загружжаем в следующий за
//мультиплексором регистр А, если же 1, то это С-инструкция, берем команду 
//выхода АЛУ
mux2in1_16 instrMux(.a(instruction), .b(aluOut), .sel(instruction[15]), .y(aRegIn));

//Проверяем, что op code нулевой (это условие записи в регистр А) и 
//что выбрана команда С и при этом dest указывает любую из команд на 
//сохранение в регистр А и его варианты

assign instrType = instruction[15];
assign isAcommand = ~instrType;
assign saveToRegA = instruction[5];
assign loadInAreg = isAcommand | saveToRegA;

//Регистр А на выходе выдает стразу и значение в себе (идет на вход АЛУ), 
//и 15-разрядное число адреса регистра М

register Aregister(

	.clk(),
	.load(loadInAreg),
	.in(aRegIn),
	.out(aRegOut)
);

assign adressM = aRegOut[14:0];

//В зависимости от поля "а" С-команды, на Y-вход АЛУ подается,
//либо регистр А (а == 0), либо регистр М (а == 1)

assign a = instruction[12];
mux2in1_16 AorMMux(.a(aRegOut), .b(inM), .sel(a), .y(yIn));

//В регистр D записываются данные только, если d[2] (index 4) равен 1
//и это С-команда:

assign saveToRegD = instruction[4];
assign loadD = instrType & saveToRegD;

register Dregister(

	.clk(),
	.load(loadD),
	.in(aluOut),
	.out(xIn)
);

//Если d[3] = 1, то это означент, что надо записать данные в М.
//При этом проверить, что выполняется С-команда:

assign saveToM = instruction[3];
assign writeM = instrType & saveToM;

ALU ALU_Hack(
	.x(xIn),
	.y(yIn),
	.zx(instruction[11]), 
	.zy(instruction[9]), 
	.nx(instruction[10]), 
	.ny(instruction[8]), 
	.f(instruction[7]), 
	.no(instruction[6]),
	.out(aluOut),
	.zr(zr),
	.ng(ng)
);

assign outM = aluOut;

//Это сложная функция J, суть котрой - логика работы РС

assign notZero = ~zr;
assign notNegative = ~ng;
assign isPositive = notZero & notNegative;

//Flags to indicate how to jump
assign JGT = instruction[0] & isPositive;
assign JEQ = instruction[1] & zr;
assign JLT = instruction[2] & ng;


or3in1 jumpers(
	.A(JGT),
	.B(JEQ),
	.C(JLT),
	.Y(jump)
);

assign loadPC = jump & instrType;

PC PC_Hack(

	.in(aRegOut), 
	.load(loadPC), 
	.inc(1'b1), 
	.reset(reset), 
	.out(PCout)
);

assign pc = PCout[14:0];

endmodule
