import sys
from forthglobals import *
from worddefs import *
cWords = {":":cColon, ";":cSemiColon, "if":cIf, "else":cElse, "then":cThen}
rWords = {}
rMake = {"+":rAdd, ".":rDot, "getInt":rGetInt}
rWordDefs = []
for k in rMake:
    v = rMake[k]
    rWords[k] = [len(rWordDefs)]
    rWordDefs.append(v)
defs = []


def compile(source):
    """takes in a iterable which returns the separated words
    and compiles it into bytecode"""
    nums = set("1234567890")
    stack = []
    links = []
    for word in source:
        cVal = cWords.get(word, None)
        rVal = rWords.get(word, None)
        if cVal != None:
            if type(cVal) == int:
                interpC(stack, links, defs, cWords, rWords)
            else:
                cVal(stack, links, defs, cWords, rWords)
        elif rVal != None:
            if type(rVal) == int:
                stack.append(SYMRUCALL)
                stack.append(rVal)            
            else:
                stack.append(SYMRCALL)
                stack.append(rVal[0])
        elif len(word) >= 2 and word[0] == "0" and not (word[1] >= "0" and word[1] <= "9"):
            stack.append(SYMINT)
            if word[1] == "x":
                stack.append(int(word[2:],16))
            elif word[1] == "o":
                stack.append(int(word[2:],8))
            elif word[1] == "b":
                stack.append(int(word[2:],2))
        elif len(word) >= 1 and word[0] in nums:
            stack.append(SYMINT)
            stack.append(int(word))
        else:
            stack.append(SYMSTR)
            stack.append(word)
    #print("stack:", stack)
    #print("defnames:", rWords)
    #print("defs:", defs)
    return [len(defs)] + defs + stack

def interp(dataspace):
    """Takes a bytecode compiled forth program definition <dataspace>
    forth program definition structure described in the README"""
    stack = []
    links = []
    i = dataspace[0]
    DATASTART = 1
    i += DATASTART
    ldata = len(dataspace)
    #print("dataspace:", dataspace)
    while i < ldata:
        sym = dataspace[i]
        #print(i, sym)
        i += 1
        if sym == SYMHALT:
            break
        elif sym == SYMRUCALL:
            ind = dataspace[i]
            i += 1
            links.append(i)
            i = ind + DATASTART
        elif sym == SYMRCALL:
            fun = dataspace[i]
            i += 1
            rWordDefs[fun](stack, links)
        elif sym == SYMCUCALL:
            pass
        elif sym == SYMCCALL:
            pass
        elif sym == SYMINT:
            d = dataspace[i]
            i += 1
            stack.append(d)
        elif sym == SYMSTR:
            d = dataspace[i]
            i += 1
            stack.append(d)
        elif sym == SYMRET:
            i = links.pop()
        elif sym == SYMRBRANCH:
            jmp = dataspace[i]
            i += 1
            cond = stack.pop()
            if cond == 0:
                i += jmp
        elif sym == SYMRJUMP:
            jmp = dataspace[i]
            i += 1
            i += jmp
        else:
            print("err")
            break
        #print("cur:",stack)
    #print("output:", stack)
    
compiled = compile(": THREE getInt if NOTZERO else ZERO then ; THREE .".split(" "))#sys.stdin
interp(compiled)
#compile(sys.stdin)
