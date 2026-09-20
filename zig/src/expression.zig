const lexer = @import("lexer.zig");
const Token = lexer.Token;
pub const Expression = @This();

const Compare = enum(i8) {
    EQUAL = 0,
    GREATER = 1,
    LESSER = -1,
};
pub const Operator = enum(u8) {
    ADD = 43,
    SUBSTRACT = 45,
    MULTPLY = 42,
    DIVIDE = 47,

    pub fn getPriority(self: Operator) i8 {
        switch (self) {
            .ADD,
            .SUBSTRACT,
            => return 0,
            .MULTPLY,
            .DIVIDE,
            => return 1,
            else => unreachable,
        }
    }
    pub fn cmp(self: Operator, other: Operator) Compare {
        const diff = self.getPriority() - other.getPriority();
        if (diff > 0) {
            return Compare.GREATER;
        } else if (diff < 0) {
            return Compare.LESSER;
        } else {
            return Compare.EQUAL;
        }
    }
};
pub const Node = struct {
    parent: ?[1]Node = null,
    children: ?[]Node = null,
    value: usize,
};

pub const LEFT = 0;
pub const RIGHT = 1;
pub const Nodes = struct {
    nodes: []Node,
    tokens: []Token,
    code: []const u8,
    curr: usize,
    pub fn addExprToTree(self: Nodes, currNode: *Node, headNode: **Node, tailNode: **Node) void {
        const cmp = tokenOpCmp(getToken(currNode, self.code), getToken(tailNode, self.code));
        switch (cmp) {
            .EQUAL,
            .LESSER,
            => {
                var prev = tailNode.*;
                while (prev.*.parent != null) {
                    if (.GREATER == tokenOpCmp(getToken(currNode, self.code), getToken(prev.*.parent, self.code))) {
                        break;
                    }
                    prev = prev.*.parent;
                }
                if (prev.*.parent == null) {
                    headNode.* = currNode;
                } else {
                    prev.*.parent[RIGHT] = currNode;
                    currNode.*.parent = prev.*.parent;
                }
                tailNode.*.children[RIGHT] = currNode.*.children[LEFT];
                tailNode.*.children[RIGHT].parent = tailNode.*;
                currNode.*.children[LEFT] = prev;
                prev.*.parent = currNode;
                tailNode.* = currNode;
            },
            else => {
                tailNode.*.children[RIGHT] = currNode;
                currNode.*.parent = tailNode.*;
                tailNode.* = currNode;
            },
        }
    }
};
pub fn tokenOpCmp(curr: Token, other: Token) Compare {
    const currVal: Operator = @intCast(lexer.getValue(curr, Nodes.code)[0]);
    const otherVal: Operator = @intCast(lexer.getValue(other, Nodes.code)[0]);
    return Operator.cmp(currVal, otherVal);
}
pub fn getToken(node: Node, tokens: []Token) Token {
    return tokens[node.value];
}
