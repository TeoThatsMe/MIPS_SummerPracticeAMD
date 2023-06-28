def getRegVal(reg):
    valRet=0
    if "$t" in reg:
        valRet+=8
        valRet+=int(reg[2])
        if(int(reg[2])>7):
            valRet+=8
    elif "$s" in reg:
        valRet+=16
        valRet+=int(reg[2])
    elif "$ra" in reg:
        valRet+=31
    
    return valRet

f= open("test.asm","r")
output=open("iesire.hex","w")
lines=f.readlines()
linii=[]
for l in lines:
    l=l.replace("    ","")
    #l=l.replace(" ","")
    l=l.replace("\n","")
    #   print(l)
    if l=="" or l.__contains__(':'):
        continue
    linii.append(l)


#print(linii)
for linie in linii:
    p1,p2=linie.split(" ",1)
    p2=p2.replace(" ","")
    p2=p2.split(",")

    instructiune=0

    match p1:
        case "addi":
            instructiune+=(8<<26)
            instructiune+=int(p2[2])
            if(int(p2[2])<0):
                instructiune+=pow(2,16)
            instructiune+=(getRegVal(p2[0])<<16)
            instructiune+=(getRegVal(p2[1])<<21)

        case "lui":
            instructiune+=(15<<26)
            instructiune+=(int(p2[1],0))
            instructiune+=(getRegVal(p2[0])<<16)

        case "sw":
            instructiune+=(43<<26)
            instructiune+=(getRegVal(p2[0])<<16)
            imm,rs=p2[1].split("(")
            rs=rs.replace(")","")
            instructiune+=(getRegVal(rs)<<21)
            instructiune+=int(imm)
            if(int(imm)<0):
                instructiune+=pow(2,16)
        
        case "lw":
            instructiune+=(35<<26)
            instructiune+=(getRegVal(p2[0])<<16)
            imm,rs=p2[1].split("(")
            rs=rs.replace(")","")
            instructiune+=(getRegVal(rs)<<21)
            instructiune+=int(imm)
            if(int(imm)<0):
                instructiune+=pow(2,16)

        case "add":
            instructiune+=32
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
        
        case "xor":
            instructiune+=38
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
        
        case "jr":
            instructiune+=8
            instructiune+=(getRegVal(p2[0])<<21)

    #print(hex(instructiune))


    #print(p2)
    if instructiune!=0:
        print("{0:#010x}".format(instructiune))
        output.write("{0:#010x}".format(instructiune) + "\n")
    else:
        print(linie)
        output.write(linie + "\n")
