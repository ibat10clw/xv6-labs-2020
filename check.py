import sys
def to_bin(num):
    num = int(num)
    return bin(num)[2:].rjust(9, "0")
filename = 'pgtbl.txt'
f = open(filename, 'r')
data = f.readlines()
f.close()
va = []
vva = []
layer = [0, 0, 0]
while not data[0].startswith("kernel"):
    data.pop(0)
for i in range(len(data)):
    if data[i].startswith("user"):
        break
    if data[i].startswith(".. .. .."):
        L0 = data[i].split(" ")[2][2:-1]
        pa = data[i].split(" ")[-1][:-1]
        last_12 = pa[-3:]
        #print(L2, L1, L0, last_12)
        va.append([L2, L1, L0, last_12])
    elif data[i].startswith(".. .."):
        #print(data[i].split(" "))
        L1 = data[i].split(" ")[1][2:-1]
        #print(L1)
        pass
    elif data[i].startswith(".."):
        #print(data[i].split(" "))
        L2 = data[i].split(" ")[0][2:-1]
j = i + 1
for i in range(j, len(data)):
    if data[i].startswith("init"):
        break
    if data[i].startswith(".. .. .."):
        L0 = data[i].split(" ")[2][2:-1]
        pa = data[i].split(" ")[-1][:-1]
        last_12 = pa[-3:]
        #print(L2, L1, L0, last_12)
        vva.append([L2, L1, L0, last_12])
    elif data[i].startswith(".. .."):
        #print(data[i].split(" "))
        L1 = data[i].split(" ")[1][2:-1]
        #print(L1)
        pass
    elif data[i].startswith(".."):
        #print(data[i].split(" "))
        L2 = data[i].split(" ")[0][2:-1]
    if not data[i].endswith("000\n"):
        print(data[i])

if len(len(va) != len(vva)):
    print("page table size is inconsistent:", len(va), len(vva))
    sys.exit()

# check va
for i in range(len(va)):
    L2, L1, L0, last_12 = va[i]
    kernel_addr = to_bin(L2) + to_bin(L1) + to_bin(L0) + bin(int(last_12))[2:].rjust(12, "0")
    L2, L1, L0, last_12 = vva[i]
    user_addr = to_bin(L2) + to_bin(L1) + to_bin(L0) + bin(int(last_12))[2:].rjust(12, "0")
    if kernel_addr != user_addr:
        print('virtual address is inconsistent:', kernel_addr, user_addr)

f = open(filename, 'r')
data = f.readlines()
f.close()

pa = []
ppa = []
to = pa
for i in range(len(data)):
    if data[i].startswith("init"):
        break
    elif data[i].startswith("user"):
        to = ppa
    if data[i].startswith(".. .. .."):
        to.append((int(data[i].split(" ")[4], 16) >> 10) << 12)

# check pa 
cnt = 0
for i in range(len(pa)):
    if pa[i] != ppa[i]:
        cnt += 1
        print('physical address is inconsistent:', hex(pa[i]), hex(ppa[i]))
print(cnt)