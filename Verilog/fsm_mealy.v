// This is a verilog module for an example Mealy FSM.+
module fsm_mealy (
    input clk,        // Clock signal.
    input reset,      // Reset to S0.
    input A,          // Input signal A.
    input B,          // Input signal B.
    output reg Q      // Mealy output, which depends on state AND inputs.
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
            current_state <= S0;
        else
            current_state <= next_state;
    end

    // We combine Next State and Output logic for Mealy FSMs
    // because output depends on both state and input.
    always @(*) begin
        // We give some default values to prevents latches.
        next_state = S0;
        Q = 1'b0;

        case (current_state)
            S0: begin
                if (A) begin
                    next_state = S1;  // A -> go to S1.
                    Q = 1'b0;         // output 0.
                end else begin
                    next_state = S0;  // NOT(A) -> stay S0.
                    Q = 1'b0;         // output 0.
                end
            end

            S1: begin
                if (B) begin
                    next_state = S2;  // B -> go to S2.
                    Q = 1'b0;         // output 0.
                end else begin
                    next_state = S0;  // NOT(B) -> back to S0.
                    Q = 1'b0;         // output 0.
                end
            end

            S2: begin
                if (A & B) begin
                    next_state = S2;  // AB -> stay at S2.
                    Q = 1'b1;         // output 1 only happens here.
                end else begin
                    next_state = S0;  // NOT(A) or NOT(B) -> back to S0.
                    Q = 1'b0;         // output 0.
                end
            end

            default: begin
                next_state = S0;
                Q = 1'b0;
            end
        endcase
    end

endmodule