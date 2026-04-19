// Testbench for sr_latch_nor
module tb_sr_latch_nor;

    // Declare signals to drive/observe.
    reg  S, R;          // reg = we'll drive these ourselves.
    wire Q, Q_Not;      // wire = outputs coming FROM the DUT.

    // Instantiate the Device Under Test (DUT).
    sr_latch_nor dut (
        .S     (S),
        .R     (R),
        .Q     (Q),
        .Q_Not (Q_Not)
    );

    // Apply stimulus and display results.
    initial begin
        // Print a header (like the "a -> y" header in Image 2)
        $display("  S  R  |  Q  Q_Not");
        $display("---------------------");

        // --- Test 1: RESET (R=1, S=0) → Q should go LOW ---
        S = 0; R = 1; #10;
        $display("  %b  %b  |  %b    %b      <- Reset", S, R, Q, Q_Not);

        // --- Test 2: HOLD (R=0, S=0) → Q should stay LOW ---
        S = 0; R = 0; #10;
        $display("  %b  %b  |  %b    %b      <- Hold (stay reset)", S, R, Q, Q_Not);

        // --- Test 3: SET (S=1, R=0) → Q should go HIGH ---
        S = 1; R = 0; #10;
        $display("  %b  %b  |  %b    %b      <- Set", S, R, Q, Q_Not);

        // --- Test 4: HOLD (R=0, S=0) → Q should stay HIGH ---
        S = 0; R = 0; #10;
        $display("  %b  %b  |  %b    %b      <- Hold (stay set)", S, R, Q, Q_Not);

        // --- Test 5: FORBIDDEN (S=1, R=1) → undefined behavior ---
        S = 1; R = 1; #10;
        $display("  %b  %b  |  %b    %b      <- Forbidden state!", S, R, Q, Q_Not);

        $finish; // End simulation (like the $finish you see in both images)
    end

endmodule