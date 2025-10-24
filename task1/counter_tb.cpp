#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;
    int pause_cycles = 3;
    int cycles_left = 0;
    int prev_count = 0;  // new variables for the delay

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vcounter* top = new Vcounter;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("counter.vcd");

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    //run simulation for many clock cycles
    for (i=0; i<300; i++) {

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++) {
            tfp->dump (2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }
        top->rst = (i < 2) | (i == 15);

        if (cycles_left > 0) { // keep en low on pause
            cycles_left -= 1;
            top->en = 0;
        }
        else {
            top->en = (i > 4);
        }

        if (top->count == 0x9 && prev_count != 0x9 && cycles_left == 0) {
            cycles_left = pause_cycles - 1; // because there is a 1 cycle delay
            top->en = 0;
        }
        prev_count = top->count; // to prevent count is not stuck at 0x9

        if (Verilated::gotFinish()) exit(0);
    }
    tfp->close();
    exit(0);
}