#!/usr/bin/env python3
import os
os.chdir("".join(os.path.split(os.path.realpath(__file__))[:-1]))
from utils import *

parser=argparse.ArgumentParser()
parser.add_argument("inputs",type=dirtype,default="../entradas",nargs="?",help="directory with .in files")
parser.add_argument("--grasp_settings",type=str,default="grasp_settings.csv",help="csv with some grasp settings")
parser.add_argument("--tabu_settings",type=str,default="tabu_settings.csv",help="csv with some tabu settings")
parser.add_argument("-f","--force",action="store_true",help="force redo tests")
args=parser.parse_args()


bins=[
	"exact",
	"greedy",
	"grasp",
	"tabu"
]
compile_if_needed(bins)

hooks={b:partial(common_command,b) for b in bins if b not in {"grasp","tabu"}}
grasp_settings=np.genfromtxt(args.grasp_settings,delimiter=",",dtype=np.uint32)
grasp_settings=[grasp_settings] if len(grasp_settings.shape)==1 else grasp_settings
tabu_settings=np.genfromtxt(args.tabu_settings,delimiter=",",dtype=np.uint32)
tabu_settings=[tabu_settings] if len(tabu_settings.shape)==1 else tabu_settings
for window_size,no_it in grasp_settings:
	hooks["grasp,%d,%d"%(window_size,no_it)]=Grasp(window_size,no_it)
for t_size,btmax in tabu_settings:
	hooks["tabu,%d,%d"%(t_size,btmax)]=Tabu(window_size,no_it)

if not os.path.exists("results"):
	os.mkdir("results")
for funcname,func in hooks.items():
	if not os.path.isfile(os.path.join("results",funcname+".csv")) or args.force:
		gen_csv(func,funcname,args.inputs)

Result=namedtuple("results",["label","inputs","rsss","prices","times"])
results={}
for fname in os.listdir("results"):
	stem,extension=os.path.splitext(fname)
	if extension!=".csv":
		continue
	label=stem
	label="exato" if label=="exact" else label
	label="guloso" if label=="greedy" else label
	if "grasp" in stem or "tabu" in stem:
		w,it=tuple(stem.split(",")[1:])
		it=int(it)
		it=str(int(it//1e3))+"K" if it>=1e3 else str(it)
		label=rf"{'grasp' if 'grasp' in stem else 'tabu'} $\alpha=%s,\beta=%s$"%(w,it)
	data=np.genfromtxt(os.path.join("results",fname),delimiter=",",dtype=str,comments=";").T
	inputs=data[0]				#first column as string
	data=data[1:].astype(int)	# rest as int
	results[stem]=Result(label,inputs,*data)
if results.get("exact")!=None:
	latex_gap_mean(results)
	fig_gap(results)
fig_time(results)
fig_mem(results)
plt.show()