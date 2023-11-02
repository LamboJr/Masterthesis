// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.1 (lin64) Build 3865809 Sun May  7 15:04:56 MDT 2023
// Date        : Thu Nov  2 18:13:28 2023
// Host        : Acer running 64-bit Ubuntu 22.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/daniel/Masterthesis/GB_MASTER_UNIT/GB_MASTER_UNIT.gen/sources_1/ip/blk_mem_gen_0/blk_mem_gen_0_sim_netlist.v
// Design      : blk_mem_gen_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z020clg400-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "blk_mem_gen_0,blk_mem_gen_v8_4_6,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "blk_mem_gen_v8_4_6,Vivado 2023.1" *) 
(* NotValidForBitStream *)
module blk_mem_gen_0
   (clka,
    ena,
    wea,
    addra,
    dina,
    douta,
    clkb,
    enb,
    web,
    addrb,
    dinb,
    doutb);
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME BRAM_PORTA, MEM_SIZE 8192, MEM_WIDTH 32, MEM_ECC NONE, MASTER_TYPE OTHER, READ_LATENCY 1" *) input clka;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA EN" *) input ena;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA WE" *) input [0:0]wea;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA ADDR" *) input [10:0]addra;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA DIN" *) input [31:0]dina;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTA DOUT" *) output [31:0]douta;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB CLK" *) (* x_interface_parameter = "XIL_INTERFACENAME BRAM_PORTB, MEM_SIZE 8192, MEM_WIDTH 32, MEM_ECC NONE, MASTER_TYPE OTHER, READ_LATENCY 1" *) input clkb;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB EN" *) input enb;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB WE" *) input [0:0]web;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB ADDR" *) input [10:0]addrb;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB DIN" *) input [31:0]dinb;
  (* x_interface_info = "xilinx.com:interface:bram:1.0 BRAM_PORTB DOUT" *) output [31:0]doutb;

  wire [10:0]addra;
  wire [10:0]addrb;
  wire clka;
  wire clkb;
  wire [31:0]dina;
  wire [31:0]dinb;
  wire [31:0]douta;
  wire [31:0]doutb;
  wire ena;
  wire enb;
  wire [0:0]wea;
  wire [0:0]web;
  wire NLW_U0_dbiterr_UNCONNECTED;
  wire NLW_U0_rsta_busy_UNCONNECTED;
  wire NLW_U0_rstb_busy_UNCONNECTED;
  wire NLW_U0_s_axi_arready_UNCONNECTED;
  wire NLW_U0_s_axi_awready_UNCONNECTED;
  wire NLW_U0_s_axi_bvalid_UNCONNECTED;
  wire NLW_U0_s_axi_dbiterr_UNCONNECTED;
  wire NLW_U0_s_axi_rlast_UNCONNECTED;
  wire NLW_U0_s_axi_rvalid_UNCONNECTED;
  wire NLW_U0_s_axi_sbiterr_UNCONNECTED;
  wire NLW_U0_s_axi_wready_UNCONNECTED;
  wire NLW_U0_sbiterr_UNCONNECTED;
  wire [10:0]NLW_U0_rdaddrecc_UNCONNECTED;
  wire [3:0]NLW_U0_s_axi_bid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_bresp_UNCONNECTED;
  wire [10:0]NLW_U0_s_axi_rdaddrecc_UNCONNECTED;
  wire [31:0]NLW_U0_s_axi_rdata_UNCONNECTED;
  wire [3:0]NLW_U0_s_axi_rid_UNCONNECTED;
  wire [1:0]NLW_U0_s_axi_rresp_UNCONNECTED;

  (* C_ADDRA_WIDTH = "11" *) 
  (* C_ADDRB_WIDTH = "11" *) 
  (* C_ALGORITHM = "1" *) 
  (* C_AXI_ID_WIDTH = "4" *) 
  (* C_AXI_SLAVE_TYPE = "0" *) 
  (* C_AXI_TYPE = "1" *) 
  (* C_BYTE_SIZE = "9" *) 
  (* C_COMMON_CLK = "0" *) 
  (* C_COUNT_18K_BRAM = "0" *) 
  (* C_COUNT_36K_BRAM = "2" *) 
  (* C_CTRL_ECC_ALGO = "NONE" *) 
  (* C_DEFAULT_DATA = "0" *) 
  (* C_DISABLE_WARN_BHV_COLL = "0" *) 
  (* C_DISABLE_WARN_BHV_RANGE = "0" *) 
  (* C_ELABORATION_DIR = "./" *) 
  (* C_ENABLE_32BIT_ADDRESS = "0" *) 
  (* C_EN_DEEPSLEEP_PIN = "0" *) 
  (* C_EN_ECC_PIPE = "0" *) 
  (* C_EN_RDADDRA_CHG = "0" *) 
  (* C_EN_RDADDRB_CHG = "0" *) 
  (* C_EN_SAFETY_CKT = "0" *) 
  (* C_EN_SHUTDOWN_PIN = "0" *) 
  (* C_EN_SLEEP_PIN = "0" *) 
  (* C_EST_POWER_SUMMARY = "Estimated Power for IP     :     10.698 mW" *) 
  (* C_FAMILY = "zynq" *) 
  (* C_HAS_AXI_ID = "0" *) 
  (* C_HAS_ENA = "1" *) 
  (* C_HAS_ENB = "1" *) 
  (* C_HAS_INJECTERR = "0" *) 
  (* C_HAS_MEM_OUTPUT_REGS_A = "0" *) 
  (* C_HAS_MEM_OUTPUT_REGS_B = "0" *) 
  (* C_HAS_MUX_OUTPUT_REGS_A = "0" *) 
  (* C_HAS_MUX_OUTPUT_REGS_B = "0" *) 
  (* C_HAS_REGCEA = "0" *) 
  (* C_HAS_REGCEB = "0" *) 
  (* C_HAS_RSTA = "0" *) 
  (* C_HAS_RSTB = "0" *) 
  (* C_HAS_SOFTECC_INPUT_REGS_A = "0" *) 
  (* C_HAS_SOFTECC_OUTPUT_REGS_B = "0" *) 
  (* C_INITA_VAL = "0" *) 
  (* C_INITB_VAL = "0" *) 
  (* C_INIT_FILE = "blk_mem_gen_0.mem" *) 
  (* C_INIT_FILE_NAME = "no_coe_file_loaded" *) 
  (* C_INTERFACE_TYPE = "0" *) 
  (* C_LOAD_INIT_FILE = "0" *) 
  (* C_MEM_TYPE = "2" *) 
  (* C_MUX_PIPELINE_STAGES = "0" *) 
  (* C_PRIM_TYPE = "1" *) 
  (* C_READ_DEPTH_A = "2048" *) 
  (* C_READ_DEPTH_B = "2048" *) 
  (* C_READ_LATENCY_A = "1" *) 
  (* C_READ_LATENCY_B = "1" *) 
  (* C_READ_WIDTH_A = "32" *) 
  (* C_READ_WIDTH_B = "32" *) 
  (* C_RSTRAM_A = "0" *) 
  (* C_RSTRAM_B = "0" *) 
  (* C_RST_PRIORITY_A = "CE" *) 
  (* C_RST_PRIORITY_B = "CE" *) 
  (* C_SIM_COLLISION_CHECK = "ALL" *) 
  (* C_USE_BRAM_BLOCK = "0" *) 
  (* C_USE_BYTE_WEA = "0" *) 
  (* C_USE_BYTE_WEB = "0" *) 
  (* C_USE_DEFAULT_DATA = "0" *) 
  (* C_USE_ECC = "0" *) 
  (* C_USE_SOFTECC = "0" *) 
  (* C_USE_URAM = "0" *) 
  (* C_WEA_WIDTH = "1" *) 
  (* C_WEB_WIDTH = "1" *) 
  (* C_WRITE_DEPTH_A = "2048" *) 
  (* C_WRITE_DEPTH_B = "2048" *) 
  (* C_WRITE_MODE_A = "WRITE_FIRST" *) 
  (* C_WRITE_MODE_B = "WRITE_FIRST" *) 
  (* C_WRITE_WIDTH_A = "32" *) 
  (* C_WRITE_WIDTH_B = "32" *) 
  (* C_XDEVICEFAMILY = "zynq" *) 
  (* downgradeipidentifiedwarnings = "yes" *) 
  (* is_du_within_envelope = "true" *) 
  blk_mem_gen_0_blk_mem_gen_v8_4_6 U0
       (.addra(addra),
        .addrb(addrb),
        .clka(clka),
        .clkb(clkb),
        .dbiterr(NLW_U0_dbiterr_UNCONNECTED),
        .deepsleep(1'b0),
        .dina(dina),
        .dinb(dinb),
        .douta(douta),
        .doutb(doutb),
        .eccpipece(1'b0),
        .ena(ena),
        .enb(enb),
        .injectdbiterr(1'b0),
        .injectsbiterr(1'b0),
        .rdaddrecc(NLW_U0_rdaddrecc_UNCONNECTED[10:0]),
        .regcea(1'b0),
        .regceb(1'b0),
        .rsta(1'b0),
        .rsta_busy(NLW_U0_rsta_busy_UNCONNECTED),
        .rstb(1'b0),
        .rstb_busy(NLW_U0_rstb_busy_UNCONNECTED),
        .s_aclk(1'b0),
        .s_aresetn(1'b0),
        .s_axi_araddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arburst({1'b0,1'b0}),
        .s_axi_arid({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_arready(NLW_U0_s_axi_arready_UNCONNECTED),
        .s_axi_arsize({1'b0,1'b0,1'b0}),
        .s_axi_arvalid(1'b0),
        .s_axi_awaddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awburst({1'b0,1'b0}),
        .s_axi_awid({1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_awready(NLW_U0_s_axi_awready_UNCONNECTED),
        .s_axi_awsize({1'b0,1'b0,1'b0}),
        .s_axi_awvalid(1'b0),
        .s_axi_bid(NLW_U0_s_axi_bid_UNCONNECTED[3:0]),
        .s_axi_bready(1'b0),
        .s_axi_bresp(NLW_U0_s_axi_bresp_UNCONNECTED[1:0]),
        .s_axi_bvalid(NLW_U0_s_axi_bvalid_UNCONNECTED),
        .s_axi_dbiterr(NLW_U0_s_axi_dbiterr_UNCONNECTED),
        .s_axi_injectdbiterr(1'b0),
        .s_axi_injectsbiterr(1'b0),
        .s_axi_rdaddrecc(NLW_U0_s_axi_rdaddrecc_UNCONNECTED[10:0]),
        .s_axi_rdata(NLW_U0_s_axi_rdata_UNCONNECTED[31:0]),
        .s_axi_rid(NLW_U0_s_axi_rid_UNCONNECTED[3:0]),
        .s_axi_rlast(NLW_U0_s_axi_rlast_UNCONNECTED),
        .s_axi_rready(1'b0),
        .s_axi_rresp(NLW_U0_s_axi_rresp_UNCONNECTED[1:0]),
        .s_axi_rvalid(NLW_U0_s_axi_rvalid_UNCONNECTED),
        .s_axi_sbiterr(NLW_U0_s_axi_sbiterr_UNCONNECTED),
        .s_axi_wdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_wlast(1'b0),
        .s_axi_wready(NLW_U0_s_axi_wready_UNCONNECTED),
        .s_axi_wstrb(1'b0),
        .s_axi_wvalid(1'b0),
        .sbiterr(NLW_U0_sbiterr_UNCONNECTED),
        .shutdown(1'b0),
        .sleep(1'b0),
        .wea(wea),
        .web(web));
endmodule
`pragma protect begin_protected
`pragma protect version = 1
`pragma protect encrypt_agent = "XILINX"
`pragma protect encrypt_agent_info = "Xilinx Encryption Tool 2023.1"
`pragma protect key_keyowner="Synopsys", key_keyname="SNPS-VCS-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
aMT3usC6uizzcwnzOCX4OsS16Ob+YxFcsGovFpFklbnaIaD1S0lVdxenTwHPp6ByIEi+ehwr6Rgg
z/3AlTheI5NFTM8ihiMA18/wmUxI7EbaftJACA1LykUKCuj5myy0T+DACuv3sGYIZS38TZTZnnBC
FGAlvTZmRWs+JzneH3o=

`pragma protect key_keyowner="Aldec", key_keyname="ALDEC15_001", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
lR9ZerhYSAb39nzEkeYvhnwEs5t9y/+yTDf8KuoUtR1BGeHZq8pA/YxtjzQLtaOW1R1IQUb0FtSI
e3CYAb7WHYbIjcpw3vKHvW1SqcGn9CMGa556CYKmD2oF12Kow8xRaFvMSBUVxX7HsHxNWnRd+PU1
+C0YayU2KFIY/7Yl6cZ5luAzhw/6SW3PFYUIyyqWy5MCIXweHOwQR2IpQEdlDur5nluN7i7BeB+i
fxwwHh8TU/g7T4mhZFkiTuBKdLAtQOjxWxzqTMxgcuAjlTylY16FgMFOASdvvSbqBZJjbxMdVloU
rYjS8O/8rWktv8GXcaIdBJ2BRj01q7jsChsbwA==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VELOCE-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=128)
`pragma protect key_block
Qvl63GHz9mq2xOB7elt/vAQ7URLGdD1Lkcz7f3Wtw31dwjjjbP62Ny/Jr6OmBIheWlgejx38qxAT
TrHiiEyjKmGcnPn1Tn2n+cH4RAxCbOFnCI9n6+YsYMTe9JkplGhGGr39SkFgJz0I2IKpPsuqTjCj
rhf49TAryNMQeRpREJA=

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-VERIF-SIM-RSA-2", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
MA+9Ro+dh339m0iZrkKbqTKN8gQ5xkxN/SPCfhkOn+5jjgCTS5IOKLHil+HsZDjX333ebxnornwG
MOBxyEdFfLM8SA+bs2r41J/j0af2VVMmCM3hOh8JmZxB4X9Jg/glegNCbvwzqxMbOQNEy+zt7j5t
TFVD82RtPFmYVVYZZyll/WvAA+0aVpyjzLCIM1GznFky0RWLv65Wp4MJJnNRRrtG3muMznVO/u2s
tACsJ9jzv9M0IlMYjYH9BixhG6cZX02I4LEXXaPkhdOINlMMhsbArXtc9NphzmS4bY1/1yF1D6YD
EKLyS2Sr3HDl0O/lefN+jvfG8iKuVl55PNNrVQ==

`pragma protect key_keyowner="Real Intent", key_keyname="RI-RSA-KEY-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
wpMTg7STjFkUDhOqdNPa0FHXTnHQgKmhvqDv+rRVBvMiQ8O7u8oj7ibITq3o+jugJsMJ60B410gQ
JFTcqCJKYmYJvqi8rPLLOYDmFG6ZLP/Ixr3n62IyIaCeDltBahi3yV009QN0X+iuzuFCL+Y7g9ff
IvAgyBly+Z3Itv2H9EJMZPMl17Sa7IkgjmWqzVXIKNMKn0iDVYsQw6ZgzQDYQ8N8IvTIEggU3/lh
6Nf0hV0ev3qOv/2P+4w0U766Ux3yLuzPJSI7bKm3/ip9NjhOytxOiKKqVXhKG8dzbbuS5u3EE/eq
q6YxkL7gpvNltVqqBnJB6vHSyWrD6+MqsCtR9A==

`pragma protect key_keyowner="Xilinx", key_keyname="xilinxt_2022_10", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
Q7Q4SSp70lxFryaopuic9VVP/Ire0pSsPEIMYdURBAczC7ShkuYeV02U7L3BlAiyBE4vBKcwYSQd
cWiaj8sVP7q4kxoRHKxLV1R5PIO6l4DsLWE2E+1MLyUPME0w5KTular/oX8EPCJ5n/8VCtW7x4Vf
dpeyki1/IAPJkAyi3zVZKHzgKhEwnZaZZtZYuMWoPZMt4V38sAcE42Raf+7yfFWG5HO74JY6iEnW
gJeRk58K+avB/XLF2/j2RQZfjTYizrprT2tUMBK6e7DRWZZtk8AOcsMhUikev44IFGNbNXjP8BXC
0J3y3P7pCFT6l+saU83nRwi/H25fSA34diJtNw==

`pragma protect key_keyowner="Metrics Technologies Inc.", key_keyname="DSim", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
a/8ooC+s+6nfvfa1+oBhsvYWLJjFgp83DI1kNyOi5Am+ugPbGRmgGZudfyo6yw6Yd5gGbLm5aToQ
5G4cGF5HaXD5TU6A0ZZFMTIbzFLE76JMjjIxX8JcaJIZpSmrXqlru8l5gDINUEAmwUY3mRQnjcGJ
0Z+kMRH8iAEF+gEviPiFZSBbJeOPqivIS217kimQJX3BeNbNPQTP+GUidcRywpGMh5avxtA0kDRO
F9SoCSyTm9hr2v9hsK1IUAYQLb7n2/R+z5YNKNzt1oN4qgJH1wZfdI8if2K8+ohyOdnxrrgJOWdj
cOqr7cGqEOYfBMTIQeHVZzb7NGWVN+9B8XSUaQ==

`pragma protect key_keyowner="Atrenta", key_keyname="ATR-SG-RSA-1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=384)
`pragma protect key_block
FLPvOUNRWNW2GU+FEGmt2XWthOT5bY/31DRbol2cUmEGNF6b2XzpCosNKGx/o2n6sQvGP39KRFCs
nJu0ihe2dUGee9nEZZUcpwPjnEfXVI3yJaRVYy8iL+rm59lXq0jX4sjAPieDvv8shgAnoXLTZGlq
K+2c1JhaHt+nFi27TDrYar/+P8nP1MhocOS7BjzCvSs0foEXj92/qD+71Sm/LqGr8cjlH2qTJJ8B
ynxoH6iT+bksVA2VbtPT9o6h1kJ/zwP4wcsL9l+qSlJhd4GI11JPux26DlNyIi41WmufQcfiT0PB
r6O9+0E9lV9ODwKdjaxfZRK29rjKeq2yr0jWhMV38XKKqHAJli7MIypGRXcCo+u89H87KgYt+ebw
s3foIqCe0JKR57WzI8VD6XdNtOL8eBxK539oemx4vkE0cGYECZKYru6A2hPeZOYDD5eyWSUlQl1R
EciK49WM8HnssyRVcmE6di6bISMbVi0TZG/v98bz+9UZa8DtqMVYH0tz

`pragma protect key_keyowner="Cadence Design Systems.", key_keyname="CDS_RSA_KEY_VER_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
fphquQOeFuqByo36Gh2C1zEC1J6u9swSMbMzsKldIvLm+SZ6/hr/N8KJ/G2vBABzX6UtbVuP1ZXx
AxdftP4Aqis1B3Bs6989aQG9eo0SOHA7r6aFLtFb3qoD5Pvqw4aVNU4z4EtTpFpn/jCWD21lKROf
q5X32HRfFq1jwqod+9vIbUNRRzz5y9VHvXfacZlxDazSPmcCF4hxB1KqWqT44KmYVkDedgkgnYgb
ZGidHnTb3W7C8tSqC9ac4kNJCL429QndtddweESJNlpX+65pt9Irok9pkOodwoj0QScswOIFjhBZ
/GrzZLQcFWiD3gXRU4DazzxQnGdRH4qEIRWziw==

`pragma protect key_keyowner="Synplicity", key_keyname="SYNP15_1", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
1lUYYHPCt1BUJOvcBbgMU2GSQiqfxItz4ntieMaenjrtsE9SLwaU6xB0tBl8Atw5yP/RRNww1kX/
9uZbTz5He3r9mPVt+mGxB4N3f9BbCrQRb4USVPgKO/+vWUfMQERGklScy0+fz75WuxH74CjRUoDI
8iyssb2cUNnfDe13jIoI8gM1w4w/Pkxkmb6Mef53QMxacHAWEZeytcH3fuL/adO263D8P90U3XJv
vBXJmbjkRVi9qzjBzfMxuOy2KbZaZgR3BLzaffIfFnMwg/Rb8sGls5pQsZv5jL2wk3+Bj3OXBYdd
pDyjGoalJBzObKzd/t15kNHwY4FXYFcZLQPncw==

`pragma protect key_keyowner="Mentor Graphics Corporation", key_keyname="MGC-PREC-RSA", key_method="rsa"
`pragma protect encoding = (enctype="BASE64", line_length=76, bytes=256)
`pragma protect key_block
YRmSEzaa2WFVvMH1BwWc1TIUpVbzSEIP0VbI6n0sEgct/X4PiTfMQmK1jBVCaISIzwBxscKQwZOt
mb/nmINGg6I7ih39LSbBMtx6cdCUiyaLkPeRbqfyPpKhvnUIFmdKVvTd1dYzxeOeuDnhSVaBaAcN
3lngSg7lIbmhLIGjC29yQrBTiLArbVZi6IRGronMK51e3UrYa6GspsznhiuRcXjEb4bHKrJ2CM5Z
BUwA+E9949sQgyOagFZbLVle2ESbwBaoxcAPn2gxfRHlT0leqyLgUGDZLsfArzGzw9BTGzyEG2TR
XOrKFNYRfMXMrnGsBM7acIelY4LdAMgsKgDH/A==

`pragma protect data_method = "AES128-CBC"
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 48384)
`pragma protect data_block
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
`pragma protect end_protected
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;
    parameter GRES_WIDTH = 10000;
    parameter GRES_START = 10000;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    wire GRESTORE;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;
    reg GRESTORE_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;
    assign (strong1, weak0) GRESTORE = GRESTORE_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

    initial begin 
	GRESTORE_int = 1'b0;
	#(GRES_START);
	GRESTORE_int = 1'b1;
	#(GRES_WIDTH);
	GRESTORE_int = 1'b0;
    end

endmodule
`endif
