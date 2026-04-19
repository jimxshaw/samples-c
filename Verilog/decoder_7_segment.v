module decoder_hex_7_segment (
    input [3:0] hex_in,             // 4-bit hex input (0-F).
    output reg [6:0] segment_out    // 7 segments: {a,b,c,d,e,f,g}.
);

// Always with asterisk (*) means run this block 
// whenever any input changes and recalculate the output.
always @(*) begin
    case (hex_in)
        // 4 is number of bits.
        // h is hex format.
        // 0 or whatever is the value.

        // 7 is the number of bits.
        // b is binary format.
        // 1111110 or whatever is the actual bit pattern.
        4'h0: segment_out = 7'b1111110; // 0.
        4'h1: segment_out = 7'b0110000; // 1.
        4'h2: segment_out = 7'b1101101; // 2.
        4'h3: segment_out = 7'b1111001; // 3.
        4'h4: segment_out = 7'b0110011; // 4.
        4'h5: segment_out = 7'b1011011; // 5.
        4'h6: segment_out = 7'b1011111; // 6.
        4'h7: segment_out = 7'b1110000; // 7.
        4'h8: segment_out = 7'b1111111; // 8.
        4'h9: segment_out = 7'b1111011; // 9.

        4'hA: segment_out = 7'b1110111; // A.
        4'hB: segment_out = 7'b0011111; // B.
        4'hC: segment_out = 7'b1001110; // C.
        4'hD: segment_out = 7'b0111101; // D.
        4'hE: segment_out = 7'b1001111; // E.
        4'hF: segment_out = 7'b1000111; // F.

        default: segment_out = 7'b0000000; // blank.
    endcase
end

endmodule