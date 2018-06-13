%define last 0  
           

%macro native 2
section .data
  w_%1: db %2, 0
  dq last              
  %define last w_%1
  xt_%1: dq %1_impl
section .text
  %1_impl:    
%endmacro


