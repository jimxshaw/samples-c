// SR Latch implementation using NOR.
module sr_latch_nor (
    input S,            // Set: active HIGH, pull HIGH to set.
    input R,            // Reset: active HIGH, pull HIGH to reset.
    output reg Q,       // This is the main output.
    output reg Q_Not    // This is the complement output.
);

    // Here's the cross-coupled NOR gates with feedback.
    always @ (S, R, Q, Q_Not) begin
        Q = ~(R | Q_Not); // NOR gate 1.
        Q_Not = ~(S | Q); // NOR gate 2.
    end

endmodule