const std = @import("std");
const expr = @import("expression.zig");
const lex = @import("lexer.zig");
const Node = expr.Node;
const Nodes = expr.Nodes;
const Operator = expr.Operator;
const Token = lex.Token;
const LEFT = expr.LEFT;
const RIGHT = expr.RIGHT;
// TODO: need to find out  to fille a hashmap a compile time
//
//comptime {
//    const op_func = std.ArrayHashMapUnmanaged(Operator, *const fn (Number, Number) NumberError!Number, std.hash_map.StringContext, false);
//    op_func = .empty;
//    op_func.put(@This(), std.heap.FixedBufferAllocator, .ADD, add);
//}
const Numeric = enum {
    integer,
    decimal,
};
pub const Number = union(Numeric) {
    integer: i32,
    decimal: f32,
};

pub const NumberError = error{ NO_NUMBERS, NOT_DIVISBLE_BY_ZERO, UNKNOWN_OPERATOR, EMPTY };
pub fn calc(nodes: *Nodes) NumberError!Number {
    const node: *Node = nodes.getCurr();
    if (node.*.children[expr.LEFT] == null) {
        return NumberError.NO_NUMBERS;
    }
    nodes.getCurrChild(expr.LEFT);
    const left: Number = try calc(nodes);
    nodes.getCurrChild(expr.RIGHT);
    const right: Number = try calc(nodes);
    return calc_op(getOp(node, nodes.tokens, nodes.code), left, right);
}
pub fn calc_op(op: Operator, a: Number, b: Number) NumberError!Number {
    switch (op) {
        .ADD => {
            return add(a, b);
        },
        .SUBSTRACT => {},
        .MULTPLY => {},
        .DIVIDE => {},
        _ => return NumberError.UNKNOWN_OPERATOR,
    }
    return NumberError.EMPTY;
}
pub fn add(a: Number, b: Number) !Number {
    switch (a) {
        .integer => {
            switch (b) {
                .integer => return Number{ .integer = a.integer + b.integer },
                .decimal => return Number{ .decimal = @as(f32, @floatFromInt(a.integer)) + b.decimal },
            }
        },
        .decimal => {
            switch (b) {
                .integer => return Number{ .decimal = a.decimal + @as(f32, @floatFromInt(b.integer)) },
                .decimal => return Number{ .decimal = a.decimal + b.decimal },
            }
        },
    }
}
pub fn numberToString(arena: std.mem.Allocator, number: Number) ![]u8 {
    switch (number) {
        .integer => {
            return std.fmt.allocPrint(arena, "{d}", .{number.integer});
        },
        .decimal => {
            return std.fmt.allocPrint(arena, "{d}", .{number.decimal});
        },
    }
}
pub fn getOp(node: *Node, tokens: []Token, code: []const u8) Operator {
    return @enumFromInt(lex.getValue(expr.getToken(node, tokens), code)[0]);
}
