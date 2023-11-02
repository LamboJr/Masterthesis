// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2023 Advanced Micro Devices, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2023.1 (lin64) Build 3865809 Sun May  7 15:04:56 MDT 2023
// Date        : Thu Nov  2 18:13:27 2023
// Host        : Acer running 64-bit Ubuntu 22.04.3 LTS
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ blk_mem_gen_0_sim_netlist.v
// Design      : blk_mem_gen_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7z020clg400-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "blk_mem_gen_0,blk_mem_gen_v8_4_6,{}" *) (* downgradeipidentifiedwarnings = "yes" *) (* x_core_info = "blk_mem_gen_v8_4_6,Vivado 2023.1" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
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
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_blk_mem_gen_v8_4_6 U0
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
`pragma protect encoding = (enctype = "BASE64", line_length = 76, bytes = 48528)
`pragma protect data_block
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
pAXaWOcPnJOgyUWyFEk67Ea6GFBd
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
