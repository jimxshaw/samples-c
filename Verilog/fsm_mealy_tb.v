// Testbench for fsm_mealy.
module tb_fsm_mealy;

    // Declare signals.
    reg  clk, reset, A, B;      // reg = we drive these.
    wire Q;                     // wire = DUT drives this back.

    // Instantiate the DUT.
    fsm_mealy dut (
        .clk   (clk),
        .reset (reset),
        .A     (A),
        .B     (B),
        .Q     (Q)
    );

    // Clock generator.
    initial clk = 0;
    always #5 clk = ~clk;

    // Pass/Fail counters.
    integer pass_count, fail_count;

    // Helper task.
    // For Mealy: we set inputs, wait for combinational propagation (#1),
    // THEN check Q — because Q reacts to inputs immediately, not just
    // on the clock edge like Moore did.
    // We then clock once to advance state for the next test.
    task apply_and_check;
        input      a_in, b_in;
        input      expected_Q;
        input [31:0] test_num;
        input [8*40:1] scenario;

        begin
            // Set inputs — Q will react combinationally right away.
            A = a_in;
            B = b_in;
            #1; // Let combinational logic settle (no clock needed for Q).

            if (Q === expected_Q) begin
                $display("Test %02d | State transition + A=%b B=%b | Q=%b (exp %b) | PASS | %0s",
                    test_num, A, B, Q, expected_Q, scenario);
                pass_count = pass_count + 1;
            end else begin
                $display("Test %02d | State transition + A=%b B=%b | Q=%b (exp %b) | FAIL <<< | %0s",
                    test_num, A, B, Q, expected_Q, scenario);
                fail_count = fail_count + 1;
            end

            // Now clock once to register the next_state into current_state.
            @(posedge clk); #1;
        end
    endtask

    // Stimulus.
    initial begin
        A = 0; B = 0; reset = 1;
        pass_count = 0; fail_count = 0;

        $display("======================================================");
        $display("          FSM Mealy Testbench                         ");
        $display("======================================================");
        $display("State machine:");
        $display("  S0: A=1 -> S1 (Q=0) | A=0 -> S0 (Q=0)");
        $display("  S1: B=1 -> S2 (Q=0) | B=0 -> S0 (Q=0)");
        $display("  S2: A&B -> S2 (Q=1) | else -> S0 (Q=0)  <- Q=1 ONLY here");
        $display("KEY: Q is combinational — reacts to inputs instantly.");
        $display("------------------------------------------------------");

        // Hold reset for 2 cycles then release.
        @(posedge clk); #1;
        @(posedge clk); #1;
        reset = 0;

        // Scenario A: Happy path to Q=1.
        // S0, A=1: next=S1, Q=0.
        apply_and_check(1, 0, 0, 1,  "S0: A=1 -> next=S1, Q=0");

        // S1, B=1: next=S2, Q=0.
        apply_and_check(0, 1, 0, 2,  "S1: B=1 -> next=S2, Q=0");

        // S2, A=1 & B=1: STAY S2, Q=1 (only place Q goes HIGH).
        apply_and_check(1, 1, 1, 3,  "S2: A&B=1 -> stay S2, Q=1 !!!");

        // S2 again, A=1 & B=1: confirm Q=1 holds while staying in S2.
        apply_and_check(1, 1, 1, 4,  "S2: A&B=1 -> stay S2, Q=1 again");

        // S2, A=0: drop out of S2 back to S0, Q goes LOW immediately.
        apply_and_check(0, 1, 0, 5,  "S2: A=0 -> next=S0, Q drops to 0");

        // Scenario B: B=0 in S1 kicks back to S0.
        apply_and_check(1, 0, 0, 6,  "S0: A=1 -> next=S1, Q=0");
        apply_and_check(0, 0, 0, 7,  "S1: B=0 -> kicked back to S0, Q=0");

        // Scenario C: Linger in S0 with A=0.
        apply_and_check(0, 0, 0, 8,  "S0: A=0 -> stay S0, Q=0");
        apply_and_check(0, 0, 0, 9,  "S0: A=0 -> stay S0, Q=0 again");

        // Scenario D: KEY Mealy test — input glitch in S2.
        // Get to S2 first via S0->S1->S2.
        apply_and_check(1, 0, 0, 10, "S0->S1: setup");
        apply_and_check(0, 1, 0, 11, "S1->S2: setup");

        // Now we're in S2. Pull B LOW mid-cycle WITHOUT clocking.
        // Q must drop to 0 IMMEDIATELY — this only happens in Mealy!
        A = 1; B = 1; #1; // Confirm Q=1 first.
        $display("  [Glitch test] In S2 with A=1,B=1: Q=%0b (should be 1)", Q);

        B = 0; #1; // Pull B low — no clock yet.
        if (Q === 0) begin
            $display("  [Glitch test] B dropped mid-cycle: Q=%0b (exp 0) | PASS | Mealy reacts instantly!", Q);
            pass_count = pass_count + 1;
        end else begin
            $display("  [Glitch test] B dropped mid-cycle: Q=%0b (exp 0) | FAIL <<< | Should react instantly", Q);
            fail_count = fail_count + 1;
        end
        @(posedge clk); #1; // Clock out of S2 (A&B was 0 when edge hit).

        // Scenario E: Reset from inside S2.
        // Get back to S2.
        apply_and_check(1, 0, 0, 12, "S0->S1: reset test setup");
        apply_and_check(0, 1, 0, 13, "S1->S2: reset test setup");

        // Assert reset while in S2 with A&B high.
        A = 1; B = 1; #1;
        reset = 1;
        #1; // Let reset propagate.

        // On next posedge, state goes to S0, Q must drop to 0.
        @(posedge clk); #1;
        if (Q === 0) begin
            $display("Test 14 | RESET from S2 with A&B=1 | Q=%0b (exp 0) | PASS | Reset overrides output", Q);
            pass_count = pass_count + 1;
        end else begin
            $display("Test 14 | RESET from S2 with A&B=1 | Q=%0b (exp 0) | FAIL <<< | Reset should override", Q);
            fail_count = fail_count + 1;
        end
        reset = 0;

        $display("------------------------------------------------------");
        $display("Result: %0d PASS, %0d FAIL out of %0d tests.",
            pass_count, fail_count, pass_count + fail_count);
        $display("======================================================");
        $finish;
    end

endmodule