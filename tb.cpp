#include "tb.h"

void tb::source(void) 
{
	sc_int<16> tmp; 

	// Reset
	inp.write(0); 
	inp_vld.write(0); 
	rst.write(1); 
	wait(); 

	rst.write(0); 
	wait(); 

	for (int i = 0; i < 64; i++) {
		tmp = (i > 23 && i < 29)? 256 : 0;
		
		inp_vld.write(1);
		inp.write(tmp); 
		start_time[i] = sc_time_stamp(); 
		do {
			wait();
		} while ( !inp_rdy.read() ); 
		inp_vld.write(0);  
		
	}

	wait(10000); 
	printf("Hangling simulation"); 
	
	sc_stop(); 
}

void tb::sink() {
	sc_int<16> indata; 

	// extract clock period 
	sc_clock *clk_p = dynamic_cast<sc_clock*>(clk.get_interface()); 
	clock_period = clk_p->period(); 
	
	// Open simulation output results
	char output_file[256]; 
	sprintf(output_file, "./output.dat"); 
	outfp = fopen(output_file, "wb"); 
	if (outfp == NULL) 
	{
		printf("Couldn't open output.dat for writing \n"); 
		exit(0); 
	}

	// initialize
	outp_rdy.write(0); 
	
	double total_cycles = 0; 

	// read data
	for (int i = 0; i < 64; i++) {
		outp_rdy.write(1); 
		do {
			wait();
		} while ( !outp_vld.read() ); 
		
		indata = outp.read();
		end_time[i] = sc_time_stamp(); 
		total_cycles += (end_time[i] - start_time[i])/clock_period; 
		outp_rdy.write(0);

		fprintf(outfp, "%g\n", indata.to_double() ); 	
		cout << i << " :\t" << indata.to_double() << endl; 
	}

	double total_throughput = (start_time[63] - start_time[0])/clock_period; 

	printf("Average latency is %g cycles.\n", (double)(total_cycles/64));
	printf("Average throughput is %g cycles per input.\n", (double)(total_throughput/63));
	
	fclose(outfp); 
	sc_stop(); 
}
