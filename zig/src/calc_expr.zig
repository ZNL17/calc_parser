const std = @import("std");
const expr = @import("expression.zig");
const Node = expr.Node;
const Operator = expr.Operator;

comptime {
    const op_func = std.ArrayHashMapUnmanaged(Operator, *const fn (Number, Number) NumberError!Number, std.hash_map.StringContext, false);
    op_func = .empty;
    op_func.put(@This(), std.heap.FixedBufferAllocator, .ADD, add);
}
const Numeric = enum {
    integer,
    decimal,
};
pub const Number = union(Numeric) {
    integer: i32,
    decimal: f32,
};

pub const NumberError = error{
    NO_NUMBERS,
    NOT_DIVISBLE_BY_ZERO,
};
pub fn calc(node: *Node) NumberError!Number {
    try node.*.children[expr.LEFT] catch {
        return NumberError.NO_NUMBERS;
    };
}
pub fn calc_op(op: Node, a: Number, b: Number) NumberError!Number {}
pub fn add(a: Number, b: Number) NumberError!Number {
    switch (a) {
        .integer => {
            switch (b) {
                .integer => return a.integer + b.integer,
                .decimal => return Number{ .deciaml = (@floatFromInt(a.integer) + b.decimal) },
                else => unreachable,
            }
        },
        .decimal => {
            switch (b) {
                .integer => return a.decimal + @floatFromInt(b.integer),
                .decimal => return a.decimal + b.decimal,
                else => unreachable,
            }
        },
        else => unreachable,
    }
}
