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
    start: u32,
    end: u32,
};

const SPACE = @as(u8, 0x20);
const HORIZONTAL_TAB = @as(u8, 0x09);
pub fn lexer(arena: std.mem.Allocator, code: []const u8) ![]Token {
    var tokens = try std.ArrayList(Token).initCapacity(arena, 100);
    var i: u32 = 0;
    while (i < code.len) : (i += 1) {
        const char = code[i];
        if (!isInWhiteList(char)) continue;
        if (isWhiteSpace(char)) continue;
        if (isOperator(char)) {
            try tokens.append(arena, .{
                .type = .OPERATOR,
                .start = i,
                .end = i + 1,
            });
            continue;
        }
        if (isParentheses(char)) {
            try tokens.append(arena, .{
                .type = .PARENTHESES,
                .start = i,
                .end = i + 1,
            });
            continue;
        }
        if (try comsumeInteger(arena, &tokens, code, &i)) continue;
        if (try consumeFloat(arena, &tokens, code, &i)) continue;
    }
    return tokens.items;
}
pub fn isInWhiteList(char: u8) bool {
    if (isWhiteSpace(char)) return true;
    if (isNumber(char)) return true;
    if (isOperator(char)) return true;
    if (char == '.' or char == ',') return true;
    if (char == '(' or char == ')') return true;
    return false;
}
pub fn isWhiteSpace(char: u8) bool {
    return char == SPACE or char == HORIZONTAL_TAB;
}
pub fn isOperator(char: u8) bool {
    const value: Expr.Operator = @enumFromInt(char);
    switch (value) {
        .ADD,
        .SUBSTRACT,
        .MULTPLY,
        .DIVIDE,
        => return true,
        _ => return false,
    }
}
pub fn isParentheses(char: u8) bool {
    return char == '(' or char == ')';
}
pub fn comsumeInteger(arena: std.mem.Allocator, tokens: *std.ArrayList(Token), code: []const u8, index: *u32) !bool {
    if (!isNumber(code[index.*])) return false;
    const start: u32 = index.*;
    index.* += 1;
    while (index.* < code.len) : (index.* += 1) {
        const char = code[index.*];
        if (isNumber(char)) continue;
        if (char == '.') {
            index.* = start;
            return false;
        }
        break;
    }

    try tokens.append(arena, .{
        .type = .INTEGER,
        .start = start,
        .end = index.*,
    });
    index.* -= 1;
    return true;
}
pub fn isNumber(char: u8) bool {
    return char >= '0' and char <= '9';
}
pub fn consumeFloat(arena: std.mem.Allocator, tokens: *std.ArrayList(Token), code: []const u8, index: *u32) !bool {
    if (!isNumber(code[index.*])) return false;
    const start: u32 = index.*;
    index.* += 1;
    while (index.* < code.len) : (index.* += 1) {
        const char = code[index.*];
        if (isNumber(char)) continue;
        if (char == '.') break;
        index.* = start;
        return false;
    }
    index.* += 1;
    while (index.* < code.len) : (index.* += 1) {
        const char = code[index.*];
        if (!isNumber(char)) break;
    }
    try tokens.append(arena, .{
        .type = .FLOAT,
        .start = start,
        .end = index.*,
    });

    index.* -= 1;
    return true;
}
pub fn isTokenChar(token: Token, code: []const u8, char: u8) bool {
    return getValue(token, code)[0] == char;
}
pub fn printToken(writer: *std.Io.Writer, token: Token, code: []const u8) !void {
    try writer.print("<{s}> -> ", .{getTokenType(token)});
    try writer.print("<{s}>\n", .{getValue(token, code)});
    try writer.flush();
}
pub fn printTokens(writer: *std.Io.Writer, tokens: []Token, code: []const u8) !void {
    for (tokens) |token| {
        try writer.print("<{s}> -> ", .{getTokenType(token)});
        try writer.print("<{s}>\n", .{getValue(token, code)});
        try writer.flush();
    }
}
pub fn printTokenValues(writer: *std.Io.Writer, tokens: []Token, code: []const u8) !void {
    for (tokens) |token| {
        try writer.print("{s}", .{getValue(token, code)});
        try writer.flush();
    }
}
pub fn getValue(token: Token, code: []const u8) []const u8 {
    return code[token.start..token.end];
}
pub fn getTokenType(token: Token) []const u8 {
    return @tagName(token.type);
}
