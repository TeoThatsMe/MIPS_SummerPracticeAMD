import numpy as np

def getRegVal(reg):
    valRet=0
    if "$a" in reg:
        valRet+=4
        valRet+=int(reg[2])
    elif "$v" in reg:
        valRet+=2
        valRet+=int(reg[2])
    elif "$t" in reg:
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
 
f=open("test.asm","r")
output=open("iesire.hex","w")
lines=f.readlines()
linii=[]
etichete={}
PC=0
for l in lines:
    l=l.replace("    ","")
    #l=l.replace(" ","")
    l=l.replace("\n","")
    #   print(l)
    if l=="":
        continue
    if l.__contains__(':'):
        #print(PC)
        temp=l.split(":")
        etichete[temp[0]]=PC
        if len(temp[1])==0:
            continue
        l=temp[1].replace("    ","")
        l=l.replace("\n","")
        l=l.replace(" ","",1)
        linii.append(l)
        PC+=1
        continue
    linii.append(l)
    PC+=1
 
print(etichete)
 
 
counter=1
#print(linii)
for linie in linii:
    p1,p2=linie.split(" ",1)
    p2=p2.replace(" ","")
    p2=p2.split(",")
 
    instructiune=0
 
    match p1:
 
        #R type
 
        case "add":
            instructiune+=32
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
        case "sub":
            instructiune+=34
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
        case "and":
            instructiune+=36
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
        case "or":
            instructiune+=37
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
        case "xor":
            instructiune+=38
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
        case "slt":
            instructiune+=42
            instructiune+=(getRegVal(p2[0])<<11)
            instructiune+=(getRegVal(p2[1])<<21)
            instructiune+=(getRegVal(p2[2])<<16)
 
 
        #I Type
 
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
 
        case "beq":
            instructiune+=4<<26
            instructiune+=(getRegVal(p2[0])<<21)
            instructiune+=(getRegVal(p2[1])<<16)
            adresa=etichete[p2[2]]
            adresa=adresa-counter
            #print(adresa)
            if adresa<0:
                instructiune+=65536
            instructiune+=adresa
 
 
        #J Type
 
        case "j":
            instructiune+=2<<26
            adresa=etichete[p2[0]]
            instructiune+=adresa
            #print(adresa)
    counter+=1
 
    #print(hex(instructiune))
 
 
    #print(p2)
    if instructiune!=0:
        print("{0:08x}".format(instructiune))
        output.write("{0:08x}".format(instructiune) + "\n")
    else:
        print(linie)
        output.write(linie + "\n")