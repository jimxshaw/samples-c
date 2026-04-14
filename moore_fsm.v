// This is a verilog module for an example Moore FSM.
module fsm_moore (
    input clk,        // Clock signal.
    input reset,      // Reset to S0.
    input A,          // Input signal A.
    input B,          // Input signal B.
    output reg Q      // Moore output, which depends only on the state.
);

    // This is the state encoding by giving each state a number.
    parameter S0 = 2'b00;
    parameter S1 = 2'b01;
    parameter S2 = 2'b10;

    reg [1:0] current_state, next_state;

    // Here we register the state by updating the current
    // state on every clock edge.
    always @(posedge clk or posedge reset) begin
        if (reset)
            current_state <= S0;  // Reset always goes to S0.
        else
            current_state <= next_state;
    end

    // Here is the next state logic by figuring out where
    // to go based on the current state and the inputs.
    always @(*) begin
        case (current_state)
            S0: begin
                if (A)
                    next_state = S1;  // A -> go to S1.
                else
                    next_state = S0;  // NOT(A) -> stay at S0.
            end

            S1: begin
                if (B)
                    next_state = S2;  // B -> go to S2.
                else
                    next_state = S0;  // NOT(B) -> back to S0.
            end

            S2: begin
                next_state = S0;      // Always go back to S0.
            end

            default: next_state = S0;
        endcase
    end

    // Here's the output logic where the output depends
    // only on the current state. No inputs are needed.
    always @(*) begin
        case (current_state)
            S0: Q = 1'b0;   // Q = 0 in S0.
            S1: Q = 1'b0;   // Q = 0 in S1.
            S2: Q = 1'b1;   // Q = 1 in S2.

            default: Q = 1'b0;
        endcase
    end

endmodule
