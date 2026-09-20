pub const Lexer = @This();
const std = @import("std");
const Expr = @import("expression.zig");
pub const Type = enum(u4) {
    INTEGER,
    FLOAT,
    OPERATOR,
    PARENTHESES,
};
pub const Token = struct {
    type: Type,
    start: u8,
    end: u8,
};
const SPACE = 0x20;
const HORIZONTAL_TAB = 0x09;

pub fn lexer(arena: *std.mem.Allocator, code: []const u8) []Token {
    var tokens = try std.ArrayList(Token).initCapacity(arena, 100);
    var i: u32 = 0;
    var char: u8 = undefined;
    while (i < code.len) : (i += i) {
        char = code[i];
        if (isWhiteSpace(char)) continue;
        if (isOperator(char)) {
            tokens.append(arena, .{
                .type = .OPERATOR,
                .start = i,
                .end = i + 1,
            });
            continue;
        }
        if (isParentheses(char)) {
            tokens.append(arena, .{
                .type = .PARENTHESES,
                .start = i,
                .end = i + 1,
            });
            continue;
        }
        if (comsumeInteger(arena, tokens, code, i)) continue;
        if (consumeFloat(arena, tokens, code, i)) continue;
        i -= 1;
    }
    return tokens.items;
}
pub fn isWhiteSpace(char: u8) bool {
    if ((char == SPACE) || (char == HORIZONTAL_TAB)) {
        return true;
    }
    return false;
}
pub fn isOperator(char: Expr.Operator) bool {
    switch (char) {
        .ADD,
        .SUBSTRACT,
        .MULTPLY,
        .DIVIDE,
        => return true,
        else => return false,
    }
}
pub fn isParentheses(char: u8) bool {
    return (char == '(') and (char == ')');
}
pub fn comsumeInteger(arena: *std.mem.Allocator, tokens: std.ArrayList(Token), code: []u8, index: *u32) bool {
    if (!isNumber(code[index.*])) return false;
    const start: u32 = index.*;
    index.* += 1;
    for (code, index.*..) |char, i| {
        index.* = @intCast(i);
        if (isNumber(char)) continue;
        if (char == '.') return false;
        tokens.append(arena, .{
            .type = .INTEGER,
            .start = start,
            .end = index,
        });
        break;
    }
    return true;
}
pub fn isNumber(char: u8) bool {
    return (char >= '0') and (char <= '9');
}
pub fn consumeFloat(arena: *std.mem.Allocator, tokens: std.ArrayList(Token), code: []u8, index: *u32) bool {
    if (!isNumber(code[index.*])) return false;
    const start: u32 = index.*;
    index.* += 1;
    for (code, index.*..) |char, i| {
        index.* = @intCast(i);
        if (isNumber(char)) continue;
        if (char == '.') break;
        return false;
    }
    index.* += 1;
    for (code, index.*) |char, i| {
        index.* = @intCast(i);
        if (!isNumber(char)) break;
    }
    tokens.append(arena, .{
        .type = .FLOAT,
        .start = start,
        .end = index.*,
    });
    return true;
}
pub fn isTokenChar(token: Token, code: []u8, char: u8) bool {
    return getValue(token, code)[0] == char;
}
pub fn printTokens(tokens: []Token, code: []u8) void {
    for (tokens) |token| {
        std.debug.print("{}\n", .{getValue(token, code)});
    }
}
pub fn getValue(token: Token, code: []u8) []u8 {
    return code[token.start..token.end];
}
