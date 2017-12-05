from forthglobals import *

def cColon(stack, links, defs, cWords, rWords):
    """starts a function definition"""
    links.append(len(stack))

def cSemiColon(stack, links, defs, cWords, rWords):
    """ends a function definition, instantializes the funciton"""
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
    """adds top two stack numbers"""
    b = stack.pop()
    a = stack.pop()
    stack.append(a + b)

def rDot(stack, links):
    """prints top stack var"""
    print(stack.pop())

def rGetInt(stack, links):
    """gets an int, just for testing purposes"""
    print("enter an int:")
    k = int(input())
    stack.append(k)
    
def cIf(stack, links, defs, cWords, rWords):
    """starts a if else then statement"""
    stack.extend([0,0])
    links.append(len(stack))
    
def cElse(stack, links, defs, cWords, rWords):
    """delimits the else part of the statement"""
    stack.extend([0,0])
    links.append(len(stack))

def cThen(stack, links, defs, cWords, rWords):
    """ends an if else then statement -- initializes the statement"""
    top = len(stack)
    mid = links.pop()
    bottom = links.pop()
    stack[bottom - 2] = SYMRBRANCH
    stack[bottom - 1] = mid-bottom
    stack[mid - 2] = SYMRJUMP
    stack[mid - 1] = top-mid
    
