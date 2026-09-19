const Lexer = @import("lexer.zig");
pub const Expression = @This();
pub const Operator = enum(u8) {
    ADD = 43,
    SUBSTRACT = 45,
    MULTPLY = 42,
    DIVIDE = 47,
};
pub const Node = struct {
    parent: ?[1]Node,
    children: ?[]Node,
    value: Lexer.Token,
};

const numeric = enum {
    integer,
    decimal,
};
pub const Number = union(numeric) {
    integer: i32,
    decimal: f32,
};
pub fn main() void {}
