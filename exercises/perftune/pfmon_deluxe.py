#!/usr/bin/env python
# pfmon frontend - 2008-2010; Andrzej Nowak, CERN
# supports core2, nehalem, westmere (roughly)

import os, sys, re, getopt, locale, traceback

locale.setlocale(locale.LC_ALL, 'en_US')

re_result = re.compile("[CPU0]*\s*(\d+)\s([\w:]+)")
#pfmon = "/usr/local/bin/pfmon"
pfmon = "/usr/bin/pfmon"
args = ""
timeout = 10
andreas_mode = False

def fprint(arg1, arg2):
    print "%31s: %s" % (arg1, arg2)

cpuinfo = {}
def read_cpuinfo():
    global cpuinfo
    f = open("/proc/cpuinfo")
    lines = f.readlines()
    f.close()
    
    for line in lines:
        tempstr = line.split(":")
        if cpuinfo.has_key(tempstr[0]):
            break
        if len(tempstr) < 2:
            continue
        key = tempstr[0].replace(" ", "_").lower().strip("\n\t")
        val = tempstr[1].strip("\n\t")
        try:
            cpuinfo[key] = int(val)
        except:
            cpuinfo[key] = val
        
class Results(dict):
    def __init__(self):
        dict.__init__(self)
        
    def get(self, arg):
        return float(self[arg])

results = Results()

def analysis_standard(results):
    if results.get("CPU_CLK_UNHALTED:BUS") == 0:
        print "WARNING: CPU_CLK_UNHALTED:BUS is zero - your results might be bogus"
        print "-----------------------------------------------------------------------"

    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    fprint("load instructions %", "%.3f%%" % (results.get("INST_RETIRED:LOADS") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("store instructions %", "%.3f%%" % (results.get("INST_RETIRED:STORES") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("load and store instructions %", "%.3f%%" % ((results.get("INST_RETIRED:STORES") + results.get("INST_RETIRED:LOADS")) / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("resource stalls % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:ANY") / results.get("UNHALTED_CORE_CYCLES") * 100))
    fprint("branch instructions %", "%.3f%%" % (results.get("BRANCH_INSTRUCTIONS_RETIRED") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("% of branch instr. mispredicted", "%.3f%%" % (float(results["MISPREDICTED_BRANCH_RETIRED"])*100 / float(results["BRANCH_INSTRUCTIONS_RETIRED"])))
    fprint("% of l2 loads missed", "%.3f%%" % (results.get("LAST_LEVEL_CACHE_MISSES") / results.get("LAST_LEVEL_CACHE_REFERENCES") * 100))
    fprint("bus utilization %", "%.3f%%" % (results.get("BUS_TRANS_ANY:ALL_AGENTS")*2 / results.get("CPU_CLK_UNHALTED:BUS") * 100))
    fprint("data bus utilization %", "%.3f%%" % (results.get("BUS_DRDY_CLOCKS:ALL_AGENTS") / results.get("CPU_CLK_UNHALTED:BUS") * 100))
    fprint("bus not ready %", "%.3f%%" % (results.get("BUS_BNR_DRV:ALL_AGENTS") * 2 / results.get("CPU_CLK_UNHALTED:BUS") * 100))
    fprint("comp. SIMD instr. ('new FP') %", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE:SCALAR_SINGLE:PACKED_DOUBLE:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("comp. x87 instr. ('old FP') %", "%.3f%%" % (results.get("X87_OPS_RETIRED:ANY") / results.get("INSTRUCTIONS_RETIRED") * 100))

def analysis_standard_nhm(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    fprint("load instructions %", "%.3f%%" % (results.get("MEM_INST_RETIRED:LOADS") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("store instructions %", "%.3f%%" % (results.get("MEM_INST_RETIRED:STORES") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("load and store instructions %", "%.3f%%" % ((results.get("MEM_INST_RETIRED:LOADS") + results.get("MEM_INST_RETIRED:STORES")) / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("resource stalls % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:ANY") / results.get("UNHALTED_CORE_CYCLES") * 100))
    fprint("branch instructions % (approx)", "%.3f%%" % (results.get("BR_INST_EXEC:ANY") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("% of branch instr. mispredicted", "%.3f%%" % (float(results["BR_MISP_EXEC:ANY"])*100 / float(results["BR_INST_EXEC:ANY"])))
    fprint("% of L3 loads missed", "%.3f%%" % (results.get("LAST_LEVEL_CACHE_MISSES") / results.get("LAST_LEVEL_CACHE_REFERENCES") * 100))
#    fprint("bus utilization %", "%.3f%%" % (results.get("BUS_TRANS_ANY:ALL_AGENTS")*2 / results.get("CPU_CLK_UNHALTED:BUS") * 100))
#    fprint("data bus utilization %", "%.3f%%" % (results.get("BUS_DRDY_CLOCKS:ALL_AGENTS") / results.get("CPU_CLK_UNHALTED:BUS") * 100))
#    fprint("bus not ready %", "%.3f%%" % (results.get("BUS_BNR_DRV:ALL_AGENTS") * 2 / results.get("CPU_CLK_UNHALTED:BUS") * 100))
#    fprint("comp. SIMD instr. ('new FP') %", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE:SCALAR_SINGLE:PACKED_DOUBLE:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
    fprint("computational x87 instr. %", "%.3f%%" % (results.get("INST_RETIRED:X87") / results.get("INSTRUCTIONS_RETIRED") * 100))
#    fprint("computational mmx instr. %", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:MMX") / results.get("INSTRUCTIONS_RETIRED") * 100))
#    fprint("computational instr. % (approx)", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:MMX:SSE_DOUBLE_PRECISION:SSE_FP:SSE_FP_PACKED:SSE_FP_SCALAR:SSE_SINGLE_PRECISION:SSE2_INTEGER:X87") / results.get("INSTRUCTIONS_RETIRED") * 100))

def analysis_simd1(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    all_comp = results.get("SIMD_COMP_INST_RETIRED:SCALAR_SINGLE") + \
            results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE") + \
            results.get("SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE") + \
            results.get("SIMD_COMP_INST_RETIRED:PACKED_DOUBLE")
    fprint("all computational SIMD instr.", "%d" % all_comp)
    fprint("computational SIMD instr. %", "%.3f%%" % (all_comp / results.get("INSTRUCTIONS_RETIRED") * 100))
    if all_comp > 0:
        fprint("", "")
        fprint("breakdown", "% of instr	% of comp. SIMD")
        fprint("SCALAR_SINGLE", "%.3f%%		%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_COMP_INST_RETIRED:SCALAR_SINGLE") / all_comp * 100))
        fprint("PACKED_SINGLE", "%.3f%%         %.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE") / all_comp * 100))
        fprint("SCALAR_DOUBLE", "%.3f%%         %.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE") / all_comp * 100))
        fprint("PACKED_DOUBLE", "%.3f%%         %.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_COMP_INST_RETIRED:PACKED_DOUBLE") / all_comp * 100))
    else:
        fprint("SCALAR_SINGLE % (of instr.)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("PACKED_SINGLE % (of instr.)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("SCALAR_DOUBLE % (of instr.)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("PACKED_DOUBLE % (of instr.)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        print "Skipping the rest of ratio analysis due to missing data"

"""    
        fprint("SCALAR_SINGLE % (of comp. SIMD)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_SINGLE") / all_comp * 100))
        fprint("PACKED_SINGLE % (of comp. SIMD)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_SINGLE") / all_comp * 100))
        fprint("SCALAR_DOUBLE % (of comp. SIMD)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE") / all_comp * 100))
        fprint("PACKED_DOUBLE % (of comp. SIMD)", "%.3f%%" % (results.get("SIMD_COMP_INST_RETIRED:PACKED_DOUBLE") / all_comp * 100))
"""

#    "simd1": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION,FP_COMP_OPS_EXE:SSE_SINGLE_PRECISION,FP_COMP_OPS_EXE:SSE_FP_PACKED,FP_COMP_OPS_EXE:SSE_FP_SCALAR\
#    -eFP_COMP_OPS_EXE:SSE_FP,FP_COMP_OPS_EXE:SSE2_INTEGER,FP_COMP_OPS_EXE:MMX,FP_COMP_OPS_EXE:X87 --switch-timeout=%d" % timeout,


def analysis_simd1_nhm(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    all_simd_comp = results.get("FP_COMP_OPS_EXE:SSE_FP") + \
            results.get("FP_COMP_OPS_EXE:SSE2_INTEGER")

    all_comp = all_simd_comp + \
            results.get("FP_COMP_OPS_EXE:MMX") + \
            results.get("FP_COMP_OPS_EXE:X87")

    fprint("all computational SIMD uops.", "%s" % locale.format("%d", all_simd_comp, 1))
    fprint("all computational uops.", "%s" % locale.format("%d", all_comp, 1))
    fprint("% of SIMD in comp. uops.", "%s" % locale.format("%.3f%%", all_simd_comp / all_comp * 100.0, 1))
#    fprint("computational SIMD instr. %", "%.3f%%" % (all_comp / results.get("INSTRUCTIONS_RETIRED") * 100))
    if all_simd_comp > 0:
        fprint("", "")
        fprint("breakdown", "% of comp. SIMD")
        fprint("DOUBLE_PRECISION", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION") / all_simd_comp * 100))
        fprint("SINGLE_PRECISION", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE_SINGLE_PRECISION") / all_simd_comp * 100))
        fprint("PACKED", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE_FP_PACKED") / all_simd_comp * 100))
        fprint("SCALAR", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE_FP_SCALAR") / all_simd_comp * 100))
        fprint("FLOATING POINT", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE_FP") / all_simd_comp * 100))
        fprint("INTEGER", "%.3f%%" % (results.get("FP_COMP_OPS_EXE:SSE2_INTEGER") / all_simd_comp * 100))

def analysis_simd2_nhm(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    all_simd = results.get("SSEX_UOPS_RETIRED:PACKED_DOUBLE") + \
            results.get("SSEX_UOPS_RETIRED:PACKED_SINGLE") + \
            results.get("SSEX_UOPS_RETIRED:SCALAR_DOUBLE") + \
            results.get("SSEX_UOPS_RETIRED:SCALAR_SINGLE") + \
            results.get("SSEX_UOPS_RETIRED:VECTOR_INTEGER")

    all_uops = results.get("UOPS_RETIRED:ANY")
    
    fprint("all SIMD uops", "%s" % locale.format("%d", all_simd, 1))
    fprint("% of SIMD in all uops", "%s" % locale.format("%.3f%%", all_simd / all_uops * 100.0, 1))
    if all_simd > 0:
        fprint("", "")
        fprint("breakdown", "%of all uops   % of all SIMD")
        fprint("PACKED_DOUBLE", "%.3f%%\t\t%.3f%%" % (results.get("SSEX_UOPS_RETIRED:PACKED_DOUBLE") / all_uops * 100, results.get("SSEX_UOPS_RETIRED:PACKED_DOUBLE") / all_simd * 100))
        fprint("PACKED_SINGLE", "%.3f%%\t\t%.3f%%" % (results.get("SSEX_UOPS_RETIRED:PACKED_SINGLE") / all_uops * 100, results.get("SSEX_UOPS_RETIRED:PACKED_SINGLE") / all_simd * 100))
        fprint("SCALAR_DOUBLE", "%.3f%%\t\t%.3f%%" % (results.get("SSEX_UOPS_RETIRED:SCALAR_DOUBLE") / all_uops * 100, results.get("SSEX_UOPS_RETIRED:SCALAR_DOUBLE") / all_simd * 100))
        fprint("SCALAR_SINGLE", "%.3f%%\t\t%.3f%%" % (results.get("SSEX_UOPS_RETIRED:SCALAR_SINGLE") / all_uops * 100, results.get("SSEX_UOPS_RETIRED:SCALAR_SINGLE") / all_simd * 100))
        fprint("VECTOR_INTEGER", "%.3f%%\t\t%.3f%%" % (results.get("SSEX_UOPS_RETIRED:VECTOR_INTEGER") / all_uops * 100, results.get("SSEX_UOPS_RETIRED:VECTOR_INTEGER") / all_simd * 100))

def analysis_simd2(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    fprint("any SIMD instructions %", "%.3f%%" % (results.get("SIMD_INST_RETIRED:ANY") / results.get("INSTRUCTIONS_RETIRED") * 100))
    if results.get("SIMD_INST_RETIRED:ANY") > 0:
        fprint("", "")
        fprint("brekadown", "% of instr.	% of any SIMD")
        fprint("SCALAR_SINGLE", "%.3f%%		%.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_INST_RETIRED:SCALAR_SINGLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("PACKED_SINGLE", "%.3f%%         %.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_INST_RETIRED:PACKED_SINGLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("SCALAR_DOUBLE", "%.3f%%         %.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_INST_RETIRED:SCALAR_DOUBLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("PACKED_DOUBLE", "%.3f%%         %.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_INST_RETIRED:PACKED_DOUBLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("VECTOR int.", "%.3f%%         %.3f%%" % (results.get("SIMD_INST_RETIRED:VECTOR") / results.get("INSTRUCTIONS_RETIRED") * 100, results.get("SIMD_INST_RETIRED:VECTOR") / results.get("SIMD_INST_RETIRED:ANY") * 100))
    else:
        fprint("SCALAR_SINGLE % (of instr.)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("PACKED_SINGLE % (of instr.)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_SINGLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("SCALAR_DOUBLE % (of instr.)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("PACKED_DOUBLE % (of instr.)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_DOUBLE") / results.get("INSTRUCTIONS_RETIRED") * 100))
        fprint("VECTOR int. % (of instr.)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:VECTOR") / results.get("INSTRUCTIONS_RETIRED") * 100))
        print "Skipping the rest of ratio analysis due to missing data (not enough SIMD instructions?)"
"""
        fprint("SCALAR_SINGLE % (of any SIMD)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_SINGLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("PACKED_SINGLE % (of any SIMD)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_SINGLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("SCALAR_DOUBLE % (of any SIMD)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:SCALAR_DOUBLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("PACKED_DOUBLE % (of any SIMD)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:PACKED_DOUBLE") / results.get("SIMD_INST_RETIRED:ANY") * 100))
        fprint("VECTOR int. % (of any SIMD)", "%.3f%%" % (results.get("SIMD_INST_RETIRED:VECTOR") / results.get("SIMD_INST_RETIRED:ANY") * 100))
"""

def analysis_simd_uop(results):
    pass
    
def analysis_simd_maxi(results):
    pass
    
def analysis_uops(results):
    pass
    
def analysis_stalls(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    fprint("", "")
    fprint("breakdown", "% of cycles	% of stalls")
    fprint("resource stalls", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:ANY") / results.get("UNHALTED_CORE_CYCLES") * 100, 100.))
    fprint("ROB full", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:ROB_FULL") / results.get("UNHALTED_CORE_CYCLES") * 100, results.get("RESOURCE_STALLS:ROB_FULL") / results.get("RESOURCE_STALLS:ANY") * 100))
    fprint("RS full", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:RS_FULL") / results.get("UNHALTED_CORE_CYCLES") * 100, results.get("RESOURCE_STALLS:RS_FULL") / results.get("RESOURCE_STALLS:ANY") * 100))  
    fprint("LD or ST", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:LD_ST") / results.get("UNHALTED_CORE_CYCLES") * 100, results.get("RESOURCE_STALLS:LD_ST") / results.get("RESOURCE_STALLS:ANY") * 100)) 
    fprint("FP ctrl word", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:FPCW") / results.get("UNHALTED_CORE_CYCLES") * 100, results.get("RESOURCE_STALLS:FPCW") / results.get("RESOURCE_STALLS:ANY") * 100))
    fprint("BR mispredict", "%.3f%%		%.3f%%" % (results.get("RESOURCE_STALLS:BR_MISS_CLEAR") / results.get("UNHALTED_CORE_CYCLES") * 100, results.get("RESOURCE_STALLS:BR_MISS_CLEAR") / results.get("RESOURCE_STALLS:ANY") * 100))

"""
    fprint("resource stalls % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:ANY") / results.get("UNHALTED_CORE_CYCLES") * 100))        
    fprint("ROB full % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:ROB_FULL") / results.get("UNHALTED_CORE_CYCLES") * 100))
    fprint("RS full % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:RS_FULL") / results.get("UNHALTED_CORE_CYCLES") * 100))  
    fprint("LD or ST % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:LD_ST") / results.get("UNHALTED_CORE_CYCLES") * 100)) 
    fprint("FP ctrl word % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:FPCW") / results.get("UNHALTED_CORE_CYCLES") * 100))
    fprint("BR mispredict % (of cycles)", "%.3f%%" % (results.get("RESOURCE_STALLS:BR_MISS_CLEAR") / results.get("UNHALTED_CORE_CYCLES") * 100))
    fprint("ROB full % (of Res. stalls)", "%.3f%%" % (results.get("RESOURCE_STALLS:ROB_FULL") / results.get("RESOURCE_STALLS:ANY") * 100))
    fprint("RS full % (of Res. stalls)", "%.3f%%" % (results.get("RESOURCE_STALLS:RS_FULL") / results.get("RESOURCE_STALLS:ANY") * 100))  
    fprint("LD or ST % (of Res. stalls)", "%.3f%%" % (results.get("RESOURCE_STALLS:LD_ST") / results.get("RESOURCE_STALLS:ANY") * 100)) 
    fprint("FP ctrl word % (of Res. stalls)", "%.3f%%" % (results.get("RESOURCE_STALLS:FPCW") / results.get("RESOURCE_STALLS:ANY") * 100))
    fprint("BR mispred. % (of Res. stalls)", "%.3f%%" % (results.get("RESOURCE_STALLS:BR_MISS_CLEAR") / results.get("RESOURCE_STALLS:ANY") * 100))
"""

def analysis_cache(results):
    pass

def analysis_cycle(results):
    pass

def analysis_uncore_nhm(results):
    pass
    
def analysis_cycle_nhm(results):
    fprint("CPI", "%.4f" % (results.get("UNHALTED_CORE_CYCLES") / results.get("INSTRUCTIONS_RETIRED")))
    fprint("", "")
    active_cycles = results.get("UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5")
    stalled_cycles = results.get("UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5--1")
    resource_stalls = results.get("RESOURCE_STALLS:ANY")
    uop_stalls = results.get("UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5--2")
    
    fprint("Active cycles", "%s (%.1f%% of cycles)" % (locale.format("%d", int(active_cycles), True), active_cycles*100.0/(active_cycles+stalled_cycles)))
    fprint("Stalled cycles", "%s (%.1f%% of cycles)" % (locale.format("%d", int(stalled_cycles), True), stalled_cycles*100.0/(active_cycles+stalled_cycles)))
    fprint("Sum of all resource stalls", "%s" % locale.format("%d", int(resource_stalls), True))
    fprint("Number of uop stalls", "%s" % locale.format("%d", int(uop_stalls), True))
    fprint("Average stall length", "%d cycles" % (stalled_cycles / uop_stalls))
    
analysis_tbl_penryn = {
    "standard": analysis_standard,
    "simd1": analysis_simd1,
    "simd2": analysis_simd2,
    "simd_uop": analysis_simd_uop,
    "simd_maxi": analysis_simd_maxi,
    "uops": analysis_uops,
    "stalls": analysis_stalls,
    "cache": analysis_cache,
    "cycle": analysis_cycle        
}

analysis_tbl_nehalem = {
    "standard": analysis_standard_nhm,
    "simd1": analysis_simd1_nhm,
    "simd2": analysis_simd2_nhm,
#    "simd_uop": analysis_simd_uop,
#    "simd_maxi": analysis_simd_maxi,
#    "uops": analysis_uops,
#    "stalls": analysis_stalls,
#    "cache": analysis_cache
    "uncore": analysis_uncore_nhm,
    "cycle": analysis_cycle_nhm
}

print ""
print "Pfmon deluxe v.5.0 (Westmere ready)"
print "A simple script wrapping pfmon in pink paper. Comments to Andrzej Nowak (CERN)"
print ""

read_cpuinfo()

# startup - determine if we're in stdin mode
stdin_mode = 0
if sys.argv[0].count("stdin") == 0:
    stdin_mode = 0
else:
    stdin_mode = 1

# get options
focus = "standard"
extra = ""

del sys.argv[0]
optlist, arglist = getopt.getopt(sys.argv[0:], "af:t:e:h", ["andreas-mode", "focus=", "timeout=", "extra=", "help"])

for o, a in optlist:
    if o in ("-f", "--focus"):
        print "Focus: %s" % a
        focus = a
    
    elif o in ("-t", "--timeout"):
        print "Setting timeout to %d ms" % int(a)
        timeout = int(a)
        
    elif o in ("-h", "--help"):
        print "what do you mean, 'help'?"
        print "Usage: ./pfmon_deluxe.py [-a] [-f focus (standard/simd1/simd2/simd_uop/stalls/cache/uncore/cycle)] [-t switch-timeout] -e \"additional pfmon options and binary\""
        print "  -a: andreas mode (don't use)"
        print "  -f: focus on a certain group of events"
        print "  -t: multiplexing switch timeout in ms"
        sys.exit(0)
    
    elif o in ("-e", "--extra"):
        extra = a
        
    elif o in ("-a", "--andreas"):
        andreas_mode = True

for a in arglist:
    extra += " " + a

setup_tbl_penryn = {
    "standard": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,BRANCH_INSTRUCTIONS_RETIRED,MISPREDICTED_BRANCH_RETIRED -eINST_RETIRED:LOADS,SIMD_COMP_INST_RETIRED:PACKED_SINGLE:SCALAR_SINGLE:PACKED_DOUBLE:SCALAR_DOUBLE -eINST_RETIRED:STORES,X87_OPS_RETIRED:ANY -eRESOURCE_STALLS:ANY,BUS_TRANS_ANY:ALL_AGENTS -eBUS_DRDY_CLOCKS:ALL_AGENTS,BUS_BNR_DRV:ALL_AGENTS -eLAST_LEVEL_CACHE_REFERENCES,LAST_LEVEL_CACHE_MISSES -eCPU_CLK_UNHALTED:BUS --switch-timeout=%d" % timeout,
    "simd1": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,SIMD_COMP_INST_RETIRED:PACKED_SINGLE,SIMD_COMP_INST_RETIRED:SCALAR_SINGLE -eSIMD_COMP_INST_RETIRED:PACKED_DOUBLE,SIMD_COMP_INST_RETIRED:SCALAR_DOUBLE --switch-timeout=%d" % timeout,
    "simd2": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,SIMD_INST_RETIRED:PACKED_SINGLE,SIMD_INST_RETIRED:SCALAR_SINGLE -eSIMD_INST_RETIRED:PACKED_DOUBLE,SIMD_INST_RETIRED:SCALAR_DOUBLE -eSIMD_INST_RETIRED:VECTOR,SIMD_INST_RETIRED:ANY --switch-timeout=%d" % timeout,
    "simd_uop": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,SIMD_UOP_TYPE_EXEC:MUL,SIMD_UOP_TYPE_EXEC:SHIFT -eSIMD_UOP_TYPE_EXEC:PACK,SIMD_UOP_TYPE_EXEC:UNPACK -eSIMD_UOP_TYPE_EXEC:LOGICAL,SIMD_UOP_TYPE_EXEC:ARITHMETIC --switch-timeout=%d" % timeout,
    "simd_maxi": "--with-header --aggregate-results --follow-all -eUNIMPLEMENTED --switch-timeout=%d" % timeout,
    "uops": "--with-header --aggregate-results --follow-all -eUNIMPLEMENTED --switch-timeout=%d" % timeout,
    "stalls": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,RESOURCE_STALLS:ROB_FULL,RESOURCE_STALLS:RS_FULL -eRESOURCE_STALLS:LD_ST,RESOURCE_STALLS:FPCW -eRESOURCE_STALLS:BR_MISS_CLEAR,RESOURCE_STALLS:ANY --switch-timeout=%d" % timeout,
    "cache": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,LAST_LEVEL_CACHE_REFERENCES,LAST_LEVEL_CACHE_MISSES -eL2_LINES_IN:ANY,L2_LINES_OUT:ANY -eMACHINE_NUKES:MEM_ORDER,L2_NO_REQ:BOTH_CORES -eMEM_LOAD_RETIRED:L2_MISS,MEM_LOAD_RETIRED:L2_LINE_MISS --switch-timeout=%d" % timeout
}

_setup_stall_cycles="UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5"

setup_tbl_nehalem = {
    "standard": "--with-header --aggregate-results --follow-all  -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,BR_INST_EXEC:ANY,BR_MISP_EXEC:ANY,INST_RETIRED:X87,RESOURCE_STALLS:ANY\
     -eLAST_LEVEL_CACHE_REFERENCES,LAST_LEVEL_CACHE_MISSES,MEM_INST_RETIRED:LOADS,MEM_INST_RETIRED:STORES --switch-timeout=%d" % timeout,
#     -eRESOURCE_STALLS:ANY,FP_COMP_OPS_EXE:MMX:SSE_DOUBLE_PRECISION:SSE_FP:SSE_FP_PACKED:SSE_FP_SCALAR:SSE_SINGLE_PRECISION:SSE2_INTEGER:X87\
# missing: -eBUS_DRDY_CLOCKS:ALL_AGENTS,BUS_BNR_DRV:ALL_AGENTS -eCPU_CLK_UNHALTED:BUS BUS_TRANS_ANY:ALL_AGENTS

#    -eFP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION:SSE_FP_PACKED,FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION:SSE_FP_SCALAR,FP_COMP_OPS_EXE:SSE_SINGLE_PRECISION:SSE_FP_PACKED,FP_COMP_OPS_EXE:SSE_SINGLE_PRECISION:SSE_FP_SCALAR\
    "simd1": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,FP_COMP_OPS_EXE:SSE_DOUBLE_PRECISION,FP_COMP_OPS_EXE:SSE_SINGLE_PRECISION,FP_COMP_OPS_EXE:SSE_FP_PACKED,FP_COMP_OPS_EXE:SSE_FP_SCALAR\
    -eFP_COMP_OPS_EXE:SSE_FP,FP_COMP_OPS_EXE:SSE2_INTEGER,FP_COMP_OPS_EXE:MMX,FP_COMP_OPS_EXE:X87 --switch-timeout=%d" % timeout,

    "simd2": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,UOPS_RETIRED:ANY,SSEX_UOPS_RETIRED:VECTOR_INTEGER -eSSEX_UOPS_RETIRED:PACKED_DOUBLE,SSEX_UOPS_RETIRED:PACKED_SINGLE,SSEX_UOPS_RETIRED:SCALAR_DOUBLE,SSEX_UOPS_RETIRED:SCALAR_SINGLE --switch-timeout=%d" % timeout,
    
    "stalls": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,RESOURCE_STALLS:LOAD,RESOURCE_STALLS:STORE,RESOURCE_STALLS:RS_FULL,RESOURCE_STALLS:ROB_FULL -eRESOURCE_STALLS:FPCW,RESOURCE_STALLS:MXCSR,RESOURCE_STALLS:OTHER,RESOURCE_STALLS:ANY --switch-timeout=%d" % timeout,

    "cache": "--with-header --aggregate-results --follow-all -e LAST_LEVEL_CACHE_MISSES,LAST_LEVEL_CACHE_REFERENCES,CACHE_LOCK_CYCLES:L1D,CACHE_LOCK_CYCLES:L1D_L2 -e L2_LINES_IN:ANY,L2_LINES_OUT:ANY,LONGEST_LAT_CACHE:REFERENCE,LONGEST_LAT_CACHE:MISS -e MEM_UNCORE_RETIRED:OTHER_CORE_L2_HITM,MEM_UNCORE_RETIRED:REMOTE_CACHE_LOCAL_HOME_HIT,MEM_UNCORE_RETIRED:REMOTE_DRAM,MEM_UNCORE_RETIRED:LOCAL_DRAM -e OFFCORE_REQUESTS:ANY,OFFCORE_REQUESTS:ANY_READ,OFFCORE_REQUESTS:DEMAND_READ_DATA,PARTIAL_ADDRESS_ALIAS -e MEM_LOAD_RETIRED:LLC_MISS,MEM_UNCORE_RETIRED:UNCACHEABLE,L2_TRANSACTIONS:ANY,IO_TRANSACTIONS --switch-timeout=%d" % timeout,
    "datasource": "?", # p23 in levinthal's presentation
    "uncore": "--with-header --aggregate-results --follow-all --system-wide -eUNC_LLC_HITS:ANY,UNC_LLC_HITS:READ,UNC_LLC_HITS:WRITE,UNC_LLC_HITS:PROBE,UNC_LLC_MISS:ANY,UNC_LLC_MISS:READ,UNC_LLC_MISS:WRITE,UNC_LLC_MISS:PROBE --switch-timeout=%d" % timeout,

    # core cycles, instructions retired, cycles >=1 uops executed, cycles 0 uops executed
    "cycle": "--with-header --aggregate-results --follow-all -eUNHALTED_CORE_CYCLES,INSTRUCTIONS_RETIRED,RESOURCE_STALLS:ANY,UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5,UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5,UOPS_EXECUTED:PORT0:PORT1:PORT2_CORE:PORT3_CORE:PORT4_CORE:PORT5 --counter-mask=0,0,0,1,1,1 --inv-mask=0,0,0,0,1,1 --edge-mask=0,0,0,0,0,1 --anythread-mask=0,0,0,1,1,1 --switch-timeout=%d" % timeout,
    "uop_flow": "?"
}

#print cpuinfo

# choose the right CPU... it's getting crowded in here
if cpuinfo['model'] == 26 or cpuinfo['model'] == 37 or cpuinfo['model'] == 44:
    print "Setting up for Nehalem/Westmere"
    setup_tbl = setup_tbl_nehalem
    analysis_tbl = analysis_tbl_nehalem

else:
#if cpuinfo['model'] <= 25 or cpuinfo['model'] >= 27:
    print "Setting up for Penryn and friends"
    setup_tbl = setup_tbl_penryn
    analysis_tbl = analysis_tbl_penryn


try:
    setup = setup_tbl[focus]
except:
    print "Unknown focus. Exiting"
    sys.exit(0)

if stdin_mode == 0:
    print "Running %s with arguments: %s" % (pfmon, extra)
    cmd = pfmon + " " + setup + " " + extra
    print "Command line: %s" % cmd
    no_thing___, p = os.popen4(cmd)
else:
    p = sys.stdin

print "-----------------------------------------------------------------------"

resultlines = []
duplicate_counter = 1

for line in p.readlines():
    line = line.strip("\n")
    result = re_result.match(line)
#    print line
    if result:
        if not results.has_key(result.group(2)):
            results[result.group(2)] = result.group(1)
        else:
            results[result.group(2) + "--%d" % duplicate_counter] = result.group(1)
            duplicate_counter += 1
            
        resultlines.append(line)
        print locale.format("%d", int(result.group(1)), True).rjust(20), result.group(2)
    else:
        print line

if andreas_mode == True:
    print "-----------------------------------------------------------------------"
    print "Formatted results:"

    if 0:
        for key, value in results.iteritems():
            print "%17s %s" % (value, key.replace(":", "\n" + 19*" " + "-"))

    if 1:
        maxx = 0
        for k, v in results.iteritems():
            if len(str(v)) > maxx:
                maxx = len(str(v))
                
        for line in resultlines:
            if len(line) < 50:
                print line[:maxx] + " " + line[maxx:]
            else:
                idx = line.find(":", 50)
                line = line[:idx] + "\n  " + maxx * " " + line[idx:]
                print line[:maxx] + " " + line[maxx:]            

#if focus != None and focus != "standard":
#    sys.exit(0)
    
print "-----------------------------------------------------------------------"
print "Ratios:"

try:
    analysis_tbl[focus](results)
except:
    print "\n! Alert, captain! Bogus results. Results processing interrupted due to an exception."
    print sys.exc_info()[0:2]
    traceback.print_exc()
print ""

# rate my CPI
