import os
import subprocess
from tqdm import tqdm
from multiprocessing import Pool
from functools import partial
from collections import namedtuple
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import argparse

def bash_real_time(command):
	pid,fd = os.forkpty()
	if pid == 0:
		os.execv('/bin/bash',['bash','-c',command])
		exit(0)
	while True:
		try:
			output = os.read(fd,1024).decode("utf-8").rstrip()
		except Exception:
			break
		for line in output.split("\n"):
			yield line
	pid,status,res_usage=os.wait4(pid,0)
	exit_code=os.WEXITSTATUS(status)
	if exit_code!=0:
		raise OSError(exit_code)
def latex_gap_mean(results):
	exact=results.get("exact").prices
	gap=lambda x:(100/len(x))*np.sum(abs(exact-x)/exact)
	ans=[]
	for result in results.values():
		if result.label=="exato":
			continue
		ans.append((result.label,gap(result.prices)))
	ans.sort(key=lambda t:t[1])
	template=r"""
		\begin{table}[]
		\begin{tabular}{|c|c|c|}
		\hline
		Nome do Método & Gap (\%%) \\ \hline
		%s
		\end{tabular}
		\end{table}
	""".replace("\t","")[1:]
	with open(os.path.join("results","gap.tex"),"w") as f:
		table="\\\\ \\hline\n".join(["%s & %s"%(t[0],str(t[1])) for t in ans])
		f.write(template%table)
def fig_gap(results):
	exact=results["exact"]
	data={}
	for result in results.values():
		if result.label=="exato":
			continue
		data[result.label]=100*(abs(exact.prices-result.prices)/exact.prices)
	df=pd.DataFrame(data,index=exact.inputs)
	df.plot.barh()
	plt.xlabel("Gap (%)")
	plt.title("Gráfico de gap para cada instância do guloso e grasp com diferentes parâmetros")
	plt.figure()
	plt.close()
def fig_time(results):
	index=list(results.values())[0].inputs
	df=pd.DataFrame({result.label:result.times for result in results.values()},index=index)
	df.plot.barh()
	plt.xlabel(r"Tempo em $\mu$segundos")
	plt.title("Gráfico de tempo de execução para cada instância de todos os métodos")
	plt.figure()
	plt.close()
def fig_mem(results):
	index=list(results.values())[0].inputs
	df=pd.DataFrame({result.label:result.rsss/1e6 for result in results.values()},index=index)
	df.plot.barh()
	plt.xlabel(r"memória residente máxima em GB")
	plt.title("Gráfico de memória residente máxima para cada instância de todos os métodos")
	plt.figure()
	plt.close()
def gen_csv(func,funcname,inputs):
	print("generating %s.csv"%funcname)
	ans=[]
	pool=Pool(len(os.sched_getaffinity(0))-1)
	for fname,rss,w,time in tqdm(pool.imap(func,inputs),total=len(inputs)):
		ans.append((os.path.split(fname)[-1],rss,w,time))
	ans.sort(key=lambda t:t[0])
	f=open(os.path.join("results",funcname+".csv"),"w")
	f.write(";file_name,rss_kb,price_max,time_microseconds\n")
	for t in ans:
		f.write("%s,%d,%d,%d\n"%t)
	f.close()
def dirtype(path):
	if not os.path.isdir(path):
		raise TypeError("%s is not a directroy"%path)
	ans=[os.path.join(path,p) for p in os.listdir(path)]
	return sorted(ans)
def compile_if_needed(bins):
	if all((os.path.isfile(p) for p in bins)):
		return
	print("compiling...")
	os.chdir("core")
	for line in tqdm(bash_real_time("make"),total=len(bins)+1):pass
	for p in bins:
		os.rename(p,os.path.join("..",p))
	os.chdir("..")
class Grasp:
	def __init__(self,window_size,no_it):
		self.window_size=window_size
		self.no_it=no_it
	def __call__(self,fname):
		return common_command("grasp",fname,str(self.window_size),str(self.no_it))
def common_command(command,*args):
	args=["./"+command]+list(args)
	p = subprocess.Popen(args,stdout=subprocess.PIPE)
	ru = os.wait4(p.pid, 0)[2]
	rss=ru.ru_maxrss
	out=p.stdout.read().decode()
	
	w,time=(int(a) for a in out.split(","))

	return args[1],rss,w,time