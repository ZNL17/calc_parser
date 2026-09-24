const std = @import("std");
const Io = std.Io;

const zig = @import("zig");
const parser = @import("parser.zig");
const calc = @import("calc_expr.zig");

pub fn main(init: std.process.Init) !void {
    // This is appropriate for anything that lives as long as the process.

    const arena: std.mem.Allocator = init.arena.allocator();

    // Accessing command line arguments:
    const args = try init.minimal.args.toSlice(arena);
    if (args.len < 2) return;
    // In order to do I/O operations need an `Io` instance.
    const io = init.io;

    // Stdout is for the actual output of your application, for example if you
    // are implementing gzip, then only the compressed bytes should be sent to
    // stdout, not any debugging messages.

    var stdout_buffer: [1024]u8 = undefined;
    var stdout_file_writer: Io.File.Writer = .init(.stdout(), io, &stdout_buffer);
    const stdout_writer = &stdout_file_writer.interface;

    if (args.len == 2) {
        //TODO: how to defer flush?
        const number = parser.parseText(arena, args[1], stdout_writer) catch |err| {
            handParseError(err, stdout_writer);
            return;
        };
        if (number) |num| {
            stdout_writer.print("{s}", .{calc.numberToString(arena, num)});
        } else {
            stdout_writer.print("couldn't parse the input\n");
        }
        try stdout_writer.flush(); // Don't forget to flush!
        return;
    }
    if (args.len == 3 and std.mem.eql(u8, "-f", args[1])) {
        try parser.parseFiles(arena, io, args[2], stdout_writer);
        try stdout_writer.flush(); // Don't forget to flush!
        return;
    }
}
pub fn handParseError(err: anyerror, writer: *std.Io.Writer) void {
    switch (err) {
        error.NO_NUMBERS, error.NOT_DIVISBLE_BY_ZERO, error.UNKNOWN_OPERATOR, error.EMPTY, error.Overflow, error.InvalidCharacter => |e| {
            writer.print("{s}", .{@errorName(e)}) catch |ioErr| {
                switch (ioErr) {
                    error.WriteFailed => return,
                }
            };
            try writer.flush();
        },
        else => return,
    }
}

test "simple test" {
    const gpa = std.testing.allocator;
    var list: std.ArrayList(i32) = .empty;
    defer list.deinit(gpa); // Try commenting this out and see if zig detects the memory leak!
    try list.append(gpa, 42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}

test "fuzz example" {
    try std.testing.fuzz({}, testOne, .{});
}

fn testOne(context: void, smith: *std.testing.Smith) !void {
    _ = context;
    // Try command `zig build test --fuzz -Doptimize=ReleaseFast` to see if it manages to fail this test case!

    const gpa = std.testing.allocator;
    var list: std.ArrayList(u8) = .empty;
    defer list.deinit(gpa);
    while (!smith.eos()) switch (smith.value(enum { add_data, dup_data })) {
        .add_data => {
            const slice = try list.addManyAsSlice(gpa, smith.value(u4));
            smith.bytes(slice);
        },
        .dup_data => {
            if (list.items.len == 0) continue;
            if (list.items.len > std.math.maxInt(u32)) return error.SkipZigTest;
            const len = smith.valueRangeAtMost(u32, 1, @min(32, list.items.len));
            const off = smith.valueRangeAtMost(u32, 0, @intCast(list.items.len - len));
            try list.appendSlice(gpa, list.items[off..][0..len]);
            try std.testing.expectEqualSlices(
                u8,
                list.items[off..][0..len],
                list.items[list.items.len - len ..],
            );
        },
    };
}
