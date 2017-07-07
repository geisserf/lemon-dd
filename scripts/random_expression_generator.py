from random import random, randint, choice

vars = ["Var0", "Var1", "Var2", "Var3", "Var4", "Var5", "Var6", "Var7", "Var8",
        "Var9",
        "Var10", "Var11", "Var12", "Var13", "Var14", "Var15", "Var16", "Var17",
        "Var18", "Var19",
        "Var20", "Var21", "Var22", "Var23", "Var24", "Var25", "Var26", "Var26",
        "Var28", "Var29",
        "Var30", "Var31", "Var32", "Var33", "Var34", "Var35", "Var36", "Var36",
        "Var38", "Var39",
        "Var40", "Var41", "Var42", "Var43", "Var44", "Var45", "Var46", "Var46",
        "Var48", "Var49",
        "Var50", "Var51", "Var52", "Var53", "Var54", "Var55", "Var56", "Var56",
        "Var58", "Var59",
        "Var60", "Var61", "Var62", "Var63", "Var64", "Var65", "Var66", "Var66",
        "Var68", "Var69",
        "Var70", "Var71", "Var72", "Var73", "Var74", "Var75", "Var76", "Var76",
        "Var78", "Var79",
        "Var80", "Var81", "Var82", "Var83", "Var84", "Var85", "Var86", "Var86",
        "Var88", "Var89",
        "Var90", "Var91", "Var92", "Var93", "Var94", "Var95", "Var96", "Var96",
        "Var89", "Var99"];

operators = {
    '*': {'prec': 20, 'prob': .2},
    '/': {'prec': 20, 'prob': .2},
    '+': {'prec': 30, 'prob': .3},
    '-': {'prec': 30, 'prob': .3}}

max_levels = 2
integer_range = (0, 100)


# A node in an expression tree
class expression(object):
    def __init__(self):
        super(expression, self).__init__()

    def precedence(self):
        return -1


    @classmethod
    def create_random(cls, level):
        if level == 0:
            is_op = True
        elif level == max_levels:
            is_op = False
        else:
            is_op = random() <= 1.0 - pow(level / max_levels, 2.0)

        if is_op:
            return binary_expression.create_random(level)
        else:
            return integer_expression.create_random(level)


class integer_expression(expression):
    def __init__(self, value):
        super(integer_expression, self).__init__()

        togle = randint(0, 1)

        if togle == 0:
            self.value = value
        else:
            self.value = vars[value % len(vars)]


    def __str__(self):
        return self.value.__str__()

    def precedence(self):
        return 0

    @classmethod
    def create_random(cls, level):
        return integer_expression(randint(integer_range[0],
                                          integer_range[1]))


class binary_expression(expression):
    def __init__(self, symbol, left_expression, right_expression):
        super(binary_expression, self).__init__()

        self.symbol = symbol
        self.left = left_expression
        self.right = right_expression


    @classmethod
    def create_random(cls, level):
        symbol = None

        # Choose an operator based on its probability distribution
        r = random()
        cumulative = 0.0
        for k, v in operators.items():
            cumulative += v['prob']
            if r <= cumulative:
                symbol = k
                break

        assert symbol != None

        left = expression.create_random(level + 1)
        right = expression.create_random(level + 1)

        return binary_expression(symbol, left, right)

    def precedence(self):
        return operators[self.symbol]['prec']

    def __str__(self):
        left_str = self.left.__str__()
        right_str = self.right.__str__()
        op_str = self.symbol

        # Use precedence to determine if we need to put the sub expressions in
        # parentheses
        if self.left.precedence() > self.precedence():
            left_str = '(' + left_str + ')'
        if self.right.precedence() > self.precedence():
            right_str = '(' + right_str + ')'

        # Nice to have space around low precedence operators
        if operators[self.symbol]['prec'] >= 30:
            op_str = ' ' + op_str + ' '

        #return left_str + op_str + right_str
        return "("+left_str + op_str + right_str+")"


for i in range(1000):
    expr = expression.create_random(0)
    print(expr)

