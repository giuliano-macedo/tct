import re
import numpy as np
from tqdm import tqdm
def read_instance(filename):
	f=open(filename)
	prices=[]
	weights=[]
	n=int(next(f).strip())
	for line in f:
		line=[int(l)for l in re.split(r"[^\d]+",line.strip())]
		if len(line)==3:
			_,price,weight = line
			prices.append(price)
			weights.append(weight)
		else:
			w,=line
	f.close()
	return prices,weights,w
valores,pesos,w=read_instance("../entradas/input12.in")

# print("uint w=%i,n=%i;"%(w,len(pesos)))
# print("uint pesos[%i]=%s;"%(len(pesos),repr(pesos).replace("[","{").replace("]","}")))
# print("uint valores[%i]=%s;"%(len(valores),repr(valores).replace("[","{").replace("]","}")))

# print(w*len(pesos)*4)
n=len(pesos)
mem=np.zeros((n+1)*(w+1),dtype=np.uint32).reshape((n+1,w+1))

def mochiladin(w , n): 
	for i in tqdm(range(n+1)):
		for j in range(w+1):
			if (i==0) or (j==0):
				mem[i][j] = 0; 
			elif (pesos[i-1] <= j):
				mem[i][j] = max(valores[i-1] + mem[i-1][j-pesos[i-1]],  mem[i-1][j]); 
			else:
				mem[i][j] = mem[i-1][j]; 
	return mem[n][w];
print(mochiladin(w,len(pesos)))