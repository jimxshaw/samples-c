// Testbench for decoder_hex_7_segment.v
module tb_decoder_hex_7_segment;

    // Declare signals.
    reg  [3:0] hex_in;       // reg = we drive this.
    wire [6:0] segment_out;  // wire = DUT drives this back.

    // Expected values for all 16 inputs (for PASS/FAIL check).
    reg [6:0] expected [0:15];

    // Instantiate the DUT.
    decoder_hex_7_segment dut (
        .hex_in     (hex_in),
        .segment_out(segment_out)
    );

    integer i;
    integer pass_count, fail_count;

    initial begin
        // Load the expected segment pattern for each hex digit.
        expected[0]  = 7'b1111110; // 0
        expected[1]  = 7'b0110000; // 1
        expected[2]  = 7'b1101101; // 2
        expected[3]  = 7'b1111001; // 3
        expected[4]  = 7'b0110011; // 4
        expected[5]  = 7'b1011011; // 5
        expected[6]  = 7'b1011111; // 6
        expected[7]  = 7'b1110000; // 7
        expected[8]  = 7'b1111111; // 8
        expected[9]  = 7'b1111011; // 9
        expected[10] = 7'b1110111; // A
        expected[11] = 7'b0011111; // B
        expected[12] = 7'b1001110; // C
        expected[13] = 7'b0111101; // D
        expected[14] = 7'b1001111; // E
        expected[15] = 7'b1000111; // F

        pass_count = 0;
        fail_count = 0;

        // Header — {a,b,c,d,e,f,g} matches my segment order.
        $display("Input | Got {a,b,c,d,e,f,g} | Expected {a,b,c,d,e,f,g} | Result");
        $display("------|---------------------|--------------------------|-------");

        // Loop through all 16 hex values.
        for (i = 0; i <= 15; i = i + 1) begin
            hex_in = i[3:0]; // Assign input.
            #10;             // Wait for output to settle.

            if (segment_out === expected[i]) begin
                $display("  %H   |      %b      |         %b         | PASS",
                    hex_in, segment_out, expected[i]);
                pass_count = pass_count + 1;
            end else begin
                $display("  %H   |      %b      |         %b         | FAIL <<<",
                    hex_in, segment_out, expected[i]);
                fail_count = fail_count + 1;
            end
        end

        // ── 5. Summary ───────────────────────────────────────────────
        $display("------|---------------------|--------------------------|-------");
        $display("Result: %0d PASS, %0d FAIL out of 16 tests.", pass_count, fail_count);

        $finish;
    end

endmodule