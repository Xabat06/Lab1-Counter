module counter #(
    parameter WIDTH = 8
)(
    // interface signals
    input logic clk, // clock
    input logic rst, // reset
    input logic en, // counter enable
    output logic [WIDTH-1:0] count // count output
);

always_ff @ (posedge clk or posedge rst)  // asynchronous reset
    if (rst) count <= {WIDTH{1'b0}};
    else begin 
        if (en == 1) count <= count + {{WIDTH-1{1'b0}}, en};
        else count <= count + 8'b11111111; // -1
    end

endmodule
