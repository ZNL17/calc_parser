const std = @import("std");
pub fn main() void {
    var num: u8 = 0;
    for (num..10) |i| {
        num = @intCast(i);
        const j: *usize = &i;
        std.debug.print("{d}\n", .{i});
        j.* = 0;
    }
    std.debug.print("last: {d}\n", .{num});
}
