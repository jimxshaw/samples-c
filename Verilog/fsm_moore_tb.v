// Testbench for fsm_moore.
module tb_fsm_moore;

    // Declare signals.
    reg clk, reset, A, B;       // reg = we drive these.
    wire Q;                     // wire = DUT drives this back.

    // Instantiate the DUT
    fsm_moore dut (
        .clk   (clk),
        .reset (reset),
        .A     (A),
        .B     (B),
        .Q     (Q)
    );

    // Clock generator.
    // Toggles every 5 units → full period = 10 units.
    // This runs forever in the background via 'always'.
    initial clk = 0;
    always #5 clk = ~clk;

    // Helper task for applying one clock cycle.
    // A 'task' is like a function — reusable and keeps things clean.
    // We set A and B BEFORE the rising edge so the DUT samples
    // stable inputs, then we wait just after the edge to check Q.
    task apply_and_check;
        input      a_in, b_in;      // inputs to apply this cycle.
        input      expected_Q;      // what Q should be AFTER the edge.
        input [63:0] test_num;      // just for display labeling.
        input [8*20:1] scenario;    // scenario label string.

        begin
            // Set inputs before the rising clock edge.
            A = a_in;
            B = b_in;

            // Wait for the rising clock edge (state registers update).
            @(posedge clk);

            // Wait a tiny bit after the edge for outputs to settle.
            #1;

            // Check and display result.
            if (Q === expected_Q)
                $display("Test %0d | A=%b B=%b | Q=%b (expected %b) | PASS | %0s",
                    test_num, A, B, Q, expected_Q, scenario);
            else
                $display("Test %0d | A=%b B=%b | Q=%b (expected %b) | FAIL <<< | %0s",
                    test_num, A, B, Q, expected_Q, scenario);
        end
    endtask

    // Stimulus.
    initial begin
        // Initialize all inputs.
        A = 0; B = 0; reset = 1;

        $display("=== FSM Moore Testbench ===");
        $display("State path: S0 -[A]-> S1 -[B]-> S2 -[always]-> S0");
        $display("Q output  : S0=0, S1=0, S2=1");
        $display("------------------------------------------------------------------");
        $display("Test # | Inputs   | Got → Expected | Result | Scenario");
        $display("------------------------------------------------------------------");

        // Apply reset for 2 cycles, then release.
        @(posedge clk); #1;
        @(posedge clk); #1;
        reset = 0;

        // Scenario A: Happy path.
        // S0 -[A=1]-> S1: Q should still be 0 (S1 output).
        apply_and_check(1, 0, 0, 1, "S0->S1: A=1, Q stays 0");

        // S1 -[B=1]-> S2: Q should go HIGH (S2 output).
        apply_and_check(0, 1, 1, 2, "S1->S2: B=1, Q goes HIGH");

        // S2 -[always]-> S0: Q should go LOW again (S0 output).
        apply_and_check(0, 0, 0, 3, "S2->S0: auto-return, Q goes LOW");

        // Scenario B: A=1, B=0 path (never reaches S2).
        // S0 -[A=1]-> S1.
        apply_and_check(1, 0, 0, 4, "S0->S1: A=1");

        // S1 -[B=0]-> S0: B=0 kicks us back, Q stays 0.
        apply_and_check(0, 0, 0, 5, "S1->S0: B=0, kicked back, Q stays 0");

        // Scenario C: A=0, stay in S0.
        apply_and_check(0, 0, 0, 6, "S0->S0: A=0, stay, Q=0");

        // Scenario D: Full A→B sequence twice in a row.
        apply_and_check(1, 0, 0, 7, "Round 2: S0->S1");
        apply_and_check(0, 1, 1, 8, "Round 2: S1->S2, Q HIGH again");
        apply_and_check(0, 0, 0, 9, "Round 2: S2->S0, Q LOW again");

        // Scenario E: Reset mid-operation.
        // Get to S1 first...
        A = 1; B = 0;
        @(posedge clk); #1;

        // Now slam reset while in S1.
        reset = 1;
        @(posedge clk); #1;

        if (Q === 0)
            $display("Test 10 | RESET asserted mid-S1 | Q=%b (expected 0) | PASS | Reset overrides state", Q);
        else
            $display("Test 10 | RESET asserted mid-S1 | Q=%b (expected 0) | FAIL <<< | Reset overrides state", Q);

        reset = 0;

        $display("------------------------------------------------------------------");
        $finish;
    end

endmodule