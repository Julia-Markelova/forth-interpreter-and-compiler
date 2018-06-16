%define last 0  
           

%macro native 3
section .data
  w_%1: db %2, 0
  dq last 
  db %3             
  %define last w_%1
  xt_%1: dq %1_impl
section .text
  %1_impl:    
%endmacro

%macro colon 3
section .data
  w_%1: db %2, 0
  dq last 
  db %3             
  %define last w_%1
  xt_%1: dq %1_impl
section .text
  %1_impl:  dq docol_impl
%endmacro
