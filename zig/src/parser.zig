const parser = @This();

const std = @import("std");
const lex = @import("lexer.zig");
const expr = @import("expression.zig");
const calc = @import("calc_expr.zig");
const Nodes = expr.Nodes;
const Node = expr.Node;
const Token = lex.Token;

pub fn parseText(arena: std.mem.Allocator, code: []const u8, output: *std.Io.Writer) !void {
    const nodes: *Nodes = try arena.create(Nodes);
    nodes.tokens = try lex.lexer(arena, code);
    const headNode: ?*Node = try parse(arena, nodes, @constCast(&@as(u32, 0)));
    if (headNode) |_| {
        const result = calc.calc(nodes) catch |err| {
            switch (err) {
                error.NO_NUMBERS => {
                    return;
                },
                error.NOT_DIVISBLE_BY_ZERO => {
                    return;
                },
                error.UNKNOWN_OPERATOR => {
                    return;
                },
                error.EMPTY => {
                    return;
                },
            }
        };
        _ = try output.write(try calc.numberToString(arena, result));
        try output.flush();
    } else {
        return;
    }
}
pub fn parse(arena: std.mem.Allocator, nodes: *Nodes, index: *u32) !?*Node {
    var headNode: ?*Node = null;
    var tailNode: ?*Node = null;
    var leftNode: ?*Node = null;
    var token: Token = undefined;
    while (index.* < nodes.tokens.len) : (index.* += 1) {
        token = nodes.tokens[index.*];
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, '(')) {
            index.* += 1; // TODO: could be a of by one error (didnt check for bounds?)?
            leftNode = try parse(arena, nodes, index);
            continue;
        }
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, ')')) {
            expr.addChildNode(tailNode, leftNode, expr.RIGHT);
            return headNode;
        }
        if (token.type == .INTEGER or token.type == .FLOAT) {
            leftNode = try arena.create(Node);
            leftNode.?.* = .{
                .value = index.*,
            };
            continue;
        }
        const currNode: *Node = try arena.create(Node);
        currNode.* = .{
            .value = index.*,
        };
        if (tailNode == null) {
            headNode = currNode;
            tailNode = currNode;
            continue;
        }
        nodes.addExprToTree(currNode, &headNode.?, &tailNode.?);
    }
    if (tailNode == null and leftNode != null) {
        return leftNode;
    }
    expr.addChildNode(tailNode, leftNode, expr.RIGHT);
    return headNode;
}
