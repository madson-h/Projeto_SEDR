############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project ProjetoFinal
set_top Boundary
add_files ProjetoFinal/corpo.cpp
add_files ProjetoFinal/header.h
add_files -tb ProjetoFinal/teste.cpp
open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
#source "./ProjetoFinal/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
