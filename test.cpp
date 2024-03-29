#include <systemc.h>

SC_MODULE( and2 ) {
	sc_in< sc_uint<1> > a; 
	sc_in< sc_uint<1> > b; 
	sc_in<bool> clk; 

	sc_out< sc_uint<1> > f; 

	void func() {
		f.write( a.read() & b.read() ); 
	}

	SC_CTOR( and2 ) {
		SC_METHOD( func ); 
		sensitive << clk.pos(); 
	}
};
