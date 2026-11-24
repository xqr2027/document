import os
name=input("problem: ")
time=input("TimeLimit")
memo=input("MemoryLimit")
mem_kb=stk_kb=memo*1024
optimize="Ofast"
if os.system(f"g++ {name}.cpp -o {name}.exe -{optimize} -static -lm -Wall -std=c++14")!=0:
    exit("Complie Failed")
for file in os.listdir("."):
    if file.endswith(".in") or name not in file:
        continue
    os.system(
        f"""
        -ulimit -t time;
        -ulimit -s mem_kb;
        -ulimit -v mem_kb;
        cp "{file}" "{name}.in";
        time -p ./{name}.exe;
        diff "{name}.out" "{file[:-3]}.out";
        rm -f "{name}.out";
        """
    )
