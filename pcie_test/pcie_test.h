#ifndef HWTEST_H
#define HWTEST_H

#define PCI_TEST_MEM 0x80000000
#define PCI_TEST_SIZE 0x100000
#define PCI_TEST_MEM_END (PCI_TEST_MEM + PCI_TEST_SIZE)

#define PCI_IOMEM0_START 0x30600000
#define PCI_IOMEM0_SIZE    0x800000
#define PCI_IOMEM1_START 0x30e02000
#define PCI_IOMEM1_SIZE      0x1000
#define PCI_IOMEM2_START 0x40000000
#define PCI_IOMEM2_SIZE  0x10000000

/* 寄存器物理地址 */
#define PCI_APB_BASE 0x30e02000
#define PCI_SW_RST   0x330020e8
#define A55_GIC_BASE_ADDR (0x32000000)
#define A55_GIC_DIST_OFFSET 0x1000
#define A55_GIC_DIST_ENABLE_SET 0x100
#define A55_GIC_DIST_ENABLE_CLEAR 0x180

#define PCI_DM_BASE 0x30600000
//local CPU通过DBI配置ATU的基地址
#define ATU_OFF 0x300000
//local CPU通过DBI配置DMA的基地址
#define DMA_OFF 0x380000
#define PCI_DM_ATU (PCI_DM_BASE + ATU_OFF)
#define PCI_DM_DMA (PCI_DM_BASE + DMA_OFF)
#define PCI_DM_AXISLAVE 0x40000000
#define PCI_DM_AXISLAVE_REGION_MEM PCI_DM_AXISLAVE //0x40000000
#define PCI_DM_AXISLAVE_REGION_MEM_SIZE 0x100000 //1M
#define PCI_DM_AXISLAVE_REGION_DMA (PCI_DM_AXISLAVE + 0x1000000) //0x41000000
#define PCI_DM_AXISLAVE_REGION_DMA_SIZE 0x100000 //1M
#define PCI_DM_AXISLAVE_REGION_CFG (PCI_DM_AXISLAVE + 0x7000000) //0x47000000
#define PCI_DM_AXISLAVE_REGION_CFG_SIZE 0x1000 //4K

#define PCI_EP_BASE 0x30a00000
#define PCI_EP_ATU (PCI_EP_BASE + ATU_OFF)
#define PCI_EP_DMA (PCI_EP_BASE + DMA_OFF)
#define PCI_EP_AXISLAVE 0x48000000
#define PCI_EP_AXISLAVE_REGION_MEM PCI_EP_AXISLAVE //0x48000000
#define PCI_EP_AXISLAVE_REGION_MEM_SIZE 0x4000 //16K

/* APB reg */
#define PCIE_LTSSM_EN             0x00
#define PCIE_PHY_CTRL             0x04
#define PCIE_MODE                 0x08
#define PCIE_CLK_CTRL             0x0c
#define PCIE_PHY_LPBK             0x10
#define PCIE_LANE_FLIP_EN         0x14
#define PCIE_DIAG_CTRL            0x18
#define PCIE_DEBUG_PIN_OUT        0x1c
#define PCIE_PHY_SRAM_CTRL        0x20
#define PCIE_PHY_SRAM_INIT_STATUS 0x24
#define PCIE_PHY_CRI_TYPE         0x28
#define PCIE_PHY_CRI_SEL          0x2c
#define PCIE_PHY_CRI_ADDR         0x30
#define PCIE_PHY_CRT_WDATA        0x34
#define PCIE_PHY_CRI_RDATA        0x38
#define PCIE_PHY_REG_0   0x3c
#define PCIE_PHY_REG_1   0x40
#define PCIE_PHY_REG_2   0x44
#define PCIE_PHY_REG_3   0x48
#define PCIE_PHY_REG_4   0x4c
#define PCIE_PHY_REG_5   0x50
#define PCIE_PHY_REG_6   0x54
#define PCIE_PHY_REG_7   0x58
#define PCIE_PHY_REG_8   0x5c
#define PCIE_PHY_REG_9   0x60
#define PCIE_PHY_REG_10  0x64
#define PCIE_PHY_REG_11  0x68
#define PCIE_PHY_REG_12  0x6c
#define PCIE_PHY_REG_13  0x70
#define PCIE_PHY_REG_14  0x74
#define PCIE_PHY_REG_15  0x78
#define PCIE_PHY_REG_16  0x7c
#define PCIE_PHY_REG_17  0x80
#define PCIE_PHY_REG_18  0x84
#define PCIE_PHY_REG_19  0x88
#define PCIE_PHY_REG_20  0x8c
#define PCIE_PHY_REG_21  0x90
#define PCIE_PHY_REG_22  0x94
#define PCIE_PHY_REG_23  0x98
#define PCIE_PHY_REG_24  0x9c
#define PCIE_PHY_REG_25  0xa0
#define PCIE_PHY_REG_26  0xa4
#define PCIE_PHY_REG_27  0xa8
#define PCIE_PHY_REG_28  0xac
#define PCIE_PHY_REG_29  0xb0
#define PCIE_PHY_REG_30  0xb4
#define PCIE_PHY_REG_31  0xb8
#define PCIE_PHY_REG_32  0xbc
#define PCIE_DEBUG_INFO_0  0xc0
#define PCIE_DEBUG_INFO_1  0xc4
#define PCIE_DEBUG_INFO_2  0xc8
#define PCIE_DEBUG_INFO_3  0xcc
#define PCIE_DEBUG_INFO_4  0xd0
#define PCIE_DEBUG_INFO_5  0xd4
#define PCIE_DEBUG_INFO_6  0xd8
#define PCIE_DEBUG_INFO_7  0xdc
#define PCIE_DEBUG_INFO_8  0xe0
#define PCIE_DEBUG_INFO_9  0xe4
#define PCIE_DEBUG_INFO_10  0xe8
#define PCIE_DEBUG_INFO_11  0xec
#define PCIE_DEBUG_INFO_12  0xf0
#define PCIE_DEBUG_INFO_13  0xf4
#define PCIE_DEBUG_INFO_14  0xf8
#define PCIE_DEBUG_INFO_15  0xfc
#define PCIE_DEBUG_INFO_16  0x100
#define PCIE_RASDP_ERR_MODE  0x104
#define PCIE_X4_PM  0x108
#define PCIE_X2_PM  0x10c
#define PCIE_X4_CTRL_MISC  0x110
#define PCIE_X2_CTRL_MISC  0x114
#define PCIE_SPARE_REG  0x118
#define PCIE_LOCAL_RST  0x11c
#define PCIE_X4_INT_MASK  0x120
#define PCIE_X4_INT_STATUS  0x124
#define PCIE_X2_INT_MASK  0x128
#define PCIE_X2_INT_STATUS  0x12c
#define PCIE_INT_CLR  0x130
#define PCIE_PHY_PLL_STATE  0x134
#define PCIE_PHY_PLL_FORCE  0x138
#define PCIE_PHY_SRC_SEL  0x13c
#define PCIE_LANEX_LINK_NUM  0x140
#define PCIE_PCS_ALGN_STATUS  0x144
#define PCIE_PCS_EBUF_L23  0x148
#define PCIE_PCS_EBUF_L01  0x14c


#define A55_ISR_ATTR_A55           0x4
#define A55_ISR_ATTR_LEVEL         0X00000
#define A55_IRQ_PCIE_X4_MSI_CTRL_INT         229
/* Message Signalled Interrupt registers */
#define PCI_MSI_CAP_OFF 0x50
#define PCI_MSI_ADDRESS_LO	4	/* Lower 32 bits */
#define PCI_MSI_ADDRESS_HI	8	/* Upper 32 bits (if PCI_MSI_FLAGS_64BIT set) */
#define PCI_MSI_DATA_64		12	/* 16 bits of data for 64-bit devices */
#define RC_MSI_CTRL_ADDR_OFF            0x820
#define RC_MSI_CTRL_UPPER_ADDR_OFF      0x824
#define RC_MSI_CTRL_INT_EP0_EN_OFF      0x828
#define RC_MSI_CTRL_INT_EP0_MASK_OFF    0x82c
#define RC_MSI_CTRL_INT_EP0_STATUS_OFF    0x830
#define PCIE_MISC_CONTROL_1_OFF		0x8BC
#define BIT(nr)			(1UL << (nr))
#define PCIE_DBI_RO_WR_EN		BIT(0)
#define PORT_LOGIC_WR_DISABLE	BIT(22)
#define PCI_MSI_64_BIT_ADDR_CAP	BIT(23)

#define PCIE_TEST_MSI_ADDRESS 0x8000fe00
//对端RC通过mem访问EP DMA的基地址，硬件固定死的
#define PC_REMOTE_CFG_EP_DMA 0x1000
//对端RC通过mem访问EP ATU的基地址，硬件固定死的
#define RC_REMOTE_CFG_EP_ATU 0x3200

typedef enum {
	PCI_PM_CAP_ID=0x1,
	PCI_VPD_ID=0x03,
	PCI_SLOT_ID=0x04,
	PCI_MSI_CAP_ID=0x05,
	PCIe_CAP_ID=0x10,
	PCI_MSIX_CAP_ID=0x11,
	PCI_SATA_ID=0x12,
	PCI_TYPE0_HDR=0x13,
	PCI_TYPE1_HDR=0x14
} pcie_t_cap_id;

typedef enum {
	PCIE_AER_CAP_ID=0x0001,
	PCIE_VC_CAP_ID=0x0002, // or 9
	PCIE_DSN_CAP_ID=0x0003,
	PCIE_POWER_BUDGET_CAP_ID=0x0004,
	PCIE_RC_INTERNAL_LINK_CAP_ID=0x0005,
	PCIE_RC_INTERNAL_INT_LINK_CAP_ID=0x0006,
	PCIE_RC_EVENT_COLLECTOR_EP_ASSOCIATION_CAP_ID=0x0007,
	PCIE_MFVC_CAP_ID=0x0008,
	PCIE_RCRB_HDR_CAP_ID=0x000A,
	PCIE_VENDOR_SPECIFIC_CAP_ID=0x000B,
	PCIE_ACS_CAP_ID=0x000D,
	PCIE_ARI_CAP_ID=0x000E,
	PCIE_ATS_CAP_ID=0x000F,
	PCIE_SRIOV_CAP_ID=0x0010,
	PCIE_MRIOV_CAP_ID=0x0011,
	PCIE_MULTICAST_CAP_ID=0x0012,
	PRS_EXT_CAP_ID=0x0013,
	PCIE_RESIZABLE_BAR_CAP_ID=0x0015,
	PCIE_DPA_CAP_ID=0x0016,
	PCIE_LTR_CAP_ID=0x0018,
	PCIE_TPH_CAP_ID=0x0017,
	PCIE_SPCIE_CAP_ID=0x0019,
	PCIE_PMUX_CAP_ID=0x001A,
	PCIE_PASID_CAP_ID=0x001B,
	PCIE_LNR_CAP_ID=0x001C,
	PCIE_DPC_CAP_ID=0x001D,
	PCIE_L1SUB_CAP_ID=0x001E,
	PTM_EXT_CAP_ID=0x001F,
	PCIE_MPCIE_CAP_ID=0x0020,
	FRSQ_EXT_CAP_ID=0x0021,
	RTR_EXT_CAP_ID=0x0022,
	PCIE_DVSEC_CAP_ID=0x0023,
	PCIE_VF_RESIZABLE_BAR_CAP_ID=0x0024,
	DLINK_EXT_CAP_ID=0x0025,
	PCIE_PL16G_CAP_ID=0x0026,
	PCIE_MARGIN_CAP_ID=0x0027,
	PCIE_PL32G_CAP_ID=0x0028
} pcie_t_ecap_id;

struct pci_reg_base {
	//void __iomem *phy_base;
	void __iomem *apb_base;
	void __iomem *sw_rst;
	void __iomem *gic_dist;
	void __iomem *dm_base;
	void __iomem *dm_atu;
	void __iomem *dm_dma;
	void __iomem *dm_axislv_cfg;
	void __iomem *dm_axislv_dma;
	void __iomem *dm_axislv_mem;
	void __iomem *dm_mem;
	void __iomem *ep_base;
	void __iomem *ep_atu;
	void __iomem *ep_dma;
	void __iomem *ep_axislv_mem;
	void __iomem *ep_mem;
};

struct dm_outbound {
	u32 base;
	u32 size;
	u32 target;
};

#endif /* HWTEST_H */
