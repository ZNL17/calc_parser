const parser = @This();

const std = @import("std");
const lex = @import("lexer.zig");
const expr = @import("expression.zig");
const calc = @import("calc_expr.zig");
const Nodes = expr.Nodes;
const Node = expr.Node;
const Token = lex.Token;

pub fn parseText(arena: std.mem.Allocator, code: []const u8, output: *std.Io.Writer) !void {
    //_ = try std.mem.Allocator.print(arena, "string: {s}: {d}\n", .{ code, code.len });
    const nodes: *Nodes = try arena.create(Nodes);
    nodes.* = .{ .tokens = try lex.lexer(arena, code), .code = code,  .nodes = undefined };

    var i: u32 = 0;
    const headNode: ?*Node = try parse(arena, nodes, &i);
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
        //try lex.printToken(arena, token, nodes.code);
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, '(')) {
            index.* += 1; // TODO: could be a of by one error (didnt check for bounds?)?
            leftNode = try parse(arena, nodes, index);
            continue;
        }
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, ')')) {
            expr.addChildNode(tailNode.?, leftNode.?, expr.RIGHT);
            nodes.addCurr(headNode);
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
        expr.addChildNode(currNode, leftNode.?, expr.LEFT);
        if (tailNode == null) {
            headNode = currNode;
            tailNode = currNode;
            continue;
        }
        nodes.addExprToTree(currNode, &headNode.?, &tailNode.?);
    }
    if (tailNode == null and leftNode != null) {
        nodes.addCurr(leftNode);
        return leftNode;
    }
    expr.addChildNode(tailNode.?, leftNode.?, expr.RIGHT);
    nodes.addCurr(headNode);
    return headNode;
}
