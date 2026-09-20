const parser = @This();

const std = @import("std");
const lex = @import("lexer.zig");
const expr = @import("expression.zig");

pub fn parseText(arena: *std.mem.Allocator, code: []const u8) void {
    const tokens: []lex.Token = lex.lexer(arena, code);
    const Nodes: expr.Nodes = .{};
    const headNode: ?*expr.Node = parse(tokens, code, &0);
    headNode orelse return;
}
pub fn parse(tokens: []lex.Token, code: []const u8, index: *u32) ?*expr.Node {
    var headNode: ?*expr.Node = null;
    var tailNode: ?*expr.Node = null;
    var leftNode: ?*expr.Node = null;
    for (tokens, index.*..) |token, i| {
        if (token.type == .PARENTHESES and lex.isTokenChar(token, code, '(')) {
            parse(tokens, code, &i);
            continue;
        }
        if (token.type == .PARENTHESES and lex.isTokenChar(token, code, ')')) {
            continue;
        }
        if (token.type == .INTEGER or token.type == .FLOAT) {
            continue;
        }
        var currNode: expr.Node = .{};
        currNode.value = token;
        tailNode orelse return;
    }
    if (tailNode == null and leftNode != null) {
        return leftNode;
    }
    return headNode;
}
