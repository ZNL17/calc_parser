const parser = @This();

const std = @import("std");
const zig = @import("zig");
const lex = @import("lexer.zig");
const expr = @import("expression.zig");
const calc = @import("calc_expr.zig");
const Number = calc.Number;
const Nodes = expr.Nodes;
const Node = expr.Node;
const Token = lex.Token;

const errorCollc = struct {
    err: anyerror,
    it: std.mem.SplitIterator,
};
pub fn parseText(arena: std.mem.Allocator, code: []const u8, writer: *std.Io.Writer) !?Number {
    const nodes: *Nodes = try arena.create(Nodes);
    nodes.* = .{ .tokens = try lex.lexer(arena, code), .code = code, .nodes = undefined, .curr = @constCast(&[1]?*Node{null}) };
    var i: u32 = 0;
    //try lex.printTokenValues(writer, nodes.tokens, code);
    const headNode: ?*Node = try parse(arena, writer, nodes, &i);

    if (headNode) |_| {
        return try calc.calc(headNode.?, nodes);
    } else {
        return null;
    }
}
pub fn parse(arena: std.mem.Allocator, writer: *std.Io.Writer, nodes: *Nodes, index: *u32) !?*Node {
    var headNode: ?*Node = null;
    var tailNode: ?*Node = null;
    var leftNode: ?*Node = null;
    var token: Token = undefined;
    while (index.* < nodes.tokens.len) : (index.* += 1) {
        token = nodes.tokens[index.*];
        //try lex.printToken(writer, token, nodes.code);
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, '(')) {
            index.* += 1; // TODO: could be a of by one error (didnt check for bounds?)?
            leftNode = try parse(arena, writer, nodes, index);
            continue;
        }
        if (token.type == .PARENTHESES and lex.isTokenChar(token, nodes.code, ')')) {
            //TODO: tailNode can be null should i put a null check there?
            expr.addChildNode(tailNode.?, leftNode.?, expr.RIGHT);
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
        return leftNode;
    }
    expr.addChildNode(tailNode.?, leftNode.?, expr.RIGHT);
    return headNode;
}
pub fn parseFiles(alloc: std.mem.Allocator, io: std.Io, filePath: []const u8, writer: *std.Io.Writer) !void {
    if (std.Io.Dir.cwd().openFile(io, filePath, .{ .mode = .read_only, .lock = .exclusive })) |file| {
        defer file.close(io);
        var buf: [1024]u8 = undefined;
        var reader: std.Io.File.Reader = file.reader(io, &buf);
        const errArr = std.ArrayList(errorCollc);
        while (try reader.interface.takeDelimiter('\n')) |line| {
            const it = std.mem.splitAny(u8, line, ";");
            parseText(alloc, line, writer) catch |err| collecError(alloc, errArr, err, it);
        }
    } else |err| switch (err) {
        error.FileNotFound, error.AccessDenied => {
            try writer.print("unable to open file: {}", .{err});
            try writer.flush();
        },
        else => |e| return e,
    }
}
pub fn collecError(alloc: std.mem.Allocator, arr: std.ArrayList, err: anyerror, it: std.mem.SplitIterator) void {
    _ = alloc;
    _ = arr;
    _ = err;
    _ = it;
}
