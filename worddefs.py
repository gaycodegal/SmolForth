from forthglobals import *

def cColon(stack, links, defs, cWords, rWords):
    links.append(len(stack))

def cSemiColon(stack, links, defs, cWords, rWords):
    top = len(stack)
    bottom = links.pop()
    seq = stack[bottom + 2:top]
    ind = len(defs)
    seq.append(SYMRET)
    defs.extend(seq)
    if top-bottom >= 2:
        rWords[stack[bottom + 1]] = ind
    for i in range(top-bottom):
        stack.pop()
    
def rAdd(stack, links):
    b = stack.pop()
    a = stack.pop()
    stack.append(a + b)

def rDot(stack, links):
    print(stack.pop())

def rGetInt(stack, links):
    print("enter an int:")
    k = int(input())
    stack.append(k)
    
def cIf(stack, links, defs, cWords, rWords):
    stack.extend([0,0])
    links.append(len(stack))
    
def cElse(stack, links, defs, cWords, rWords):
    stack.extend([0,0])
    links.append(len(stack))

def cThen(stack, links, defs, cWords, rWords):
    top = len(stack)
    mid = links.pop()
    bottom = links.pop()
    stack[bottom - 2] = SYMRBRANCH
    stack[bottom - 1] = mid-bottom
    stack[mid - 2] = SYMRJUMP
    stack[mid - 1] = top-mid
    
#  "if":cIf, "else":cElse, "then":cThen}
