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
    _,

    pub fn getPriority(self: Operator) i8 {
        switch (self) {
            .ADD,
            .SUBSTRACT,
            => return 0,
            .MULTPLY,
            .DIVIDE,
            => return 1,
            _ => unreachable,
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
    parent: ?*Node = null,
    children: ?[]*Node = null,
    value: u32,
};

pub const LEFT = @as(u32, 0);
pub const RIGHT = @as(u32, 1);

pub const Nodes = struct {
    nodes: []Node,
    tokens: []Token,
    code: []const u8,
    curr: *usize,
    pub fn getCurr(self: Nodes) *Node {
        return &self.nodes[self.curr.*];
    }
    pub fn getCurrChild(self: Nodes, direction: u8) void {
        self.curr.* = self.nodes[self.curr.*].children.?[direction].value;
    }
    pub fn addExprToTree(self: Nodes, currNode: *Node, headNode: **Node, tailNode: **Node) void {
        const cmp = tokenOpCmp(currNode, tailNode.*, self);
        switch (cmp) {
            .EQUAL,
            .LESSER,
            => {
                var prev = tailNode.*;
                while (prev.*.parent != null) {
                    if (.GREATER == tokenOpCmp(currNode, prev.*.parent.?, self)) {
                        break;
                    }
                    prev = prev.*.parent.?;
                }
                if (prev.*.parent == null) {
                    headNode.* = currNode;
                } else {
                    prev.*.parent.?.children.?[RIGHT] = currNode;
                    currNode.*.parent.? = prev.*.parent.?;
                }
                tailNode.*.children.?[RIGHT] = currNode.*.children.?[LEFT];
                tailNode.*.children.?[RIGHT].parent.? = tailNode.*;
                currNode.*.children.?[LEFT] = prev;
                prev.*.parent.? = currNode;
                tailNode.* = currNode;
            },
            else => {
                tailNode.*.children.?[RIGHT] = currNode;
                currNode.*.parent.? = tailNode.*;
                tailNode.* = currNode;
            },
        }
    }
};

pub fn addChildNode(parent: ?*Node, child: ?*Node, direction: u32) void {
    parent.?.*.children.?[direction] = child.?;
}
pub fn tokenOpCmp(curr: *Node, other: *Node, nodes: Nodes) Compare {
    const currToken = getToken(curr, nodes.tokens);
    const otherToken = getToken(other, nodes.tokens);
    const currVal: Operator = @enumFromInt(lexer.getValue(currToken, nodes.code)[0]);
    const otherVal: Operator = @enumFromInt(lexer.getValue(otherToken, nodes.code)[0]);
    return Operator.cmp(currVal, otherVal);
}
pub fn getToken(node: *Node, tokens: []Token) Token {
    return tokens[node.*.value];
}
