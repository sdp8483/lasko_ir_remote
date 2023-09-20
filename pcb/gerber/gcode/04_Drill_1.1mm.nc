(G-code generated by FlatCAM Evo vUnstable - Version Date: 2022/4/31)

(Name: lasko_ir_remote.drl_cnc)
(Type: G-code from Excellon)
(Units: MM)

(Created on Wednesday, 20 September 2023 at 07:31)

(This preprocessor is the default preprocessor.)
(It is made to work with MACH3 compatible motion controllers.)


(TOOLS DIAMETER: )
(Tool: 3 -> Dia: 1.0998199999999998)

(FEEDRATE Z: )
(Tool: 3 -> Feedrate: 120.0)

(FEEDRATE RAPIDS: )
(Tool: 3 -> Feedrate Rapids: 1500)

(Z_CUT: )
(Tool: 3 -> Z_Cut: -2.5)

(Tools Offset: )
(Tool: 3 -> Offset Z: 0.0)

(Z_MOVE: )
(Tool: 3 -> Z_Move: 2)

(Z Start: None mm)
(Z End: 15 mm)
(X,Y End: 0.0000, 0.0000 mm)
(Steps per circle: 16)
(Preprocessor Excellon: default)

(X range:  -39.4995 ...   -2.9007  mm)
(Y range:  -95.1001 ...   -2.0244  mm)

(Spindle Speed: 13000 RPM)
G21
G90
G94

G01 F120.00
G00 Z2.0000

M03 S13000
G4 P1
G00 X-17.7495 Y-9.7511
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-14.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-19.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-24.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-29.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-34.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-39.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-17.7495 Y-44.2493
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-44.2493
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-39.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-34.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-29.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-24.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-19.7510
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-14.2494
G01 Z-2.5000
G01 Z0
G00 Z2.0000
G00 X-24.2494 Y-9.7511
G01 Z-2.5000
G01 Z0
G00 Z2.0000
M05
G00 Z15.00
G00 X0.0 Y0.0


