// SPDX-License-Identifier: GPL-2.0
/* Copyright (C) 2012-2018 ARM Limited or its affiliates. */

#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/crypto.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <linux/of_address.h>

#include "cc_driver.h"
#include "cc_request_mgr.h"
#include "cc_buffer_mgr.h"
#include "cc_debugfs.h"
#include "cc_cipher.h"
#include "cc_aead.h"
#include "cc_hash.h"
#include "cc_ivgen.h"
#include "cc_sram_mgr.h"
#include "cc_pm.h"
#include "cc_fips.h"

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
#ifdef DX_DUMP_BYTES
void dump_byte_array(const char *name, const u8 *the_array, unsigned long size)
{
	int i, line_offset = 0, ret = 0;
	const u8 *cur_byte;
	char line_buf[80];
=======
bool cc_dump_desc;
module_param_named(dump_desc, cc_dump_desc, bool, 0600);
MODULE_PARM_DESC(cc_dump_desc, "Dump descriptors to kernel log as debugging aid");

bool cc_dump_bytes;
module_param_named(dump_bytes, cc_dump_bytes, bool, 0600);
MODULE_PARM_DESC(cc_dump_bytes, "Dump buffers to kernel log as debugging aid");

void __dump_byte_array(const char *name, const u8 *buf, size_t len)
{
	char prefix[64];
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c

	if (!the_array) {
		SSI_LOG_ERR("cannot dump array - NULL pointer\n");
		return;
	}

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	ret = snprintf(line_buf, sizeof(line_buf), "%s[%lu]: ", name, size);
	if (ret < 0) {
		SSI_LOG_ERR("snprintf returned %d . aborting buffer array dump\n", ret);
		return;
	}
	line_offset = ret;
	for (i = 0, cur_byte = the_array;
	     (i < size) && (line_offset < sizeof(line_buf)); i++, cur_byte++) {
			ret = snprintf(line_buf + line_offset,
				       sizeof(line_buf) - line_offset,
				       "0x%02X ", *cur_byte);
		if (ret < 0) {
			SSI_LOG_ERR("snprintf returned %d . aborting buffer array dump\n", ret);
			return;
		}
		line_offset += ret;
		if (line_offset > 75) { /* Cut before line end */
			SSI_LOG_DEBUG("%s\n", line_buf);
			line_offset = 0;
		}
	}

	if (line_offset > 0) /* Dump remaining line */
		SSI_LOG_DEBUG("%s\n", line_buf);
=======
	snprintf(prefix, sizeof(prefix), "%s[%zu]: ", name, len);

	print_hex_dump(KERN_DEBUG, prefix, DUMP_PREFIX_ADDRESS, 16, 1, buf,
		       len, false);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
}

static irqreturn_t cc_isr(int irq, void *dev_id)
{
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	struct ssi_drvdata *drvdata = (struct ssi_drvdata *)dev_id;
	void __iomem *cc_base = drvdata->cc_base;
=======
	struct cc_drvdata *drvdata = (struct cc_drvdata *)dev_id;
	struct device *dev = drvdata_to_dev(drvdata);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	u32 irr;
	u32 imr;

	/* STAT_OP_TYPE_GENERIC STAT_PHASE_0: Interrupt */

	/* read the interrupt status */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	irr = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IRR));
	SSI_LOG_DEBUG("Got IRR=0x%08X\n", irr);
	if (unlikely(irr == 0)) { /* Probably shared interrupt line */
		SSI_LOG_ERR("Got interrupt with empty IRR\n");
=======
	irr = cc_ioread(drvdata, CC_REG(HOST_IRR));
	dev_dbg(dev, "Got IRR=0x%08X\n", irr);
	if (irr == 0) { /* Probably shared interrupt line */
		dev_err(dev, "Got interrupt with empty IRR\n");
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		return IRQ_NONE;
	}
	imr = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IMR));

	/* clear interrupt - must be before processing events */
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_ICR), irr);

	drvdata->irq = irr;
	/* Completion interrupt - most probable */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	if (likely((irr & SSI_COMP_IRQ_MASK) != 0)) {
		/* Mask AXI completion interrupt - will be unmasked in Deferred service handler */
		CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IMR), imr | SSI_COMP_IRQ_MASK);
		irr &= ~SSI_COMP_IRQ_MASK;
=======
	if (irr & CC_COMP_IRQ_MASK) {
		/* Mask AXI completion interrupt - will be unmasked in
		 * Deferred service handler
		 */
		cc_iowrite(drvdata, CC_REG(HOST_IMR), imr | CC_COMP_IRQ_MASK);
		irr &= ~CC_COMP_IRQ_MASK;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		complete_request(drvdata);
	}
#ifdef CONFIG_CRYPTO_FIPS
	/* TEE FIPS interrupt */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	if (likely((irr & SSI_GPR0_IRQ_MASK) != 0)) {
		/* Mask interrupt - will be unmasked in Deferred service handler */
		CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IMR), imr | SSI_GPR0_IRQ_MASK);
		irr &= ~SSI_GPR0_IRQ_MASK;
=======
	if (irr & CC_GPR0_IRQ_MASK) {
		/* Mask interrupt - will be unmasked in Deferred service
		 * handler
		 */
		cc_iowrite(drvdata, CC_REG(HOST_IMR), imr | CC_GPR0_IRQ_MASK);
		irr &= ~CC_GPR0_IRQ_MASK;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		fips_handler(drvdata);
	}
#endif
	/* AXI error interrupt */
	if (irr & CC_AXI_ERR_IRQ_MASK) {
		u32 axi_err;

		/* Read the AXI error ID */
		axi_err = CC_HAL_READ_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_MON_ERR));
		SSI_LOG_DEBUG("AXI completion error: axim_mon_err=0x%08X\n", axi_err);

		irr &= ~CC_AXI_ERR_IRQ_MASK;
	}

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	if (unlikely(irr != 0)) {
		SSI_LOG_DEBUG("IRR includes unknown cause bits (0x%08X)\n", irr);
=======
	if (irr) {
		dev_dbg(dev, "IRR includes unknown cause bits (0x%08X)\n",
			irr);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		/* Just warning */
	}

	return IRQ_HANDLED;
}

int init_cc_regs(struct cc_drvdata *drvdata, bool is_probe)
{
	unsigned int val, cache_params;
	void __iomem *cc_base = drvdata->cc_base;

	/* Unmask all AXI interrupt sources AXI_CFG1 register */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	val = CC_HAL_READ_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CFG));
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CFG), val & ~SSI_AXI_IRQ_MASK);
	SSI_LOG_DEBUG("AXIM_CFG=0x%08X\n", CC_HAL_READ_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CFG)));
=======
	val = cc_ioread(drvdata, CC_REG(AXIM_CFG));
	cc_iowrite(drvdata, CC_REG(AXIM_CFG), val & ~CC_AXI_IRQ_MASK);
	dev_dbg(dev, "AXIM_CFG=0x%08X\n",
		cc_ioread(drvdata, CC_REG(AXIM_CFG)));
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c

	/* Clear all pending interrupts */
	val = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IRR));
	SSI_LOG_DEBUG("IRR=0x%08X\n", val);
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_ICR), val);

	/* Unmask relevant interrupt cause */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	val = (~(SSI_COMP_IRQ_MASK | SSI_AXI_ERR_IRQ_MASK | SSI_GPR0_IRQ_MASK));
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IMR), val);

#ifdef DX_HOST_IRQ_TIMER_INIT_VAL_REG_OFFSET
#ifdef DX_IRQ_DELAY
	/* Set CC IRQ delay */
	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IRQ_TIMER_INIT_VAL),
			      DX_IRQ_DELAY);
#endif
	if (CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IRQ_TIMER_INIT_VAL)) > 0) {
		SSI_LOG_DEBUG("irq_delay=%d CC cycles\n",
			      CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_IRQ_TIMER_INIT_VAL)));
	}
#endif

=======
	val = (unsigned int)(~(CC_COMP_IRQ_MASK | CC_AXI_ERR_IRQ_MASK |
			       CC_GPR0_IRQ_MASK));
	cc_iowrite(drvdata, CC_REG(HOST_IMR), val);

>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	cache_params = (drvdata->coherent ? CC_COHERENT_CACHE_PARAMS : 0x0);

	val = CC_HAL_READ_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CACHE_PARAMS));

	if (is_probe)
		SSI_LOG_INFO("Cache params previous: 0x%08X\n", val);

	CC_HAL_WRITE_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CACHE_PARAMS),
			      cache_params);
	val = CC_HAL_READ_REGISTER(CC_REG_OFFSET(CRY_KERNEL, AXIM_CACHE_PARAMS));

	if (is_probe)
		SSI_LOG_INFO("Cache params current: 0x%08X (expect: 0x%08X)\n",
			     val, cache_params);

	return 0;
}

static int init_cc_resources(struct platform_device *plat_dev)
{
	struct resource *req_mem_cc_regs = NULL;
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	void __iomem *cc_base = NULL;
	bool irq_registered = false;
	struct ssi_drvdata *new_drvdata = kzalloc(sizeof(*new_drvdata),
						  GFP_KERNEL);
	struct device *dev = &plat_dev->dev;
	struct device_node *np = dev->of_node;
	u32 signature_val;
=======
	struct cc_drvdata *new_drvdata;
	struct device *dev = &plat_dev->dev;
	struct device_node *np = dev->of_node;
	u32 signature_val;
	u64 dma_mask;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	int rc = 0;

	if (unlikely(!new_drvdata)) {
		SSI_LOG_ERR("Failed to allocate drvdata");
		rc = -ENOMEM;
		goto init_cc_res_err;
	}

	new_drvdata->clk = of_clk_get(np, 0);
	new_drvdata->coherent = of_dma_is_coherent(np);

	/*Initialize inflight counter used in dx_ablkcipher_secure_complete used for count of BYSPASS blocks operations*/
	new_drvdata->inflight_counter = 0;

	dev_set_drvdata(&plat_dev->dev, new_drvdata);
	/* Get device resources */
	/* First CC registers space */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	new_drvdata->res_mem = platform_get_resource(plat_dev, IORESOURCE_MEM, 0);
	if (unlikely(!new_drvdata->res_mem)) {
		SSI_LOG_ERR("Failed getting IO memory resource\n");
		rc = -ENODEV;
		goto init_cc_res_err;
	}
	SSI_LOG_DEBUG("Got MEM resource (%s): start=%pad end=%pad\n",
		      new_drvdata->res_mem->name,
		      new_drvdata->res_mem->start,
		      new_drvdata->res_mem->end);
	/* Map registers space */
	req_mem_cc_regs = request_mem_region(new_drvdata->res_mem->start, resource_size(new_drvdata->res_mem), "arm_cc7x_regs");
	if (unlikely(!req_mem_cc_regs)) {
		SSI_LOG_ERR("Couldn't allocate registers memory region at "
			     "0x%08X\n", (unsigned int)new_drvdata->res_mem->start);
		rc = -EBUSY;
		goto init_cc_res_err;
	}
	cc_base = ioremap(new_drvdata->res_mem->start, resource_size(new_drvdata->res_mem));
	if (unlikely(!cc_base)) {
		SSI_LOG_ERR("ioremap[CC](0x%08X,0x%08X) failed\n",
			    (unsigned int)new_drvdata->res_mem->start,
			    (unsigned int)resource_size(new_drvdata->res_mem));
		rc = -ENOMEM;
		goto init_cc_res_err;
	}
	SSI_LOG_DEBUG("CC registers mapped from %pa to 0x%p\n", &new_drvdata->res_mem->start, cc_base);
	new_drvdata->cc_base = cc_base;
=======
	req_mem_cc_regs = platform_get_resource(plat_dev, IORESOURCE_MEM, 0);
	/* Map registers space */
	new_drvdata->cc_base = devm_ioremap_resource(dev, req_mem_cc_regs);
	if (IS_ERR(new_drvdata->cc_base))
		return PTR_ERR(new_drvdata->cc_base);

	dev_dbg(dev, "Got MEM resource (%s): %pR\n", req_mem_cc_regs->name,
		req_mem_cc_regs);
	dev_dbg(dev, "CC registers mapped from %pa to 0x%p\n",
		&req_mem_cc_regs->start, new_drvdata->cc_base);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c

	/* Then IRQ */
	new_drvdata->res_irq = platform_get_resource(plat_dev, IORESOURCE_IRQ, 0);
	if (unlikely(!new_drvdata->res_irq)) {
		SSI_LOG_ERR("Failed getting IRQ resource\n");
		rc = -ENODEV;
		goto init_cc_res_err;
	}
	rc = request_irq(new_drvdata->res_irq->start, cc_isr,
			 IRQF_SHARED, "arm_cc7x", new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("Could not register to interrupt %llu\n",
			    (unsigned long long)new_drvdata->res_irq->start);
		goto init_cc_res_err;
	}
	init_completion(&new_drvdata->icache_setup_completion);

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	irq_registered = true;
	SSI_LOG_DEBUG("Registered to IRQ (%s) %llu\n",
		      new_drvdata->res_irq->name,
		      (unsigned long long)new_drvdata->res_irq->start);

	new_drvdata->plat_dev = plat_dev;
=======
	init_completion(&new_drvdata->hw_queue_avail);

	if (!plat_dev->dev.dma_mask)
		plat_dev->dev.dma_mask = &plat_dev->dev.coherent_dma_mask;

	dma_mask = DMA_BIT_MASK(DMA_BIT_MASK_LEN);
	while (dma_mask > 0x7fffffffUL) {
		if (dma_supported(&plat_dev->dev, dma_mask)) {
			rc = dma_set_coherent_mask(&plat_dev->dev, dma_mask);
			if (!rc)
				break;
		}
		dma_mask >>= 1;
	}

	if (rc) {
		dev_err(dev, "Failed in dma_set_mask, mask=%par\n", &dma_mask);
		return rc;
	}
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c

	rc = cc_clk_on(new_drvdata);
	if (rc)
		goto init_cc_res_err;

	if (!new_drvdata->plat_dev->dev.dma_mask)
		new_drvdata->plat_dev->dev.dma_mask = &new_drvdata->plat_dev->dev.coherent_dma_mask;

	if (!new_drvdata->plat_dev->dev.coherent_dma_mask)
		new_drvdata->plat_dev->dev.coherent_dma_mask = DMA_BIT_MASK(DMA_BIT_MASK_LEN);

	/* Verify correct mapping */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	signature_val = CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_SIGNATURE));
	if (signature_val != DX_DEV_SIGNATURE) {
		SSI_LOG_ERR("Invalid CC signature: SIGNATURE=0x%08X != expected=0x%08X\n",
			    signature_val, (u32)DX_DEV_SIGNATURE);
=======
	signature_val = cc_ioread(new_drvdata, CC_REG(HOST_SIGNATURE));
	if (signature_val != CC_DEV_SIGNATURE) {
		dev_err(dev, "Invalid CC signature: SIGNATURE=0x%08X != expected=0x%08X\n",
			signature_val, (u32)CC_DEV_SIGNATURE);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		rc = -EINVAL;
		goto init_cc_res_err;
	}
	SSI_LOG_DEBUG("CC SIGNATURE=0x%08X\n", signature_val);

	/* Display HW versions */
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	SSI_LOG(KERN_INFO, "ARM CryptoCell %s Driver: HW version 0x%08X, Driver version %s\n", SSI_DEV_NAME_STR,
		CC_HAL_READ_REGISTER(CC_REG_OFFSET(HOST_RGF, HOST_VERSION)), DRV_MODULE_VERSION);

	rc = init_cc_regs(new_drvdata, true);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("init_cc_regs failed\n");
		goto init_cc_res_err;
	}

#ifdef ENABLE_CC_SYSFS
	rc = ssi_sysfs_init(&plat_dev->dev.kobj, new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("init_stat_db failed\n");
		goto init_cc_res_err;
=======
	dev_info(dev, "ARM CryptoCell %s Driver: HW version 0x%08X, Driver version %s\n",
		 CC_DEV_NAME_STR,
		 cc_ioread(new_drvdata, CC_REG(HOST_VERSION)),
		 DRV_MODULE_VERSION);

	rc = init_cc_regs(new_drvdata, true);
	if (rc) {
		dev_err(dev, "init_cc_regs failed\n");
		goto post_clk_err;
	}

	rc = cc_debugfs_init(new_drvdata);
	if (rc) {
		dev_err(dev, "Failed registering debugfs interface\n");
		goto post_regs_err;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	}

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	rc = ssi_sram_mgr_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_sram_mgr_init failed\n");
		goto init_cc_res_err;
	}

	new_drvdata->mlli_sram_addr =
		ssi_sram_mgr_alloc(new_drvdata, MAX_MLLI_BUFF_SIZE);
	if (unlikely(new_drvdata->mlli_sram_addr == NULL_SRAM_ADDR)) {
		SSI_LOG_ERR("Failed to alloc MLLI Sram buffer\n");
=======
	rc = cc_fips_init(new_drvdata);
	if (rc) {
		dev_err(dev, "CC_FIPS_INIT failed 0x%x\n", rc);
		goto post_debugfs_err;
	}
	rc = cc_sram_mgr_init(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_sram_mgr_init failed\n");
		goto post_fips_init_err;
	}

	new_drvdata->mlli_sram_addr =
		cc_sram_alloc(new_drvdata, MAX_MLLI_BUFF_SIZE);
	if (new_drvdata->mlli_sram_addr == NULL_SRAM_ADDR) {
		dev_err(dev, "Failed to alloc MLLI Sram buffer\n");
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
		rc = -ENOMEM;
		goto init_cc_res_err;
	}

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	rc = request_mgr_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("request_mgr_init failed\n");
		goto init_cc_res_err;
	}

	rc = ssi_buffer_mgr_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("buffer_mgr_init failed\n");
		goto init_cc_res_err;
	}

	rc = ssi_power_mgr_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_power_mgr_init failed\n");
		goto init_cc_res_err;
	}

	rc = ssi_fips_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("SSI_FIPS_INIT failed 0x%x\n", rc);
		goto init_cc_res_err;
	}

	rc = ssi_ivgen_init(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_ivgen_init failed\n");
		goto init_cc_res_err;
	}

	/* Allocate crypto algs */
	rc = ssi_ablkcipher_alloc(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_ablkcipher_alloc failed\n");
		goto init_cc_res_err;
	}

	/* hash must be allocated before aead since hash exports APIs */
	rc = ssi_hash_alloc(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_hash_alloc failed\n");
		goto init_cc_res_err;
	}

	rc = ssi_aead_alloc(new_drvdata);
	if (unlikely(rc != 0)) {
		SSI_LOG_ERR("ssi_aead_alloc failed\n");
		goto init_cc_res_err;
=======
	rc = cc_req_mgr_init(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_req_mgr_init failed\n");
		goto post_sram_mgr_err;
	}

	rc = cc_buffer_mgr_init(new_drvdata);
	if (rc) {
		dev_err(dev, "buffer_mgr_init failed\n");
		goto post_req_mgr_err;
	}

	rc = cc_pm_init(new_drvdata);
	if (rc) {
		dev_err(dev, "ssi_power_mgr_init failed\n");
		goto post_buf_mgr_err;
	}

	rc = cc_ivgen_init(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_ivgen_init failed\n");
		goto post_power_mgr_err;
	}

	/* Allocate crypto algs */
	rc = cc_cipher_alloc(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_cipher_alloc failed\n");
		goto post_ivgen_err;
	}

	/* hash must be allocated before aead since hash exports APIs */
	rc = cc_hash_alloc(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_hash_alloc failed\n");
		goto post_cipher_err;
	}

	rc = cc_aead_alloc(new_drvdata);
	if (rc) {
		dev_err(dev, "cc_aead_alloc failed\n");
		goto post_hash_err;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	}

	/* If we got here and FIPS mode is enabled
	 * it means all FIPS test passed, so let TEE
	 * know we're good.
	 */
	cc_set_ree_fips_status(new_drvdata, true);

	return 0;

<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
init_cc_res_err:
	SSI_LOG_ERR("Freeing CC HW resources!\n");

	if (new_drvdata) {
		ssi_aead_free(new_drvdata);
		ssi_hash_free(new_drvdata);
		ssi_ablkcipher_free(new_drvdata);
		ssi_ivgen_fini(new_drvdata);
		ssi_power_mgr_fini(new_drvdata);
		ssi_buffer_mgr_fini(new_drvdata);
		request_mgr_fini(new_drvdata);
		ssi_sram_mgr_fini(new_drvdata);
		ssi_fips_fini(new_drvdata);
#ifdef ENABLE_CC_SYSFS
		ssi_sysfs_fini();
#endif

		if (req_mem_cc_regs) {
			if (irq_registered) {
				free_irq(new_drvdata->res_irq->start, new_drvdata);
				new_drvdata->res_irq = NULL;
				iounmap(cc_base);
				new_drvdata->cc_base = NULL;
			}
			release_mem_region(new_drvdata->res_mem->start,
					   resource_size(new_drvdata->res_mem));
			new_drvdata->res_mem = NULL;
		}
		kfree(new_drvdata);
		dev_set_drvdata(&plat_dev->dev, NULL);
	}

=======
post_hash_err:
	cc_hash_free(new_drvdata);
post_cipher_err:
	cc_cipher_free(new_drvdata);
post_ivgen_err:
	cc_ivgen_fini(new_drvdata);
post_power_mgr_err:
	cc_pm_fini(new_drvdata);
post_buf_mgr_err:
	 cc_buffer_mgr_fini(new_drvdata);
post_req_mgr_err:
	cc_req_mgr_fini(new_drvdata);
post_sram_mgr_err:
	cc_sram_mgr_fini(new_drvdata);
post_fips_init_err:
	cc_fips_fini(new_drvdata);
post_debugfs_err:
	cc_debugfs_fini(new_drvdata);
post_regs_err:
	fini_cc_regs(new_drvdata);
post_clk_err:
	cc_clk_off(new_drvdata);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	return rc;
}

void fini_cc_regs(struct cc_drvdata *drvdata)
{
	/* Mask all interrupts */
	WRITE_REGISTER(drvdata->cc_base +
		       CC_REG_OFFSET(HOST_RGF, HOST_IMR), 0xFFFFFFFF);
}

static void cleanup_cc_resources(struct platform_device *plat_dev)
{
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
	struct ssi_drvdata *drvdata =
		(struct ssi_drvdata *)dev_get_drvdata(&plat_dev->dev);

	ssi_aead_free(drvdata);
	ssi_hash_free(drvdata);
	ssi_ablkcipher_free(drvdata);
	ssi_ivgen_fini(drvdata);
	ssi_power_mgr_fini(drvdata);
	ssi_buffer_mgr_fini(drvdata);
	request_mgr_fini(drvdata);
	ssi_sram_mgr_fini(drvdata);
	ssi_fips_fini(drvdata);
#ifdef ENABLE_CC_SYSFS
	ssi_sysfs_fini();
#endif

=======
	struct cc_drvdata *drvdata =
		(struct cc_drvdata *)platform_get_drvdata(plat_dev);

	cc_aead_free(drvdata);
	cc_hash_free(drvdata);
	cc_cipher_free(drvdata);
	cc_ivgen_fini(drvdata);
	cc_pm_fini(drvdata);
	cc_buffer_mgr_fini(drvdata);
	cc_req_mgr_fini(drvdata);
	cc_sram_mgr_fini(drvdata);
	cc_fips_fini(drvdata);
	cc_debugfs_fini(drvdata);
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c
	fini_cc_regs(drvdata);
	cc_clk_off(drvdata);
	free_irq(drvdata->res_irq->start, drvdata);
	drvdata->res_irq = NULL;

	if (drvdata->cc_base) {
		iounmap(drvdata->cc_base);
		release_mem_region(drvdata->res_mem->start,
				   resource_size(drvdata->res_mem));
		drvdata->cc_base = NULL;
		drvdata->res_mem = NULL;
	}

	kfree(drvdata);
	dev_set_drvdata(&plat_dev->dev, NULL);
}

int cc_clk_on(struct cc_drvdata *drvdata)
{
	struct clk *clk = drvdata->clk;
	int rc;

	if (IS_ERR(clk))
		/* Not all devices have a clock associated with CCREE  */
		return 0;

	rc = clk_prepare_enable(clk);
	if (rc)
		return rc;

	return 0;
}

void cc_clk_off(struct cc_drvdata *drvdata)
{
	struct clk *clk = drvdata->clk;

	if (IS_ERR(clk))
		/* Not all devices have a clock associated with CCREE */
		return;

	clk_disable_unprepare(clk);
}

static int cc7x_probe(struct platform_device *plat_dev)
{
	int rc;
<<<<<<< HEAD:drivers/staging/ccree/ssi_driver.c
#if defined(CONFIG_ARM) && defined(CC_DEBUG)
	u32 ctr, cacheline_size;

	asm volatile("mrc p15, 0, %0, c0, c0, 1" : "=r" (ctr));
	cacheline_size =  4 << ((ctr >> 16) & 0xf);
	SSI_LOG_DEBUG("CP15(L1_CACHE_BYTES) = %u , Kconfig(L1_CACHE_BYTES) = %u\n",
		      cacheline_size, L1_CACHE_BYTES);

	asm volatile("mrc p15, 0, %0, c0, c0, 0" : "=r" (ctr));
	SSI_LOG_DEBUG("Main ID register (MIDR): Implementer 0x%02X, Arch 0x%01X, Part 0x%03X, Rev r%dp%d\n",
		      (ctr >> 24), (ctr >> 16) & 0xF, (ctr >> 4) & 0xFFF,
		      (ctr >> 20) & 0xF, ctr & 0xF);
#endif
=======
	struct device *dev = &plat_dev->dev;
>>>>>>> 3fb78e8be9d9428fbc4c016f8b031b01a6fdd63d:drivers/staging/ccree/cc_driver.c

	/* Map registers space */
	rc = init_cc_resources(plat_dev);
	if (rc)
		return rc;

	SSI_LOG(KERN_INFO, "ARM cc7x_ree device initialized\n");

	return 0;
}

static int cc7x_remove(struct platform_device *plat_dev)
{
	SSI_LOG_DEBUG("Releasing cc7x resources...\n");

	cleanup_cc_resources(plat_dev);

	SSI_LOG(KERN_INFO, "ARM cc7x_ree device terminated\n");

	return 0;
}

static const struct of_device_id arm_cc7x_dev_of_match[] = {
	{.compatible = "arm,cryptocell-712-ree"},
	{}
};
MODULE_DEVICE_TABLE(of, arm_cc7x_dev_of_match);

static struct platform_driver cc7x_driver = {
	.driver = {
		   .name = "cc7xree",
		   .of_match_table = arm_cc7x_dev_of_match,
#ifdef CONFIG_PM
		   .pm = &ccree_pm,
#endif
	},
	.probe = cc7x_probe,
	.remove = cc7x_remove,
};

static int __init ccree_init(void)
{
	int ret;

	cc_hash_global_init();

	ret = cc_debugfs_global_init();
	if (ret)
		return ret;

	return platform_driver_register(&cc7x_driver);
}
module_init(ccree_init);

static void __exit ccree_exit(void)
{
	platform_driver_unregister(&cc7x_driver);
	cc_debugfs_global_fini();
}
module_exit(ccree_exit);

/* Module description */
MODULE_DESCRIPTION("ARM TrustZone CryptoCell REE Driver");
MODULE_VERSION(DRV_MODULE_VERSION);
MODULE_AUTHOR("ARM");
MODULE_LICENSE("GPL v2");
