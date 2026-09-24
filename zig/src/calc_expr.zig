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
pub fn calc(node: *Node, nodes: *Nodes) !Number {
    if (node.*.children[expr.LEFT] == null) {
        return returnNumber(node, nodes);
    }
    const left: Number = try calc(node.children[expr.LEFT].?, nodes);
    const right: Number = try calc(node.children[expr.RIGHT].?, nodes);
    return calc_op(getOp(node, nodes), left, right);
}
pub fn calc_op(op: Operator, a: Number, b: Number) !Number {
    switch (op) {
        .ADD => return add(a, b),
        .SUBSTRACT => return sub(a, b),
        .MULTPLY => return mult(a, b),
        .DIVIDE => return div(a, b),
        _ => return NumberError.UNKNOWN_OPERATOR,
    }
    return NumberError.EMPTY;
}
pub fn returnNumber(node: *Node, nodes: *Nodes) !Number {
    const token = expr.getToken(node, nodes.tokens);
    switch (token.type) {
        .INTEGER => return Number{ .integer = try std.fmt.parseInt(i32, lex.getValue(token, nodes.code), 10) },
        .FLOAT => return Number{ .decimal = try std.fmt.parseFloat(f32, lex.getValue(token, nodes.code)) },
        else => return NumberError.EMPTY,
    }
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

pub fn sub(a: Number, b: Number) !Number {
    switch (a) {
        .integer => {
            switch (b) {
                .integer => return Number{ .integer = a.integer - b.integer },
                .decimal => return Number{ .decimal = @as(f32, @floatFromInt(a.integer)) - b.decimal },
            }
        },
        .decimal => {
            switch (b) {
                .integer => return Number{ .decimal = a.decimal - @as(f32, @floatFromInt(b.integer)) },
                .decimal => return Number{ .decimal = a.decimal - b.decimal },
            }
        },
    }
}

pub fn mult(a: Number, b: Number) !Number {
    switch (a) {
        .integer => {
            switch (b) {
                .integer => return Number{ .integer = a.integer * b.integer },
                .decimal => return Number{ .decimal = @as(f32, @floatFromInt(a.integer)) * b.decimal },
            }
        },
        .decimal => {
            switch (b) {
                .integer => return Number{ .decimal = a.decimal * @as(f32, @floatFromInt(b.integer)) },
                .decimal => return Number{ .decimal = a.decimal * b.decimal },
            }
        },
    }
}

pub fn div(num_a: Number, num_b: Number) !Number {
    //TODO: leanrn how to check tagged union payload
    switch (num_b) {
        .integer => |b| if (b == 0) return NumberError.NOT_DIVISBLE_BY_ZERO,
        .decimal => |b| if (b == 0) return NumberError.NOT_DIVISBLE_BY_ZERO,
    }
    switch (num_a) {
        .integer => |a|{
            switch (num_b) {
                .integer => |b| return Number{ .decimal = @as(f32, @floatFromInt(a)) / @as(f32, @floatFromInt(b)) },
                .decimal => |b| return Number{ .decimal = @as(f32, @floatFromInt(a)) / b },
            }
        },
        .decimal => |a|{
            switch (num_b) {
                .integer => |b| return Number{ .decimal = a / @as(f32, @floatFromInt(b)) },
                .decimal => |b| return Number{ .decimal = a / b },
            }
        },
    }
}
pub fn numberToString(arena: std.mem.Allocator, number: Number) ![]const u8 {
    switch (number) {
        .integer => {
            return std.fmt.allocPrint(arena, "{d}", .{number.integer});
        },
        .decimal => {
            return std.fmt.allocPrint(arena, "{d}", .{number.decimal});
        },
    }
}
pub fn getOp(node: *Node, nodes: *Nodes) Operator {
    return @enumFromInt(lex.getValue(expr.getToken(node, nodes.tokens), nodes.code)[0]);
}
